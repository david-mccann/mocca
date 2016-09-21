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

#include "mocca/net/stream/TCPNetworkAddress.h"
#include "mocca/base/Error.h"

using namespace mocca;
using namespace mocca::net;

class TCPNetworkAddressTest : public ::testing::Test {
protected:
	TCPNetworkAddressTest() {
		// You can do set-up work for each test here.
	}

	virtual ~TCPNetworkAddressTest() {
		// You can do clean-up work that doesn't throw exceptions here.
	}
};

TEST_F(TCPNetworkAddressTest, Ctor1) {
	{
		TCPNetworkAddress target("localhost", 1234);
		ASSERT_EQ("localhost", target.ip());
		ASSERT_EQ(1234, target.port());
	}
	ASSERT_THROW(TCPNetworkAddress("", 1234), Error);
	ASSERT_THROW(TCPNetworkAddress("", 66666), Error);
	ASSERT_THROW(TCPNetworkAddress("", -1), Error);
}

TEST_F(TCPNetworkAddressTest, Ctor2) {
	{
		TCPNetworkAddress target("localhost:1234");
		ASSERT_EQ("localhost", target.ip());
		ASSERT_EQ(1234, target.port());
	}
	ASSERT_THROW(TCPNetworkAddress("localhost"), Error);
	ASSERT_THROW(TCPNetworkAddress("localhost:bla:1234"), Error);
	ASSERT_THROW(TCPNetworkAddress("localhost:66666"), Error);
	ASSERT_THROW(TCPNetworkAddress("localhost:-1"), Error);
}

TEST_F(TCPNetworkAddressTest, address) {
	ASSERT_EQ("localhost:1234", TCPNetworkAddress("localhost", 1234).address());
	ASSERT_EQ("168.0.0.1:1234", TCPNetworkAddress("168.0.0.1", 1234).address());
}

TEST_F(TCPNetworkAddressTest, BindingString) {
	ASSERT_EQ("1234", TCPNetworkAddress("localhost", 1234).bindingString());
	ASSERT_EQ("1234", TCPNetworkAddress("168.0.0.1", 1234).bindingString());
}