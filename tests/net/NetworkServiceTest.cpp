#include "gtest/gtest.h"

#include "mocca/base/Error.h"
#include "mocca/base/Thread.h"
#include "mocca/base/ByteArray.h"
#include "mocca/net/TCPNetworkAddress.h"
#include "mocca/net/LoopbackNetworkService.h"
#include "mocca/net/LoopbackConnectionAcceptor.h"
#include "mocca/net/TCPNetworkService.h"

#include "mocca/testing/NetworkTesting.h"

#include <future>
#include <algorithm>

using namespace mocca;
using namespace mocca::net;
using namespace mocca::testing;

#ifdef MOCCA_TEST_TCP
typedef ::testing::Types<LoopbackNetworkService, TCPNetworkService> MyTypes;
#else
typedef ::testing::Types<LoopbackNetworkService> MyTypes;
#endif
TYPED_TEST_CASE(NetworkServiceTest, MyTypes);

template<typename T>
class NetworkServiceTest : public ::testing::Test {
protected:
    NetworkServiceTest() {
        // You can do set-up work for each test here.
    }

    virtual ~NetworkServiceTest() {
        // You can do clean-up work that doesn't throw exceptions here.
    }
};

TYPED_TEST(NetworkServiceTest, Identifier)
{
    {
        // identifier is not empty
        TypeParam target;
        auto acceptor = target.bind(createBindingString<TypeParam>());
        auto clientConnection = target.connect(createConnectionString<TypeParam>());
        ASSERT_FALSE(clientConnection->identifier().empty());
        auto serverConnection = acceptor->getConnection();
        ASSERT_FALSE(serverConnection == nullptr);
        ASSERT_FALSE(serverConnection->identifier().empty());
    }
    {
        // identifiers are distinct
        TypeParam target;
        auto acceptor = target.bind(createBindingString<TypeParam>());
        auto clientConnection1 = target.connect(createConnectionString<TypeParam>());
        auto clientConnection2 = target.connect(createConnectionString<TypeParam>());
        auto serverConnection1 = acceptor->getConnection();
        auto serverConnection2 = acceptor->getConnection();
        ASSERT_FALSE(serverConnection1 == nullptr);
        ASSERT_FALSE(serverConnection2 == nullptr);
        ASSERT_NE(clientConnection1->identifier(), clientConnection2->identifier());
        ASSERT_NE(clientConnection2->identifier(), serverConnection1->identifier());
        ASSERT_NE(serverConnection1->identifier(), serverConnection2->identifier());
    }
}

TYPED_TEST(NetworkServiceTest, AcceptorConnections) {
    {
        // cannot connect to an unbound port
        TypeParam target;
        ASSERT_THROW(target.connect(createConnectionString<TypeParam>()), Error);
    }
    {
        // acceptor has no connection when no client connects
        TypeParam target;
        auto acceptor = target.bind(createBindingString<TypeParam>());
        ASSERT_TRUE(acceptor->getConnection() == nullptr);
    }
    {
        // acceptor has a connection when a client connects
        TypeParam target;
        auto acceptor = target.bind(createBindingString<TypeParam>());
        auto connection = target.connect(createConnectionString<TypeParam>());
        ASSERT_FALSE(acceptor->getConnection() == nullptr);
    }
    {
        // acceptor has two conenctions when two clients connect
        TypeParam target;
        auto acceptor = target.bind(createBindingString<TypeParam>());
        auto connection1 = target.connect(createConnectionString<TypeParam>());
        auto connection2 = target.connect(createConnectionString<TypeParam>());
        ASSERT_FALSE(acceptor->getConnection() == nullptr);
        ASSERT_FALSE(acceptor->getConnection() == nullptr);
    }
    {
        // connection is removed when dequeued
        TypeParam target;
        auto acceptor = target.bind(createBindingString<TypeParam>());
        auto connection = target.connect(createConnectionString<TypeParam>());
        auto connectionFromAcceptor = acceptor->getConnection();
        ASSERT_FALSE(connectionFromAcceptor == nullptr);
        ASSERT_TRUE(acceptor->getConnection() == nullptr);
    }
    {
        // each acceptor has its own queue
        TypeParam target;
        auto acceptor1 = target.bind(createBindingString<TypeParam>());
        auto acceptor2 = target.bind(createBindingString<TypeParam>(1));
        auto connection = target.connect(createConnectionString<TypeParam>(1));
        ASSERT_TRUE(acceptor1->getConnection() == nullptr);
        ASSERT_FALSE(acceptor2->getConnection() == nullptr);
    }
}

TYPED_TEST(NetworkServiceTest, SendAndReceive) {
    {
        // client sends to server
        TypeParam target;
        auto acceptor = target.bind(createBindingString<TypeParam>());
        auto clientConnection = target.connect(createConnectionString<TypeParam>());
        auto serverConnection = acceptor->getConnection();
        ASSERT_FALSE(serverConnection == nullptr);
        clientConnection->send((ByteArray() << "Hello World"));
        auto recPacket = ByteArray(serverConnection->receive());
        ASSERT_EQ("Hello World", recPacket.get<std::string>());
    }
    {
        // server sends to client
        TypeParam target;
        auto acceptor = target.bind(createBindingString<TypeParam>());
        auto clientConnection = target.connect(createConnectionString<TypeParam>());
        auto serverConnection = acceptor->getConnection();
        ASSERT_FALSE(serverConnection == nullptr);
        serverConnection->send((ByteArray() << "Hello World"));
        auto recPacket = ByteArray(clientConnection->receive());
        ASSERT_EQ("Hello World", recPacket.get<std::string>());
    }
    {
        // two different clients and two receivers on the same port
        TypeParam target;
        auto acceptor = target.bind(createBindingString<TypeParam>());
        auto clientConnection1 = target.connect(createConnectionString<TypeParam>());
        auto serverConnection1 = acceptor->getConnection();
        ASSERT_FALSE(serverConnection1 == nullptr);
        auto clientConnection2 = target.connect(createConnectionString<TypeParam>());
        auto serverConnection2 = acceptor->getConnection();
        ASSERT_FALSE(serverConnection2 == nullptr);
        clientConnection1->send((ByteArray() << "Hello from 1"));
        clientConnection2->send((ByteArray() << "Hello from 2"));
 
        auto recPacket1 = ByteArray(serverConnection1->receive());
        ASSERT_EQ("Hello from 1", recPacket1.get<std::string>());
        auto recPacket2 = ByteArray(serverConnection2->receive());
        ASSERT_EQ("Hello from 2", recPacket2.get<std::string>());
    }
}

TYPED_TEST(NetworkServiceTest, ReceiveTimeout) {
    TypeParam target;
    auto acceptor = target.bind(createBindingString<TypeParam>());
    auto clientConnection = target.connect(createConnectionString<TypeParam>());
    auto serverConnection = acceptor->getConnection();
    ASSERT_FALSE(serverConnection == nullptr);
    auto recPacket = ByteArray(serverConnection->receive(std::chrono::milliseconds(10)));
    ASSERT_TRUE(recPacket.isEmpty());
}

TYPED_TEST(NetworkServiceTest, ReceiveTimeoutClient) {
    TypeParam target;
    auto acceptor = target.bind(createBindingString<TypeParam>());
    auto clientConnection = target.connect(createConnectionString<TypeParam>());
    auto serverConnection = acceptor->getConnection();
    ASSERT_FALSE(serverConnection == nullptr);
    auto recPacket = ByteArray(clientConnection->receive(std::chrono::milliseconds(10)));
    ASSERT_TRUE(recPacket.isEmpty());
}

TYPED_TEST(NetworkServiceTest, SendReceiveParallel) {
    TypeParam target;
    auto acceptor = target.bind(createBindingString<TypeParam>());
    auto clientConnection = target.connect(createConnectionString<TypeParam>());
    auto serverConnection = acceptor->getConnection();
    ASSERT_FALSE(serverConnection == nullptr);
    
    const int numItems = 200;
    std::vector<std::string> data;
    for (int i = 0; i < numItems; i++) {
        data.push_back("item " + std::to_string(i));
    }

    mocca::Thread t(std::thread([&clientConnection, data] {
        for (auto item : data) {
            clientConnection->send((ByteArray() << item));
        }
    }));

    std::vector<std::future<std::string>> futures;
    for (int i = 0; i < numItems; ++i) {
        futures.push_back(std::async(std::launch::async, [&serverConnection, i]() {
            auto recPacket = ByteArray(serverConnection->receive());
            return recPacket.get<std::string>();
        }));
    }

    std::vector<std::string> result;
    for (int i = 0; i < numItems; i++) {
        result.push_back(futures[i].get());
    }
    for (auto item : data) {
        ASSERT_TRUE(std::find(begin(result), end(result), item) != end(result));
    }
}