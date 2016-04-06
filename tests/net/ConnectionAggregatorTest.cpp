/****************************************************************
* Copyright (C) 2016 Andrey Krekhov, David McCann
*
* The content of this file may not be copied and/or distributed
* without the expressed permission of the copyright owner.
*
****************************************************************/

#include "gtest/gtest.h"

#include "mocca/base/ByteArray.h"
#include "mocca/base/ContainerTools.h"
#include "mocca/base/Error.h"
#include "mocca/base/Thread.h"
#include "mocca/net/ConnectionAggregator.h"
#include "mocca/net/ConnectionFactorySelector.h"
#include "mocca/net/NetworkError.h"
#include "mocca/testing/NetworkTesting.h"

#include <algorithm>
#include <future>

using namespace mocca;
using namespace mocca::net;
using namespace mocca::testing;

class ConnectionAggregatorTest : public ::testing::TestWithParam<const char*> {
protected:
    ConnectionAggregatorTest() {
        // You can do set-up work for each test here.
        ConnectionFactorySelector::addDefaultFactories();
        target = &ConnectionFactorySelector::messageConnectionFactory(GetParam());
    }

    virtual ~ConnectionAggregatorTest() {
        ConnectionFactorySelector::removeAll();
        // You can do clean-up work that doesn't throw exceptions here.
    }

    IMessageConnectionFactory* target;
};

INSTANTIATE_TEST_CASE_P(InstantiationName, ConnectionAggregatorTest, ::testing::Values(ConnectionFactorySelector::queuePrefixed().c_str(),
                                                                                       ConnectionFactorySelector::loopback().c_str()));

TEST_P(ConnectionAggregatorTest, EnqueueDequeue) {
    auto machine = createBindingMachine(GetParam());
    auto port = createBindingPort(GetParam());
    ConnectionAggregator target(mocca::makeUniquePtrVec(this->target->bind(machine, port)));

    auto clientConnection1 = this->target->connect(createAddress(GetParam()));
    auto clientConnection2 = this->target->connect(createAddress(GetParam()));

    clientConnection1->send(Message{ createMessagePart("Hello 1") });
    clientConnection2->send(Message{ createMessagePart("Hello 2") });

    auto data1 = target.receive(std::chrono::milliseconds(100));
    auto data2 = target.receive(std::chrono::milliseconds(100));
    ASSERT_FALSE(data1.isNull());
    ASSERT_FALSE(data2.isNull());
    auto recPacket1(data1.release().message);
    auto recPacket2(data2.release().message);
    auto recStr1 = readMessagePart(*recPacket1[0]);
    auto recStr2 = readMessagePart(*recPacket2[0]);
    ASSERT_TRUE(recStr1 == "Hello 1" && recStr2 == "Hello 2" || recStr1 == "Hello 2" && recStr2 == "Hello 1");
}

TEST_P(ConnectionAggregatorTest, MultipleAcceptors) {
    auto machine = createBindingMachine(GetParam());
    auto port1 = createBindingPort(GetParam());
    auto port2 = createBindingPort(GetParam(), 1);
    ConnectionAggregator target(mocca::makeUniquePtrVec(this->target->bind(machine, port1), this->target->bind(machine, port2)));

    auto clientConnection1 = this->target->connect(createAddress(GetParam()));
    auto clientConnection2 = this->target->connect(createAddress(GetParam(), 1));

    clientConnection1->send(Message{ createMessagePart("Hello 1") });
    clientConnection2->send(Message{ createMessagePart("Hello 2") });

    auto data1 = target.receive(std::chrono::milliseconds(100));
    auto data2 = target.receive(std::chrono::milliseconds(100));
    ASSERT_FALSE(data1.isNull());
    ASSERT_FALSE(data2.isNull());
    auto recPacket1(data1.release().message);
    auto recPacket2(data2.release().message);
    auto recStr1 = readMessagePart(*recPacket1[0]);
    auto recStr2 = readMessagePart(*recPacket2[0]);
    ASSERT_TRUE(recStr1 == "Hello 1" && recStr2 == "Hello 2" || recStr1 == "Hello 2" && recStr2 == "Hello 1");
}

TEST_P(ConnectionAggregatorTest, SendReceiveParallel) {
    auto machine = createBindingMachine(GetParam());
    auto port = createBindingPort(GetParam());
    ConnectionAggregator target(mocca::makeUniquePtrVec(this->target->bind(machine, port)));

    auto clientConnection1 = this->target->connect(createAddress(GetParam()));
    auto clientConnection2 = this->target->connect(createAddress(GetParam()));

    const int numItems = 20;
    std::vector<std::string> data;
    for (int i = 0; i < numItems; i++) {
        data.push_back("item " + std::to_string(i));
    }

    auto sendFunction = [](IMessageConnection& connection, const std::vector<std::string>& data) {
        for (auto item : data) {
            connection.send(Message{ createMessagePart(item) });
            static int sleepTime = 0;
            sleepTime = (sleepTime + 1) % 3;
            std::this_thread::sleep_for(std::chrono::milliseconds(sleepTime));
        }
    };
    auto clientFut1 = async(std::launch::async, sendFunction, std::ref(*clientConnection1), std::ref(data));
    auto clientFut2 = async(std::launch::async, sendFunction, std::ref(*clientConnection2), std::ref(data));

    auto receiveFunction = [](ConnectionAggregator& aggregator, int numItems) {
        std::vector<std::string> result;
        while (result.size() != numItems) {
            auto envelopeNullable = aggregator.receive(std::chrono::milliseconds(50));
            if (!envelopeNullable.isNull()) {
                auto envelope = envelopeNullable.release();
                auto recPacket(envelope.message);
                result.push_back(readMessagePart(*recPacket[0]));
            }
        }
        return result;
    };
    auto serverFut = std::async(receiveFunction, std::ref(target), 2 * numItems);
    std::vector<std::string> result = serverFut.get();
    for (auto str : data) {
        ASSERT_EQ(2, std::count(begin(result), end(result), str));
    }
}

TEST_P(ConnectionAggregatorTest, DisconnectStrategyThrowException) {
    auto machine = createBindingMachine(GetParam());
    auto port = createBindingPort(GetParam());
    ConnectionAggregator target(mocca::makeUniquePtrVec(this->target->bind(machine, port)),
                                ConnectionAggregator::DisconnectStrategy::ThrowException);
    {
        auto clientConnection = this->target->connect(createAddress(GetParam()));
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    bool exceptionCaught = false;
    for (int i = 0; i < 10; ++i) {
        try {
            target.receive(std::chrono::milliseconds(0));
        } catch (mocca::net::ConnectionClosedError) {
            exceptionCaught = true;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
    ASSERT_TRUE(exceptionCaught);
}

TEST_P(ConnectionAggregatorTest, DisconnectStrategyRemoveConnection) {
    auto machine = createBindingMachine(GetParam());
    auto port = createBindingPort(GetParam());
    ConnectionAggregator target(mocca::makeUniquePtrVec(this->target->bind(machine, port)));
    {
        auto clientConnection = this->target->connect(createAddress(GetParam()));
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    bool exceptionCaught = false;
    for (int i = 0; i < 10; ++i) {
        try {
            target.receive(std::chrono::milliseconds(0));
        } catch (mocca::net::ConnectionClosedError) {
            exceptionCaught = true;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
    }
    ASSERT_FALSE(exceptionCaught);
}
