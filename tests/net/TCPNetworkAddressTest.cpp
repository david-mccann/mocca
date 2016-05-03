/****************************************************************
* Copyright (C) 2016 Andrey Krekhov, David McCann
*
* The content of this file may not be copied and/or distributed
* without the expressed permission of the copyright owner.
*
****************************************************************/

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