#include "gtest/gtest.h"

#include "mocca/base/ByteArray.h"
#include "mocca/base/MessageQueue.h"
#include "mocca/net/framing/FramingUtils.h"
#include "mocca/net/stream/MessageQueueStream.h"

using namespace mocca;
using namespace mocca::net;

class FramingUtilsTest : public ::testing::Test {
protected:
    FramingUtilsTest() {}

    virtual ~FramingUtilsTest() {
        // You can do clean-up work that doesn't throw exceptions here.
    }

    std::unique_ptr<MessageQueueStream> createFilledStream(char from, char to) {
        auto receiveQueue = std::make_shared<MessageQueueStream::LoopbackMessageQueue>();
        for (unsigned char c = from; c <= to; ++c) {
            receiveQueue->enqueue(c);
        }
        std::unique_ptr<MessageQueueStream> stream(new MessageQueueStream(
            std::make_shared<MessageQueueStream::LoopbackMessageQueue>(), receiveQueue,
            std::make_shared<MessageQueueStream::LoopbackSignalQueue>(), std::make_shared<MessageQueueStream::LoopbackSignalQueue>()));
        return stream;
    }
};

TEST_F(FramingUtilsTest, ReceiveExactly_SufficientData) {
    auto stream = createFilledStream('a', 'e');
    auto result = readExactly(*stream, 3);
    ASSERT_EQ(3, result.size());
    ASSERT_EQ('a', result.data()[0]);
    ASSERT_EQ('b', result.data()[1]);
    ASSERT_EQ('c', result.data()[2]);
}

TEST_F(FramingUtilsTest, ReceiveExactly_InsufficientData) {
    auto stream = createFilledStream('a', 'e');
    auto result = readExactly(*stream, 7, std::chrono::milliseconds(1));
    ASSERT_EQ(0, result.size());
    // fixme: if no data is returned, the queue should not be modified
    // result = readExactly(*stream, 5, std::chrono::milliseconds(1));
    // ASSERT_EQ(5, result.size());
    // ASSERT_EQ('a', result.data()[0]);
}

TEST_F(FramingUtilsTest, ReceiveExactly_NoData) {
    auto stream = createFilledStream('a', 'a' - 1);
    auto result = readExactly(*stream, 1, std::chrono::milliseconds(1));
    ASSERT_EQ(0, result.size());
}

TEST_F(FramingUtilsTest, ReceiveUntil_OneCharDelimAtEnd) {
    auto stream = createFilledStream('a', 'e');
    auto result = readUntil(*stream, "e", std::chrono::milliseconds(1));
    ASSERT_EQ(5, result.size());
    ASSERT_EQ('a', result.data()[0]);
    ASSERT_EQ('e', result.data()[4]);
}

TEST_F(FramingUtilsTest, ReceiveUntil_TwoCharDelimAtEnd) {
    auto stream = createFilledStream('a', 'e');
    auto result = readUntil(*stream, "de", std::chrono::milliseconds(1));
    ASSERT_EQ(5, result.size());
    ASSERT_EQ('a', result.data()[0]);
    ASSERT_EQ('e', result.data()[4]);
}

TEST_F(FramingUtilsTest, ReceiveUntil_TwoCharDelimInMiddle) {
    auto stream = createFilledStream('a', 'e');
    auto result = readUntil(*stream, "bc", std::chrono::milliseconds(1));
    ASSERT_EQ(3, result.size());
    ASSERT_EQ('a', result.data()[0]);
    ASSERT_EQ('c', result.data()[2]);
}

TEST_F(FramingUtilsTest, ReceiveUntil_DelimEqualsChunk) {
    auto stream = createFilledStream('a', 'e');
    auto result = readUntil(*stream, "abcde", std::chrono::milliseconds(1));
    ASSERT_EQ(5, result.size());
    ASSERT_EQ('a', result.data()[0]);
    ASSERT_EQ('e', result.data()[4]);
}

TEST_F(FramingUtilsTest, ReceiveUntil_DelimAtEndOfSecondChunk) {
    auto stream = createFilledStream('a', 'e');
    auto result = readUntil(*stream, "d", std::chrono::milliseconds(1), 2);
    ASSERT_EQ(4, result.size());
    ASSERT_EQ('a', result.data()[0]);
    ASSERT_EQ('d', result.data()[3]);
}

TEST_F(FramingUtilsTest, ReceiveUntil_DelimInMiddleOfSecondChunk) {
    auto stream = createFilledStream('a', 'f');
    auto result = readUntil(*stream, "e", std::chrono::milliseconds(1), 3);
    ASSERT_EQ(5, result.size());
    ASSERT_EQ('a', result.data()[0]);
    ASSERT_EQ('e', result.data()[4]);
}

TEST_F(FramingUtilsTest, ReceiveUntil_DelimBiggerThanChunk) {
    auto stream = createFilledStream('a', 'f');
    auto result = readUntil(*stream, "abcde", std::chrono::milliseconds(1), 2);
    ASSERT_EQ(5, result.size());
    ASSERT_EQ('a', result.data()[0]);
    ASSERT_EQ('e', result.data()[4]);
}

TEST_F(FramingUtilsTest, ReceiveUntil_DelimOnBorderOfChunk) {
    auto stream = createFilledStream('a', 'f');
    auto result = readUntil(*stream, "bc", std::chrono::milliseconds(1), 2);
    ASSERT_EQ(3, result.size());
    ASSERT_EQ('a', result.data()[0]);
    ASSERT_EQ('c', result.data()[2]);
}

TEST_F(FramingUtilsTest, ReceiveUntil_DelimNotFound) {
    auto stream = createFilledStream('a', 'f');
    auto result = readUntil(*stream, "x", std::chrono::milliseconds(1), 2);
    ASSERT_EQ(0, result.size());
    // fixme: if no data is returned, the queue should not be modified
    // result = readUntil(*stream, "a", std::chrono::milliseconds(1), 2);
    // ASSERT_EQ(1, result.size());
    // ASSERT_EQ('a', result.data()[0]);
}