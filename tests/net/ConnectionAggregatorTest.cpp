#include <future>

#include "gtest/gtest.h"

#include "mocca/base/ByteArray.h"
#include "mocca/net/ConnectionAggregator.h"
#include "mocca/net/Error.h"
#include "mocca/net/MoccaNetworkService.h"
#include "mocca/net/stream/TCPNetworkService.h"
#include "mocca/testing/LoopbackPhysicalConnectionAcceptor.h"
#include "mocca/testing/LoopbackPhysicalNetworkService.h"

#include "mocca/testing/NetworkTesting.h"

using namespace mocca;
using namespace mocca::net;
using namespace mocca::testing;

#ifdef MOCCA_TEST_TCP
typedef ::testing::Types<LoopbackPhysicalNetworkService, TCPNetworkService> MyTypes;
#else
typedef ::testing::Types<LoopbackPhysicalNetworkService> MyTypes;
#endif
TYPED_TEST_CASE(ConnectionAggregatorTest, MyTypes);

template <typename T> class ConnectionAggregatorTest : public ::testing::Test {
protected:
    ConnectionAggregatorTest() { service.reset(new MoccaNetworkService<T>()); }

    virtual ~ConnectionAggregatorTest() {
        // You can do clean-up work that doesn't throw exceptions here.
    }

    std::unique_ptr<IProtocolNetworkService> service;
};

TYPED_TEST(ConnectionAggregatorTest, EnqueueDequeue) {
    auto acceptor = this->service->bind(createBindingAddress<TypeParam>());
    ConnectionAggregator target(std::move(acceptor));
    auto clientConnection1 = this->service->connect(createAddress<TypeParam>());
    auto clientConnection2 = this->service->connect(createAddress<TypeParam>());

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

TYPED_TEST(ConnectionAggregatorTest, SendReceiveParallel) {
    auto acceptor = this->service->bind(createBindingAddress<TypeParam>());
    ConnectionAggregator target(std::move(acceptor));
    auto clientConnection1 = this->service->connect(createAddress<TypeParam>());
    auto clientConnection2 = this->service->connect(createAddress<TypeParam>());

    const int numItems = 20;
    std::vector<std::string> data;
    for (int i = 0; i < numItems; i++) {
        data.push_back("item " + std::to_string(i));
    }

    auto sendFunction = [](IProtocolConnection& connection, const std::vector<std::string>& data) {
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

TYPED_TEST(ConnectionAggregatorTest, DisconnectStrategyThrowException) {
    auto acceptor = this->service->bind(createBindingAddress<TypeParam>());
    ConnectionAggregator target(std::move(acceptor), ConnectionAggregator::DisconnectStrategy::ThrowException);
    {
        auto clientConnection = this->service->connect(createAddress<TypeParam>());
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

TYPED_TEST(ConnectionAggregatorTest, DisconnectStrategyRemoveConnection) {
    auto acceptor = this->service->bind(createBindingAddress<TypeParam>());
    ConnectionAggregator target(std::move(acceptor), ConnectionAggregator::DisconnectStrategy::RemoveConnection);
    {
        auto clientConnection = this->service->connect(createAddress<TypeParam>());
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
