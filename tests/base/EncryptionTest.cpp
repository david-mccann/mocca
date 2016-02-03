/****************************************************************
* Copyright (C) 2016 Andrey Krekhov, David McCann
*
* The content of this file may not be copied and/or distributed
* without the expressed permission of the copyright owner.
*
****************************************************************/

#include "gtest/gtest.h"

#include "mocca/base/Encryption.h"

using namespace mocca;

class EncryptionTest : public ::testing::Test {
protected:
    EncryptionTest() {
        // You can do set-up work for each test here.
    }

    virtual ~EncryptionTest() {
        // You can do clean-up work that doesn't throw exceptions here.
    }
};

TEST_F(EncryptionTest, EncryptDecrypt) {
    std::string plainText = "Hello World";
    std::string key = "very very very secret string!";
    auto encoded = mocca::encryptString(plainText, key);
    ASSERT_EQ(plainText, mocca::decryptString(encoded, key));
    ASSERT_NE(plainText, mocca::decryptString(encoded, "Wrong Key"));
}