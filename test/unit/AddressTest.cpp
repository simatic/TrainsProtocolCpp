/* 
 * File:   TestAddress
 * Author: simatic
 *
 * Created on 4 juil. 2016, 15:47:11
 */

#include <gtest/gtest.h>
#include "Address.hpp"

using namespace common;

/* TEST(AddressTest, testAddrToRank) {
    Address anAdd0(0);
    EXPECT_EQ(-1, anAdd0.addrToRank());
    
    Address anAdd1(1);
    EXPECT_EQ(0, anAdd1.addrToRank());

    Address anAdd0x8000(0x8000);
    EXPECT_EQ(MAX_MEMB-1, anAdd0x8000.addrToRank());
}

TEST(AddressTest, testGetMyAddress) {
    Address anAdd = Address::getMyAddress();
    EXPECT_EQ(-1, anAdd.addrToRank());
}

TEST(AddressTest, testIsNull) {
    Address anAdd0(0);
    EXPECT_TRUE(anAdd0.isNull());
    
    Address anAdd1(1);
    EXPECT_FALSE(anAdd1.isNull());
}

TEST(AddressTest, testRankToAddr) {
    EXPECT_EQ(0, Address(Address::rankToAddr(0)).addrToRank());
    EXPECT_EQ(MAX_MEMB-1, Address(Address::rankToAddr(MAX_MEMB-1)).addrToRank());
}*/
TEST(AddressTest, testInitialize) {
    Address::initialize();
    EXPECT_TRUE(true);
}
