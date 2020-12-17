#include <gtest/gtest.h>
#include "HashTable.hpp"
#include <string>
#include <iostream>

TEST(ComplexTest, NormalData){

    HashTable<string> hashTable;
    hashTable.add("1.apple");
    hashTable.add("2.glass");
    hashTable.add("4.jackpot");
    hashTable.add("3.king");
    hashTable.add("5.lor");
    hashTable.add("7.west");
    hashTable.add("9.work");

    for(const string& temp: hashTable){
        cout << temp << " ";
    }

    ASSERT_FALSE(hashTable.hasElem("king"));
    ASSERT_TRUE(hashTable.hasElem("2.glass"));


    hashTable.deleteElem("3.king");
    ASSERT_FALSE(hashTable.hasElem("3.king"));
    ASSERT_THROW(hashTable.deleteElem("4.zz"),LinkedListException);

    HashTable<string> hashTable1 = hashTable;
    hashTable.add("10.keys");
    hashTable.add("13.row");

    ASSERT_FALSE(hashTable1.hasElem("10.keys"));

    hashTable.clear();
    ASSERT_TRUE(hashTable.empty());
    hashTable.add("ghost");
    ASSERT_TRUE(hashTable.hasElem("ghost"));
}