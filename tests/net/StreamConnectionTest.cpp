/****************************************************************
* Copyright (C) 2016 Andrey Krekhov, David McCann
*
* The content of this file may not be copied and/or distributed
* without the expressed permission of the copyright owner.
*
****************************************************************/

#include "gtest/gtest.h"

#include "mocca/net/stream/QueueConnection.h"
#include "mocca/testing/NetworkTesting.h"

using namespace mocca;
using namespace mocca::net;
using namespace mocca::testing;

class StreamConnectionTest : public ::testing::Test {
protected:
    StreamConnectionTest() {}

    virtual ~StreamConnectionTest() {}

    void createFilledStream(char from, char to) {
        receiveQueue = std::make_shared<QueueConnection::MQ>();
        for (unsigned char c = from; c <= to; ++c) {
            receiveQueue->enqueue(c);
        }
        target.reset(new QueueConnection(std::make_shared<QueueConnection::MQ>(), receiveQueue, std::make_shared<QueueConnection::SQ>(),
                                         std::make_shared<QueueConnection::SQ>(), std::make_shared<ConnectionID>()));
    }

    std::unique_ptr<QueueConnection> target;
    std::shared_ptr<QueueConnection::MQ> receiveQueue;
};

TEST_F(StreamConnectionTest, PutBack_1) {
    createFilledStream('a', 'e');
    std::vector<uint8_t> buffer(7);
    auto received = target->receive(buffer.data(), 5);
    ASSERT_EQ(5, received);
    target->putBack(buffer.data(), 5);
    receiveQueue->enqueue('f');
    receiveQueue->enqueue('g');
    received = target->receive(buffer.data(), 7);
    ASSERT_EQ(7, received);
    ASSERT_EQ('a', buffer[0]);
    ASSERT_EQ('g', buffer[6]);
}

TEST_F(StreamConnectionTest, PutBack_2) {
    createFilledStream('a', 'e');
    std::vector<uint8_t> buffer(7);
    auto received = target->receive(buffer.data(), 3);
    ASSERT_EQ(3, received);
    target->putBack(buffer.data(), 3);
    receiveQueue->enqueue('f');
    receiveQueue->enqueue('g');
    received = target->receive(buffer.data(), 7);
    ASSERT_EQ(7, received);
    ASSERT_EQ('a', buffer[0]);
    ASSERT_EQ('g', buffer[6]);
}

TEST_F(StreamConnectionTest, PutBack_3) {
    createFilledStream('a', 'e');
    std::vector<uint8_t> buffer(7);
    auto received = target->receive(buffer.data(), 3);
    ASSERT_EQ(3, received);
    target->putBack(buffer.data(), 3);
    received = target->receive(buffer.data(), 3);
    target->putBack(buffer.data(), 3);
    receiveQueue->enqueue('f');
    receiveQueue->enqueue('g');
    received = target->receive(buffer.data(), 7);
    ASSERT_EQ(7, received);
    ASSERT_EQ('a', buffer[0]);
    ASSERT_EQ('g', buffer[6]);
}

TEST_F(StreamConnectionTest, PutBack_4) {
    createFilledStream('a', 'e');
    std::vector<uint8_t> buffer(7);
    auto received = target->receive(buffer.data(), 2);
    ASSERT_EQ(2, received);
    received = target->receive(buffer.data(), 3);
    ASSERT_EQ(3, received);
    ASSERT_EQ('c', buffer[0]);
    ASSERT_EQ('e', buffer[2]);
    receiveQueue->enqueue('f');
    receiveQueue->enqueue('g');
    received = target->receive(buffer.data(), 2);
    ASSERT_EQ(2, received);
    ASSERT_EQ('f', buffer[0]);
    ASSERT_EQ('g', buffer[1]);
}