/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Address.hpp
 * Author: simatic
 *
 * Created on 4 juillet 2016, 15:39
 */

#pragma once

#include <string>

namespace common {

    typedef unsigned short address_t;

    /**
     * @brief Maximal number of participants
     */
#define MAX_MEMB (8 * sizeof(address_t))

    class Address {
    public:

        Address(int rank, std::string hostname, std::string port);
        
        /**
         * @brief Returns the rank corresponding to address of current object
         * @param[in] ad Address to convert
         * @return the corresponding rank (value between 0 and (@a MAX_MEMB-1)) or -1 if @a addrIsNull(ad) is true
         */
        const int addrToRank();

        std::string getHostname();
        std::string getPort();

        /**
         * @brief Return the current Address of the process
         */
        static Address* getMyAddress();

        static void initialize();
        
        /**
         * @brief Tests if an address is null
         */
        const bool isNull();

        /**
         * @brief Returns the address corresponding to rank @a rank
         * @param[in] rank Rank to convert
         * @return the corresponding address if @a rank is between 0 and (@a MAX_MEMB-1) and nullAddress otherwise
         */
        static address_t rankToAddr(const int rank);

        // TODO Besoin de definir les output

        // TODO Besoin de definir les ==

    protected:
    private:
        address_t   m_ad;
        int         m_rank;
        std::string m_hostname;
        std::string m_port;        
    };

}
