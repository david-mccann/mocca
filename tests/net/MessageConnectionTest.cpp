/****************************************************************
* Copyright (C) 2016 Andrey Krekhov, David McCann
*
* The content of this file may not be copied and/or distributed
* without the expressed permission of the copyright owner.
*
****************************************************************/

#include "gtest/gtest.h"

#include "mocca/base/Error.h"
#include "mocca/base/Thread.h"
#include "mocca/net/ConnectionFactorySelector.h"
#include "mocca/testing/NetworkTesting.h"

#include <algorithm>
#include <future>

using namespace mocca;
using namespace mocca::net;
using namespace mocca::testing;

class MessageConnectionTest : public ::testing::TestWithParam<const char*> {
protected:
    MessageConnectionTest() {
        // You can do set-up work for each test here.
        ConnectionFactorySelector::addDefaultFactories();
        target = &ConnectionFactorySelector::messageConnectionFactory(GetParam());
    }

    virtual ~MessageConnectionTest() {
        ConnectionFactorySelector::removeAll();
        // You can do clean-up work that doesn't throw exceptions here.
    }

    IMessageConnectionFactory* target;
};

INSTANTIATE_TEST_CASE_P(InstantiationName, MessageConnectionTest, ::testing::Values(ConnectionFactorySelector::queuePrefixed().c_str(),
                                                                                    ConnectionFactorySelector::loopback().c_str()));

TEST_P(MessageConnectionTest, Identifier) {
    auto machine = createBindingMachine(GetParam());
    auto port = createBindingPort(GetParam());
    {
        // identifier is not empty
        auto acceptor = this->target->bind(machine, port);
        auto clientConnection = this->target->connect(createAddress(GetParam()));
        ASSERT_FALSE(clientConnection->connectionID()->toString().empty());
        auto serverConnection = acceptor->accept();
        ASSERT_FALSE(serverConnection == nullptr);
        ASSERT_FALSE(serverConnection->connectionID()->toString().empty());
    }
    {
        // identifiers are distinct
        auto acceptor = this->target->bind(machine, port);
        auto clientConnection1 = this->target->connect(createAddress(GetParam()));
        auto clientConnection2 = this->target->connect(createAddress(GetParam()));
        auto serverConnection1 = acceptor->accept();
        auto serverConnection2 = acceptor->accept();
        ASSERT_FALSE(serverConnection1 == nullptr);
        ASSERT_FALSE(serverConnection2 == nullptr);
        ASSERT_FALSE(clientConnection1->connectionID() == clientConnection2->connectionID());
        ASSERT_FALSE(clientConnection2->connectionID() == serverConnection1->connectionID());
        ASSERT_FALSE(serverConnection1->connectionID() == serverConnection2->connectionID());
    }
}

TEST_P(MessageConnectionTest, AcceptorConnections) {
    auto machine = createBindingMachine(GetParam());
    auto port = createBindingPort(GetParam());
    {
        // cannot connect to an unbound port
        ASSERT_THROW(this->target->connect(createAddress(GetParam())), Error);
    }
    {
        // acceptor has no connection when no client connects
        auto acceptor = this->target->bind(machine, port);
        ASSERT_TRUE(acceptor->accept(std::chrono::milliseconds(1)) == nullptr);
    }
    {
        // acceptor has a connection when a client connects
        auto acceptor = this->target->bind(machine, port);
        auto connection = this->target->connect(createAddress(GetParam()));
        ASSERT_FALSE(acceptor->accept() == nullptr);
    }
    {
        // acceptor has two conenctions when two clients connect
        auto acceptor = this->target->bind(machine, port);
        auto connection1 = this->target->connect(createAddress(GetParam()));
        auto connection2 = this->target->connect(createAddress(GetParam()));
        ASSERT_FALSE(acceptor->accept() == nullptr);
        ASSERT_FALSE(acceptor->accept() == nullptr);
    }
    {
        // connection is removed when dequeued
        auto acceptor = this->target->bind(machine, port);
        auto connection = this->target->connect(createAddress(GetParam()));
        auto connectionFromAcceptor = acceptor->accept();
        ASSERT_FALSE(connectionFromAcceptor == nullptr);
        ASSERT_TRUE(acceptor->accept(std::chrono::milliseconds(1)) == nullptr);
    }
    {
        // each acceptor has its own queue
        auto acceptor1 = this->target->bind(machine, port);
        auto acceptor2 = this->target->bind(machine, createBindingPort(GetParam(), 1));
        auto connection = this->target->connect(createAddress(GetParam(), 1));
        ASSERT_TRUE(acceptor1->accept(std::chrono::milliseconds(1)) == nullptr);
        ASSERT_FALSE(acceptor2->accept() == nullptr);
    }
}

TEST_P(MessageConnectionTest, SendAndReceive) {
    auto machine = createBindingMachine(GetParam());
    auto port = createBindingPort(GetParam());
    {
        // client sends to server
        auto acceptor = this->target->bind(machine, port);
        auto clientConnection = this->target->connect(createAddress(GetParam()));
        auto serverConnection = acceptor->accept();
        ASSERT_FALSE(serverConnection == nullptr);
        clientConnection->send(Message{createMessagePart("Hello World")});
        auto recMessage = serverConnection->receive();
        ASSERT_EQ(1, recMessage.size());
        ASSERT_EQ("Hello World", readMessagePart(*recMessage[0]));
    }
    {
        // server sends to client
        auto acceptor = this->target->bind(machine, port);
        auto clientConnection = this->target->connect(createAddress(GetParam()));
        auto serverConnection = acceptor->accept();
        ASSERT_FALSE(serverConnection == nullptr);
        serverConnection->send(Message{createMessagePart("Hello World")});
        auto recMessage = clientConnection->receive();
        ASSERT_EQ(1, recMessage.size());
        ASSERT_EQ("Hello World", readMessagePart(*recMessage[0]));
    }
    {
        // two different clients and two receivers on the same port
        auto acceptor = this->target->bind(machine, port);
        auto clientConnection1 = this->target->connect(createAddress(GetParam()));
        auto serverConnection1 = acceptor->accept();
        ASSERT_FALSE(serverConnection1 == nullptr);
        auto clientConnection2 = this->target->connect(createAddress(GetParam()));
        auto serverConnection2 = acceptor->accept();
        ASSERT_FALSE(serverConnection2 == nullptr);
        clientConnection1->send(Message{createMessagePart("Hello from 1")});
        clientConnection2->send(Message{createMessagePart("Hello from 2")});

        auto recMessage1 = serverConnection1->receive();
        ASSERT_EQ(1, recMessage1.size());
        ASSERT_EQ("Hello from 1", readMessagePart(*recMessage1[0]));
        auto recMessage2 = serverConnection2->receive();
        ASSERT_EQ(1, recMessage2.size());
        ASSERT_EQ("Hello from 2", readMessagePart(*recMessage2[0]));
    }
}

TEST_P(MessageConnectionTest, MultiPartMessage) {
    auto machine = createBindingMachine(GetParam());
    auto port = createBindingPort(GetParam());

    auto acceptor = this->target->bind(machine, port);
    auto clientConnection = this->target->connect(createAddress(GetParam()));
    auto serverConnection = acceptor->accept();
    ASSERT_FALSE(serverConnection == nullptr);
    Message message;
    message.push_back(createMessagePart("Hello World 1"));
    message.push_back(createMessagePart("Hello World 2"));
    message.push_back(createMessagePart("Hello World 3"));

    clientConnection->send(message);
    auto recMessage = serverConnection->receive();
    ASSERT_EQ(3, recMessage.size());
    ASSERT_EQ("Hello World 1", readMessagePart(*recMessage[0]));
    ASSERT_EQ("Hello World 2", readMessagePart(*recMessage[1]));
    ASSERT_EQ("Hello World 3", readMessagePart(*recMessage[2]));
}

TEST_P(MessageConnectionTest, SendReceiveParallel) {
    auto machine = createBindingMachine(GetParam());
    auto port = createBindingPort(GetParam());
    auto acceptor = this->target->bind(machine, port);
    auto clientConnection = this->target->connect(createAddress(GetParam()));
    auto serverConnection = acceptor->accept();
    ASSERT_FALSE(serverConnection == nullptr);

    const int numItems = 200;
    std::vector<std::string> data;
    for (int i = 0; i < numItems; i++) {
        data.push_back("item " + std::to_string(i));
    }
    {
        mocca::AutoJoinThread a([&clientConnection, data] {
            for (auto item : data) {
                clientConnection->send(Message{createMessagePart(item)});
            }
        });
    }

    std::vector<std::future<std::string>> futures;
    for (int i = 0; i < numItems; ++i) {
        futures.push_back(std::async(std::launch::async, [&serverConnection, i]() {
            auto recMessage = serverConnection->receive();
            return readMessagePart(*recMessage[0]);
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
