#include "gtest/gtest.h"

#include "mocca/net/TCPNetworkAddress.h"
#include "mocca/net/Error.h"

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
	ASSERT_THROW(TCPNetworkAddress("", 1234), NetworkError);
	ASSERT_THROW(TCPNetworkAddress("", 66666), NetworkError);
	ASSERT_THROW(TCPNetworkAddress("", -1), NetworkError);
}

TEST_F(TCPNetworkAddressTest, Ctor2) {
	{
		TCPNetworkAddress target("localhost:1234");
		ASSERT_EQ("localhost", target.ip());
		ASSERT_EQ(1234, target.port());
	}
	ASSERT_THROW(TCPNetworkAddress("localhost"), NetworkError);
	ASSERT_THROW(TCPNetworkAddress("localhost:bla:1234"), NetworkError);
	ASSERT_THROW(TCPNetworkAddress("localhost:66666"), NetworkError);
	ASSERT_THROW(TCPNetworkAddress("localhost:-1"), NetworkError);
}

TEST_F(TCPNetworkAddressTest, ConnectionString) {
	ASSERT_EQ("localhost:1234", TCPNetworkAddress("localhost", 1234).connectionString());
	ASSERT_EQ("168.0.0.1:1234", TCPNetworkAddress("168.0.0.1", 1234).connectionString());
}

TEST_F(TCPNetworkAddressTest, BindingString) {
	ASSERT_EQ("1234", TCPNetworkAddress("localhost", 1234).bindingString());
	ASSERT_EQ("1234", TCPNetworkAddress("168.0.0.1", 1234).bindingString());
}