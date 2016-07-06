/* 
 * File:   TestAddress
 * Author: simatic
 *
 * Created on 4 juil. 2016, 15:47:11
 */

#include <gtest/gtest.h>
#include "Address.hpp"

using namespace common;

TEST(AddressTest, testAddrToRank) {
    Address* myAdd = Address::getMyAddress();
    EXPECT_EQ(0, myAdd->addrToRank());
}

TEST(AddressTest, testGetPort) {
    Address* myAdd = Address::getMyAddress();
    EXPECT_EQ(2123, stoi(myAdd->getPort(123)));
}

TEST(AddressTest, testRankToAddr) {
    Address* myAdd = Address::getMyAddress();
    Address* anAdd = Address::rankToAddress(0);
    EXPECT_EQ(myAdd, anAdd);
}
