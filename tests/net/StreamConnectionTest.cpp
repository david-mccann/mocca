/****************************************************************
* Copyright (C) 2016 Andrey Krekhov, David McCann
*
* The content of this file may not be copied and/or distributed
* without the expressed permission of the copyright owner.
*
****************************************************************/

#include "gtest/gtest.h"

#include "mocca/base/ByteArray.h"
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
    auto received = target->receive(5);
    ASSERT_EQ(5, received.size());
    target->putBack(received);
    receiveQueue->enqueue('f');
    receiveQueue->enqueue('g');
    received = target->receive(7);
    ASSERT_EQ(7, received.size());
    ASSERT_EQ('a', received[0]);
    ASSERT_EQ('g', received[6]);
}

TEST_F(StreamConnectionTest, PutBack_2) {
    createFilledStream('a', 'e');
    auto received = target->receive(3);
    ASSERT_EQ(3, received.size());
    target->putBack(received);
    receiveQueue->enqueue('f');
    receiveQueue->enqueue('g');
    received = target->receive(7);
    ASSERT_EQ(7, received.size());
    ASSERT_EQ('a', received[0]);
    ASSERT_EQ('g', received[6]);
}

TEST_F(StreamConnectionTest, PutBack_3) {
    createFilledStream('a', 'e');
    auto received = target->receive(3);
    ASSERT_EQ(3, received.size());
    target->putBack(received);
    received = target->receive(3);
    target->putBack(received);
    receiveQueue->enqueue('f');
    receiveQueue->enqueue('g');
    received = target->receive(7);
    ASSERT_EQ(7, received.size());
    ASSERT_EQ('a', received[0]);
    ASSERT_EQ('g', received[6]);
}

TEST_F(StreamConnectionTest, PutBack_4) {
    createFilledStream('a', 'e');
    auto received = target->receive(2);
    ASSERT_EQ(2, received.size());
    received = target->receive(3);
    ASSERT_EQ(3, received.size());
    ASSERT_EQ('c', received[0]);
    ASSERT_EQ('e', received[2]);
    receiveQueue->enqueue('f');
    receiveQueue->enqueue('g');
    received = target->receive(2);
    ASSERT_EQ(2, received.size());
    ASSERT_EQ('f', received[0]);
    ASSERT_EQ('g', received[1]);
}