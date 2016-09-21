/*The MIT License(MIT)

Copyright(c) 2016 David McCann

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files(the
"Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and / or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to
the following conditions :

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.*/

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