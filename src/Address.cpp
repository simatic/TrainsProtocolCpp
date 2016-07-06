/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Address.cpp
 * Author: simatic
 * 
 * Created on 4 juillet 2016, 15:40
 */

#include <cassert>
#include <iostream>
#include <vector>
#include <map>
#include <string>

#include <unistd.h>
#include <stdlib.h>

#include "Address.hpp"
#include "pugixml.hpp"

#define LOCALHOST_NAME_MAX_LEN 256

using namespace std;
using namespace common;

static vector<Address*> allPossibleAddresses(MAX_MEMB, nullptr);
static Address* myAddress = nullptr;

Address::Address(int rank, std::string hostname, std::string portBase) : m_ad(1<<rank), m_rank(rank), m_hostname(hostname), m_portBase(portBase)
{
}

const int Address::addrToRank() {
    return m_rank;
}

void dumpAttributes(const map<string,bool> &attributeMap) {
    cerr << " (Reminder: Required attributes are ";
    for (auto const& element : attributeMap) {
        cerr << "'" << element.first << "' ";
    }
    cerr << ")";
}

string Address::getHostname() {
    return m_hostname;
}

Address* Address::getMyAddress() {
    if (myAddress == nullptr) {
        Address::initialize();
    }
    return myAddress;
}

string Address::getPort(unsigned int offset) {
    int portBaseAsInt = stoi(m_portBase);
    return to_string(portBaseAsInt + offset);
}

void Address::initialize() {
    map<string,bool> attributeMustBeNumeric = {
                { "Rank", true },
                { "Hostname", false },
                { "PortBase", true } };

    pugi::xml_document doc;
    if (!doc.load_file(CONFIG_FILE_NAME)) {
        cerr << "Problem during analysis of file \"" << CONFIG_FILE_NAME << "\"" << endl;
        abort();
    }
    
    pugi::xml_node listParticipants = doc.child("ListParticipants");

    for (pugi::xml_node participant: listParticipants.children("Participant")) {
        map<string,string> mapNameValue;
        
        for (pugi::xml_attribute attr: participant.attributes()) {
            string name  = attr.name();
            string value = attr.value();
            try {
                if (attributeMustBeNumeric.at(name)) {
                    try {
                        (void)stoi(value);
                    }
                    catch (const invalid_argument& ia) {
                        cerr << "Error : In file \"" << CONFIG_FILE_NAME << "\", Attribute '" << name << "' has value '" << value << "' which is *not* numeric (while it *must be* numeric)" << endl;                
                        abort();
                    }
                }
                mapNameValue[name] = value;
            }
            catch (const out_of_range& oor) {
                cerr << "Error : In file \"" << CONFIG_FILE_NAME << "\", Attribute name '" << name << "' is not recognized as a standard attribute";
                dumpAttributes(attributeMustBeNumeric);
                cerr << endl;
                abort();
            }
        }

        if (mapNameValue.size() != attributeMustBeNumeric.size()) {
            cerr << "Error : In file \"" << CONFIG_FILE_NAME << "\", line with ";
            for (auto const& element : mapNameValue) {
                cerr << element.first << "=\"" << element.second << "\" ";
            }
            cerr << " has " << mapNameValue.size() << " attributes defined instead of the required " << attributeMustBeNumeric.size();
            dumpAttributes(attributeMustBeNumeric);
            cerr << endl;
            abort();
        }
        int rank = stoi(mapNameValue["Rank"]);
        try {
            if (allPossibleAddresses.at(rank) != nullptr) {
                cerr << "Error : In file \"" << CONFIG_FILE_NAME << "\", at least 2 lines define a Rank with value '" << rank << "'" << endl;
                abort();            
            }
            allPossibleAddresses.at(rank) = new Address(rank, mapNameValue["Hostname"], mapNameValue["PortBase"]);
        }
        catch (const out_of_range& oor) {
            cerr << "Error : In file \"" << CONFIG_FILE_NAME << "\", a line defines a Rank with value '" << rank << "' which is not in interval [0," << MAX_MEMB << "[" << endl;
            abort();
        }
    }

    char localhostName[LOCALHOST_NAME_MAX_LEN];
    if(gethostname(localhostName,LOCALHOST_NAME_MAX_LEN) != 0) {
        cerr << "Error while calling gethostname" << endl;
        abort();
    }
    char *localPortBase = getenv(PORT_BASE_ENV_VAR_NAME);
    if (localPortBase == NULL) {
        cerr << "Error: \"" << PORT_BASE_ENV_VAR_NAME << "\" environment variable is not defined" << endl;
        abort();
    }
    
    // We replace all addresses containing localhost by localhostName
    for (const auto add: allPossibleAddresses) {
        if ((add != nullptr) && (add->m_hostname == "localhost")) {
            add->m_hostname = localhostName;
        }
    }

    // We try to set myAddress
    for (const auto add: allPossibleAddresses) {
        if ((add != nullptr) && (add->m_hostname == localhostName) && (add->m_portBase == localPortBase)) {
            myAddress = add;
        }
    }
    if (myAddress == nullptr) {
        cerr << "Error : In file \"" << CONFIG_FILE_NAME << "\", there is no line with (Hostname=\"localhost\" or Hostname=\"" << localhostName << "\") AND PortBase=\"" << localPortBase << "\" (which is the value of \"" << PORT_BASE_ENV_VAR_NAME << "\" environment variable)" << endl;
        abort();
    }
}

Address* Address::rankToAddress(const int rank) {
    if (myAddress == nullptr) {
        Address::initialize();
    }
    return allPossibleAddresses.at(rank);
}

