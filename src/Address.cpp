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

#include "Address.hpp"
#include <cassert>

using namespace common;

static Address myAddress(0);

Address::Address(address_t ad) : m_ad(ad) {
}

const int Address::addrToRank() {
  unsigned short rank = 0;
  address_t ad(m_ad);
  while (((ad & 0x0001) == 0) && (rank < MAX_MEMB)) {
    ad /= 2;
    rank++;
  }
  if (rank < MAX_MEMB)
    return rank;
  else
    return -1;
}

const Address& Address::getMyAddress() {
    // TODO Besoin d'implanter correctement le code de getMyAddress()
    return myAddress;
}

const bool Address::isNull() {
    return m_ad == 0;
}

address_t Address::rankToAddr(const int rank) {
    assert( (0 <= rank) && (rank < MAX_MEMB));
    return 1 << rank;
}

