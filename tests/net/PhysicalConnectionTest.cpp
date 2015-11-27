#include "gtest/gtest.h"

#include "mocca/base/ByteArray.h"
#include "mocca/base/MessageQueue.h"
#include "mocca/net/LoopbackConnection_tmp.h"
#include "mocca/net/PhysicalConnection.h"

using namespace mocca;
using namespace mocca::net;

class PhysicalConnectionTest : public ::testing::Test {
protected:
    PhysicalConnectionTest() {
        sendQueue = std::make_shared<LoopbackConnection_tmp::LoopbackMessageQueue>();
        receiveQueue = std::make_shared<LoopbackConnection_tmp::LoopbackMessageQueue>();
        outSignalQueue = std::make_shared<LoopbackConnection_tmp::LoopbackSignalQueue>();
        inSignalQueue = std::make_shared<LoopbackConnection_tmp::LoopbackSignalQueue>();
    }

    virtual ~PhysicalConnectionTest() {
        // You can do clean-up work that doesn't throw exceptions here.
    }

    std::shared_ptr<LoopbackConnection_tmp::LoopbackMessageQueue> sendQueue;
    std::shared_ptr<LoopbackConnection_tmp::LoopbackMessageQueue> receiveQueue;
    std::shared_ptr<LoopbackConnection_tmp::LoopbackSignalQueue> outSignalQueue;
    std::shared_ptr<LoopbackConnection_tmp::LoopbackSignalQueue> inSignalQueue;
};

TEST_F(PhysicalConnectionTest, ReceiveExactly_SufficientData) {
    LoopbackConnection_tmp connection(sendQueue, receiveQueue, outSignalQueue, inSignalQueue);
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
    LoopbackConnection_tmp connection(sendQueue, receiveQueue, outSignalQueue, inSignalQueue);
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
    LoopbackConnection_tmp connection(sendQueue, receiveQueue, outSignalQueue, inSignalQueue);
    auto result = receiveExactly(connection, 1, std::chrono::milliseconds(1));
    ASSERT_EQ(0, result.size());
}

TEST_F(PhysicalConnectionTest, ReceiveUntil_OneCharDelimAtEnd) {
    LoopbackConnection_tmp connection(sendQueue, receiveQueue, outSignalQueue, inSignalQueue);
    for (unsigned char c = 'a'; c <= 'e'; ++c) {
        receiveQueue->enqueue(c);
    }
    auto result = receiveUntil(connection, "e", std::chrono::milliseconds(1));
    ASSERT_EQ(5, result.size());
    ASSERT_EQ('a', result.data()[0]);
    ASSERT_EQ('e', result.data()[4]);
}

TEST_F(PhysicalConnectionTest, ReceiveUntil_TwoCharDelimAtEnd) {
    LoopbackConnection_tmp connection(sendQueue, receiveQueue, outSignalQueue, inSignalQueue);
    for (unsigned char c = 'a'; c <= 'e'; ++c) {
        receiveQueue->enqueue(c);
    }
    auto result = receiveUntil(connection, "de", std::chrono::milliseconds(1));
    ASSERT_EQ(5, result.size());
    ASSERT_EQ('a', result.data()[0]);
    ASSERT_EQ('e', result.data()[4]);
}

TEST_F(PhysicalConnectionTest, ReceiveUntil_TwoCharDelimInMiddle) {
    LoopbackConnection_tmp connection(sendQueue, receiveQueue, outSignalQueue, inSignalQueue);
    for (unsigned char c = 'a'; c <= 'e'; ++c) {
        receiveQueue->enqueue(c);
    }
    auto result = receiveUntil(connection, "bc", std::chrono::milliseconds(1));
    ASSERT_EQ(3, result.size());
    ASSERT_EQ('a', result.data()[0]);
    ASSERT_EQ('c', result.data()[2]);
}

TEST_F(PhysicalConnectionTest, ReceiveUntil_DelimEqualsChunk) {
    LoopbackConnection_tmp connection(sendQueue, receiveQueue, outSignalQueue, inSignalQueue);
    for (unsigned char c = 'a'; c <= 'e'; ++c) {
        receiveQueue->enqueue(c);
    }
    auto result = receiveUntil(connection, "abcde", std::chrono::milliseconds(1));
    ASSERT_EQ(5, result.size());
    ASSERT_EQ('a', result.data()[0]);
    ASSERT_EQ('e', result.data()[4]);
}

TEST_F(PhysicalConnectionTest, ReceiveUntil_DelimAtEndOfSecondChunk) {
    LoopbackConnection_tmp connection(sendQueue, receiveQueue, outSignalQueue, inSignalQueue);
    for (unsigned char c = 'a'; c <= 'e'; ++c) {
        receiveQueue->enqueue(c);
    }
    auto result = receiveUntil(connection, "d", std::chrono::milliseconds(1), 2);
    ASSERT_EQ(4, result.size());
    ASSERT_EQ('a', result.data()[0]);
    ASSERT_EQ('d', result.data()[3]);
}

TEST_F(PhysicalConnectionTest, ReceiveUntil_DelimInMiddleOfSecondChunk) {
    LoopbackConnection_tmp connection(sendQueue, receiveQueue, outSignalQueue, inSignalQueue);
    for (unsigned char c = 'a'; c <= 'f'; ++c) {
        receiveQueue->enqueue(c);
    }
    auto result = receiveUntil(connection, "e", std::chrono::milliseconds(1), 3);
    ASSERT_EQ(5, result.size());
    ASSERT_EQ('a', result.data()[0]);
    ASSERT_EQ('e', result.data()[4]);
}

TEST_F(PhysicalConnectionTest, ReceiveUntil_DelimBiggerThanChunk) {
    LoopbackConnection_tmp connection(sendQueue, receiveQueue, outSignalQueue, inSignalQueue);
    for (unsigned char c = 'a'; c <= 'f'; ++c) {
        receiveQueue->enqueue(c);
    }
    auto result = receiveUntil(connection, "abcde", std::chrono::milliseconds(1), 2);
    ASSERT_EQ(5, result.size());
    ASSERT_EQ('a', result.data()[0]);
    ASSERT_EQ('e', result.data()[4]);
}

TEST_F(PhysicalConnectionTest, ReceiveUntil_DelimOnBorderOfChunk) {
    LoopbackConnection_tmp connection(sendQueue, receiveQueue, outSignalQueue, inSignalQueue);
    for (unsigned char c = 'a'; c <= 'f'; ++c) {
        receiveQueue->enqueue(c);
    }
    auto result = receiveUntil(connection, "bc", std::chrono::milliseconds(1), 2);
    ASSERT_EQ(3, result.size());
    ASSERT_EQ('a', result.data()[0]);
    ASSERT_EQ('c', result.data()[2]);
}

TEST_F(PhysicalConnectionTest, ReceiveUntil_DelimNotFound) {
    LoopbackConnection_tmp connection(sendQueue, receiveQueue, outSignalQueue, inSignalQueue);
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