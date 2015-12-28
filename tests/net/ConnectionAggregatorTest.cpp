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
    ConnectionAggregator target(mocca::makeUniquePtrVec(this->target->bind(createBindingAddress(GetParam()))));

    auto clientConnection1 = this->target->connect(createAddress(GetParam()));
    auto clientConnection2 = this->target->connect(createAddress(GetParam()));

    ByteArray packet1 = mocca::makeFormattedByteArray("Hello 1");
    ByteArray packet2 = mocca::makeFormattedByteArray("Hello 2");

    clientConnection1->send(std::move(packet1));
    clientConnection2->send(std::move(packet2));

    auto data1 = target.receive(std::chrono::milliseconds(100));
    auto data2 = target.receive(std::chrono::milliseconds(100));
    ASSERT_FALSE(data1.isNull());
    ASSERT_FALSE(data2.isNull());
    ByteArray recPacket1(data1.release().message);
    ByteArray recPacket2(data2.release().message);
    auto recStr1 = std::get<0>(mocca::parseFormattedByteArray<std::string>(recPacket1));
    auto recStr2 = std::get<0>(mocca::parseFormattedByteArray<std::string>(recPacket2));
    ASSERT_TRUE(recStr1 == "Hello 1" && recStr2 == "Hello 2" || recStr1 == "Hello 2" && recStr2 == "Hello 1");
}

TEST_P(ConnectionAggregatorTest, MultipleAcceptors) {
    ConnectionAggregator target(mocca::makeUniquePtrVec(this->target->bind(createBindingAddress(GetParam())),
                                                        this->target->bind(createBindingAddress(GetParam(), 1))));

    auto clientConnection1 = this->target->connect(createAddress(GetParam()));
    auto clientConnection2 = this->target->connect(createAddress(GetParam(), 1));

    ByteArray packet1 = mocca::makeFormattedByteArray("Hello 1");
    ByteArray packet2 = mocca::makeFormattedByteArray("Hello 2");

    clientConnection1->send(std::move(packet1));
    clientConnection2->send(std::move(packet2));

    auto data1 = target.receive(std::chrono::milliseconds(100));
    auto data2 = target.receive(std::chrono::milliseconds(100));
    ASSERT_FALSE(data1.isNull());
    ASSERT_FALSE(data2.isNull());
    ByteArray recPacket1(data1.release().message);
    ByteArray recPacket2(data2.release().message);
    auto recStr1 = std::get<0>(mocca::parseFormattedByteArray<std::string>(recPacket1));
    auto recStr2 = std::get<0>(mocca::parseFormattedByteArray<std::string>(recPacket2));
    ASSERT_TRUE(recStr1 == "Hello 1" && recStr2 == "Hello 2" || recStr1 == "Hello 2" && recStr2 == "Hello 1");
}

TEST_P(ConnectionAggregatorTest, SendReceiveParallel) {
    ConnectionAggregator target(mocca::makeUniquePtrVec(this->target->bind(createBindingAddress(GetParam()))));

    auto clientConnection1 = this->target->connect(createAddress(GetParam()));
    auto clientConnection2 = this->target->connect(createAddress(GetParam()));

    const int numItems = 20;
    std::vector<std::string> data;
    for (int i = 0; i < numItems; i++) {
        data.push_back("item " + std::to_string(i));
    }

    auto sendFunction = [](IMessageConnection& connection, const std::vector<std::string>& data) {
        for (auto item : data) {
            connection.send(mocca::makeFormattedByteArray(item));
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
                ByteArray recPacket(std::move(envelope.message));
                result.push_back(std::get<0>(mocca::parseFormattedByteArray<std::string>(recPacket)));
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
    ConnectionAggregator target(mocca::makeUniquePtrVec(this->target->bind(createBindingAddress(GetParam()))),
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
    ConnectionAggregator target(mocca::makeUniquePtrVec(this->target->bind(createBindingAddress(GetParam()))));
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
