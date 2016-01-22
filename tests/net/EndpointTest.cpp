/****************************************************************
* Copyright (C) 2016 Andrey Krekhov, David McCann
*
* The content of this file may not be copied and/or distributed
* without the expressed permission of the copyright owner.
*
****************************************************************/

#include "gtest/gtest.h"

#include "mocca/net/Endpoint.h"
#include "mocca/base/Error.h"

using namespace mocca::net;

class EndpointTest : public ::testing::Test {
protected:
    EndpointTest() {
        // You can do set-up work for each test here.
    }

    virtual ~EndpointTest() {
        // You can do clean-up work that doesn't throw exceptions here.
    }
};

TEST_F(EndpointTest, Ctor) {
    {
        Endpoint ep("protocol", "machine", "port");
        ASSERT_EQ("protocol", ep.protocol);
        ASSERT_EQ("machine", ep.machine);
    }
    {
        Endpoint ep("protocol:machine:port");
        ASSERT_EQ("protocol", ep.protocol);
        ASSERT_EQ("machine", ep.machine);
        ASSERT_EQ("port", ep.port);
    }
    {
        ASSERT_THROW(Endpoint("malformed"), mocca::Error);
    }
}

TEST_F(EndpointTest, ToString) {
    Endpoint ep("protocol", "machine", "port");
    Endpoint ep2(ep.toString());
    ASSERT_EQ(ep, ep2);
}