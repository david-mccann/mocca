CMAKE_MINIMUM_REQUIRED(VERSION 3.1.0)
PROJECT(mocca-test LANGUAGES CXX)

ADD_EXECUTABLE(mocca-test
	base/EndianTest.cpp
	base/ExplicitTest.cpp
	base/MessageQueueTest.cpp
	base/NullableTest.cpp
	base/ObjectPoolTest.cpp
	base/StringToolsTest.cpp
	base/ThreadTest.cpp
	base/BidirectionalMapTest.cpp
	base/CommandLineParserTest.cpp
	base/ContainerToolsTest.cpp
	base/EncryptionTest.cpp

	fs/PathTest.cpp

	net/FramingUtilsTest.cpp
	net/MessageConnectionTest.cpp
	net/TCPNetworkAddressTest.cpp
	net/WebsocketTest.cpp
	net/ConnectionAggregatorTest.cpp
	net/EndpointTest.cpp)

OPTION(MOCCA_TEST_TCP "Run network tests with TCP network service (slows down test execution)" OFF)
IF (${MOCCA_TEST_TCP})
	TARGET_COMPILE_DEFINITIONS(mocca-test PRIVATE MOCCA_TEST_TCP)
ENDIF ()

TARGET_LINK_LIBRARIES(mocca-test PRIVATE mocca gtest gtest_main)
