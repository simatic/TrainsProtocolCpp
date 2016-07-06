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
    
#define PORT_BASE_ENV_VAR_NAME "BROADCAST_PORT_BASE"
    
#define CONFIG_FILE_NAME "test/unit/addr_file.xml"

    class Address {
    public:

        Address(int rank, std::string hostname, std::string portBase);
        
        /**
         * @brief Returns the rank corresponding to address of current object
         * @param[in] ad Address to convert
         * @return the corresponding rank (value between 0 and (@a MAX_MEMB-1)) or -1 if @a addrIsNull(ad) is true
         */
        const int addrToRank();

        std::string getHostname();

        /**
         * @brief Return the current Address of the process
         */
        static Address* getMyAddress();

        /**
         * @brief Returns a port number which is @a offset away from current 
         *        objet's @a m_portBase
         * @param[in] offset Offset to apply
         * @return Resulting offset expressed as a string
         */
        std::string getPort(unsigned int offset);
        
        /**
         * @brief Returns the address corresponding to rank @a rank
         * @param[in] rank Rank to convert
         * @return the corresponding address if @a rank is between 0 and (@a MAX_MEMB-1) and nullAddress otherwise
         */
        static Address* rankToAddress(const int rank);

        // TODO Besoin de definir les output

        // TODO Besoin de definir les ==

    protected:
    private:
        static void initialize();

        address_t   m_ad;
        int         m_rank;
        std::string m_hostname;
        std::string m_portBase;        
    };

}
