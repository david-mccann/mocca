#include "gtest/gtest.h"

#include "mocca/base/ByteArray.h"
#include "mocca/base/MessageQueue.h"
#include "mocca/testing/LoopbackPhysicalConnection.h"
#include "mocca/net/PhysicalConnection.h"

using namespace mocca;
using namespace mocca::net;

class PhysicalConnectionTest : public ::testing::Test {
protected:
    PhysicalConnectionTest() {
        sendQueue = std::make_shared<LoopbackPhysicalConnection::LoopbackMessageQueue>();
        receiveQueue = std::make_shared<LoopbackPhysicalConnection::LoopbackMessageQueue>();
        outSignalQueue = std::make_shared<LoopbackPhysicalConnection::LoopbackSignalQueue>();
        inSignalQueue = std::make_shared<LoopbackPhysicalConnection::LoopbackSignalQueue>();
    }

    virtual ~PhysicalConnectionTest() {
        // You can do clean-up work that doesn't throw exceptions here.
    }

    std::shared_ptr<LoopbackPhysicalConnection::LoopbackMessageQueue> sendQueue;
    std::shared_ptr<LoopbackPhysicalConnection::LoopbackMessageQueue> receiveQueue;
    std::shared_ptr<LoopbackPhysicalConnection::LoopbackSignalQueue> outSignalQueue;
    std::shared_ptr<LoopbackPhysicalConnection::LoopbackSignalQueue> inSignalQueue;
};

TEST_F(PhysicalConnectionTest, ReceiveExactly_SufficientData) {
    LoopbackPhysicalConnection connection(sendQueue, receiveQueue, outSignalQueue, inSignalQueue);
    for (unsigned char c = 'a'; c <= 'e'; ++c) {
        receiveQueue->enqueue(c);
    }
    auto result = receiveExactly(connection, 3);
    ASSERT_EQ(3, result.size());
    ASSERT_EQ('a', result.data()[0]);
    ASSERT_EQ('b', result.data()[1]);
    ASSERT_EQ('c', result.data()[2]);
}

TEST_F(PhysicalConnectionTest, ReceiveExactly_InsufficientData) {
    LoopbackPhysicalConnection connection(sendQueue, receiveQueue, outSignalQueue, inSignalQueue);
    for (unsigned char c = 'a'; c <= 'e'; ++c) {
        receiveQueue->enqueue(c);
    }
    auto result = receiveExactly(connection, 7, std::chrono::milliseconds(1));
    ASSERT_EQ(0, result.size());
    // fixme: if no data is returned, the queue should not be modified
    // result = receiveExactly(connection, 5, std::chrono::milliseconds(1));
    // ASSERT_EQ(5, result.size());
    // ASSERT_EQ('a', result.data()[0]);
}

TEST_F(PhysicalConnectionTest, ReceiveExactly_NoData) {
    LoopbackPhysicalConnection connection(sendQueue, receiveQueue, outSignalQueue, inSignalQueue);
    auto result = receiveExactly(connection, 1, std::chrono::milliseconds(1));
    ASSERT_EQ(0, result.size());
}

TEST_F(PhysicalConnectionTest, ReceiveUntil_OneCharDelimAtEnd) {
    LoopbackPhysicalConnection connection(sendQueue, receiveQueue, outSignalQueue, inSignalQueue);
    for (unsigned char c = 'a'; c <= 'e'; ++c) {
        receiveQueue->enqueue(c);
    }
    auto result = receiveUntil(connection, "e", std::chrono::milliseconds(1));
    ASSERT_EQ(5, result.size());
    ASSERT_EQ('a', result.data()[0]);
    ASSERT_EQ('e', result.data()[4]);
}

TEST_F(PhysicalConnectionTest, ReceiveUntil_TwoCharDelimAtEnd) {
    LoopbackPhysicalConnection connection(sendQueue, receiveQueue, outSignalQueue, inSignalQueue);
    for (unsigned char c = 'a'; c <= 'e'; ++c) {
        receiveQueue->enqueue(c);
    }
    auto result = receiveUntil(connection, "de", std::chrono::milliseconds(1));
    ASSERT_EQ(5, result.size());
    ASSERT_EQ('a', result.data()[0]);
    ASSERT_EQ('e', result.data()[4]);
}

TEST_F(PhysicalConnectionTest, ReceiveUntil_TwoCharDelimInMiddle) {
    LoopbackPhysicalConnection connection(sendQueue, receiveQueue, outSignalQueue, inSignalQueue);
    for (unsigned char c = 'a'; c <= 'e'; ++c) {
        receiveQueue->enqueue(c);
    }
    auto result = receiveUntil(connection, "bc", std::chrono::milliseconds(1));
    ASSERT_EQ(3, result.size());
    ASSERT_EQ('a', result.data()[0]);
    ASSERT_EQ('c', result.data()[2]);
}

TEST_F(PhysicalConnectionTest, ReceiveUntil_DelimEqualsChunk) {
    LoopbackPhysicalConnection connection(sendQueue, receiveQueue, outSignalQueue, inSignalQueue);
    for (unsigned char c = 'a'; c <= 'e'; ++c) {
        receiveQueue->enqueue(c);
    }
    auto result = receiveUntil(connection, "abcde", std::chrono::milliseconds(1));
    ASSERT_EQ(5, result.size());
    ASSERT_EQ('a', result.data()[0]);
    ASSERT_EQ('e', result.data()[4]);
}

TEST_F(PhysicalConnectionTest, ReceiveUntil_DelimAtEndOfSecondChunk) {
    LoopbackPhysicalConnection connection(sendQueue, receiveQueue, outSignalQueue, inSignalQueue);
    for (unsigned char c = 'a'; c <= 'e'; ++c) {
        receiveQueue->enqueue(c);
    }
    auto result = receiveUntil(connection, "d", std::chrono::milliseconds(1), 2);
    ASSERT_EQ(4, result.size());
    ASSERT_EQ('a', result.data()[0]);
    ASSERT_EQ('d', result.data()[3]);
}

TEST_F(PhysicalConnectionTest, ReceiveUntil_DelimInMiddleOfSecondChunk) {
    LoopbackPhysicalConnection connection(sendQueue, receiveQueue, outSignalQueue, inSignalQueue);
    for (unsigned char c = 'a'; c <= 'f'; ++c) {
        receiveQueue->enqueue(c);
    }
    auto result = receiveUntil(connection, "e", std::chrono::milliseconds(1), 3);
    ASSERT_EQ(5, result.size());
    ASSERT_EQ('a', result.data()[0]);
    ASSERT_EQ('e', result.data()[4]);
}

TEST_F(PhysicalConnectionTest, ReceiveUntil_DelimBiggerThanChunk) {
    LoopbackPhysicalConnection connection(sendQueue, receiveQueue, outSignalQueue, inSignalQueue);
    for (unsigned char c = 'a'; c <= 'f'; ++c) {
        receiveQueue->enqueue(c);
    }
    auto result = receiveUntil(connection, "abcde", std::chrono::milliseconds(1), 2);
    ASSERT_EQ(5, result.size());
    ASSERT_EQ('a', result.data()[0]);
    ASSERT_EQ('e', result.data()[4]);
}

TEST_F(PhysicalConnectionTest, ReceiveUntil_DelimOnBorderOfChunk) {
    LoopbackPhysicalConnection connection(sendQueue, receiveQueue, outSignalQueue, inSignalQueue);
    for (unsigned char c = 'a'; c <= 'f'; ++c) {
        receiveQueue->enqueue(c);
    }
    auto result = receiveUntil(connection, "bc", std::chrono::milliseconds(1), 2);
    ASSERT_EQ(3, result.size());
    ASSERT_EQ('a', result.data()[0]);
    ASSERT_EQ('c', result.data()[2]);
}

TEST_F(PhysicalConnectionTest, ReceiveUntil_DelimNotFound) {
    LoopbackPhysicalConnection connection(sendQueue, receiveQueue, outSignalQueue, inSignalQueue);
    for (unsigned char c = 'a'; c <= 'f'; ++c) {
        receiveQueue->enqueue(c);
    }
    auto result = receiveUntil(connection, "x", std::chrono::milliseconds(1), 2);
    ASSERT_EQ(0, result.size());
    // fixme: if no data is returned, the queue should not be modified
    // result = receiveUntil(connection, "a", std::chrono::milliseconds(1), 2);
    // ASSERT_EQ(1, result.size());
    // ASSERT_EQ('a', result.data()[0]);
}