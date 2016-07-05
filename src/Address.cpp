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

#include <stdlib.h>

#include "Address.hpp"
#include "pugixml.hpp"

using namespace std;
using namespace common;

static vector<Address*> vecAddress(MAX_MEMB, nullptr);
static Address* myAddress = nullptr;

Address::Address(int rank, std::string hostname, std::string port) : m_ad(1<<rank), m_rank(rank), m_hostname(hostname), m_port(port)
{
}

const int Address::addrToRank() {
    return m_rank;
}

Address* Address::getMyAddress() {
    // TODO Besoin d'implanter correctement le code de getMyAddress()
    return myAddress;
}

void dumpAttributes(const map<string,bool> &attributeMap) {
    cerr << " (Reminder: Required attributes are ";
    for (auto const& element : attributeMap) {
        cerr << "'" << element.first << "' ";
    }
    cerr << ")";
}

void Address::initialize() {
    map<string,bool> attributeMustBeNumeric = {
                { "Rank", true },
                { "Hostname", false },
                { "Port", true } };

    pugi::xml_document doc;
    if (!doc.load_file("test/unit/addr_file.xml")) {
        cerr << "Problem during analysis of 'addr_file.xml'" << endl;
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
                        cerr << "Error : In addr_file.xml, Attribute '" << name << "' has value '" << value << "' which is *not* numeric (while it *must be* numeric)" << endl;                
                        abort();
                    }
                }
                mapNameValue[name] = value;
            }
            catch (const out_of_range& oor) {
                cerr << "Error : In addr_file.xml, Attribute name '" << name << "' is not recognized as a standard attribute";
                dumpAttributes(attributeMustBeNumeric);
                cerr << endl;
                abort();
            }
        }

        if (mapNameValue.size() != attributeMustBeNumeric.size()) {
            cerr << "Error : In addr_file.xml, line with ";
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
            if (vecAddress.at(rank) != nullptr) {
                cerr << "Error : In addr_file.xml, at least 2 lines define a Rank with value '" << rank << "'" << endl;
                abort();            
            }
            vecAddress.at(rank) = new Address(rank, mapNameValue["Hostname"], mapNameValue["Port"]);
        }
        catch (const out_of_range& oor) {
            cerr << "Error : In addr_file.xml, a line defines a Rank with value '" << rank << "' which is not in interval [0," << MAX_MEMB << "[" << endl;
            abort();
        }
    }
    for (const auto add: vecAddress) {
        if (add != nullptr) {
            cerr << "**** Rank:" << add->m_rank << ", m_ad="<< add->m_ad << " / " << add->m_hostname << ":" << add->m_port << endl;
        }
    }
}

const bool Address::isNull() {
    return m_ad == 0;
}

address_t Address::rankToAddr(const int rank) {
    assert( (0 <= rank) && (rank < MAX_MEMB));
    return 1 << rank;
}

