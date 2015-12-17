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
        Endpoint ep("protocol", "address");
        ASSERT_EQ("protocol", ep.protocol());
        ASSERT_EQ("address", ep.address());
    }
    {
        Endpoint ep("protocol:address");
        ASSERT_EQ("protocol", ep.protocol());
        ASSERT_EQ("address", ep.address());
    }
    {
        ASSERT_THROW(Endpoint("malformed"), mocca::Error);
    }
}

TEST_F(EndpointTest, ToString) {
    Endpoint ep("protocol", "address");
    Endpoint ep2(ep.toString());
    ASSERT_EQ(ep, ep2);
}