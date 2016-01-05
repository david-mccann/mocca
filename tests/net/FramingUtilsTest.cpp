#include "gtest/gtest.h"

#include "mocca/net/framing/FramingUtils.h"
#include "mocca/net/stream/QueueConnection.h"

using namespace mocca;
using namespace mocca::net;

class FramingUtilsTest : public ::testing::Test {
protected:
    FramingUtilsTest() {}

    virtual ~FramingUtilsTest() {
        // You can do clean-up work that doesn't throw exceptions here.
    }

    std::unique_ptr<QueueConnection> createFilledStream(char from, char to) {
        auto receiveQueue = std::make_shared<QueueConnection::MQ>();
        for (unsigned char c = from; c <= to; ++c) {
            receiveQueue->enqueue(c);
        }
        std::unique_ptr<QueueConnection> stream(
            new QueueConnection(std::make_shared<QueueConnection::MQ>(), receiveQueue, std::make_shared<QueueConnection::SQ>(),
                                std::make_shared<QueueConnection::SQ>(), std::make_shared<ConnectionID>()));
        return stream;
    }

    ByteArray readExactlyHelper(IStreamConnection& stream, uint32_t size) {
        ByteArray result;
        readExactly(stream, result, size, std::chrono::milliseconds(1));
        return result;
    }

    ByteArray readUntilHelper(IStreamConnection& stream, const std::string& delim, uint32_t chunkSize = 2) {
        ByteArray result;
        readUntil(stream, result, delim, std::chrono::milliseconds(1), chunkSize);
        return result;
    }
};

TEST_F(FramingUtilsTest, ReceiveExactly_SufficientData) {
    auto stream = createFilledStream('a', 'e');
    ByteArray result;
    auto status = readExactly(*stream, result, 3);
    ASSERT_EQ(ReadStatus::Complete, status);
    ASSERT_EQ(3, result.size());
    ASSERT_EQ('a', result.data()[0]);
    ASSERT_EQ('b', result.data()[1]);
    ASSERT_EQ('c', result.data()[2]);
}

TEST_F(FramingUtilsTest, ReceiveExactly_InsufficientData) {
    auto stream = createFilledStream('a', 'e');
    ByteArray result;
    auto status = readExactly(*stream, result, 7);
    ASSERT_EQ(ReadStatus::Incomplete, status);
    ASSERT_EQ(5, result.size());
}

TEST_F(FramingUtilsTest, ReceiveUntil_OneCharDelimAtEnd) {
    auto stream = createFilledStream('a', 'e');
    auto result = readUntilHelper(*stream, "e");
    ASSERT_EQ(5, result.size());
    ASSERT_EQ('a', result.data()[0]);
    ASSERT_EQ('e', result.data()[4]);
}

TEST_F(FramingUtilsTest, ReceiveUntil_TwoCharDelimAtEnd) {
    auto stream = createFilledStream('a', 'e');
    auto result = readUntilHelper(*stream, "de");
    ASSERT_EQ(5, result.size());
    ASSERT_EQ('a', result.data()[0]);
    ASSERT_EQ('e', result.data()[4]);
}

TEST_F(FramingUtilsTest, ReceiveUntil_TwoCharDelimInMiddle) {
    auto stream = createFilledStream('a', 'e');
    auto result = readUntilHelper(*stream, "bc");
    ASSERT_EQ(3, result.size());
    ASSERT_EQ('a', result.data()[0]);
    ASSERT_EQ('c', result.data()[2]);
}

TEST_F(FramingUtilsTest, ReceiveUntil_DelimEqualsChunk) {
    auto stream = createFilledStream('a', 'e');
    auto result = readUntilHelper(*stream, "abcde");
    ASSERT_EQ(5, result.size());
    ASSERT_EQ('a', result.data()[0]);
    ASSERT_EQ('e', result.data()[4]);
}

TEST_F(FramingUtilsTest, ReceiveUntil_DelimAtEndOfSecondChunk) {
    auto stream = createFilledStream('a', 'e');
    auto result = readUntilHelper(*stream, "d");
    ASSERT_EQ(4, result.size());
    ASSERT_EQ('a', result.data()[0]);
    ASSERT_EQ('d', result.data()[3]);
}

TEST_F(FramingUtilsTest, ReceiveUntil_DelimInMiddleOfSecondChunk) {
    auto stream = createFilledStream('a', 'f');
    auto result = readUntilHelper(*stream, "e", 3);
    ASSERT_EQ(5, result.size());
    ASSERT_EQ('a', result.data()[0]);
    ASSERT_EQ('e', result.data()[4]);
}

TEST_F(FramingUtilsTest, ReceiveUntil_DelimBiggerThanChunk) {
    auto stream = createFilledStream('a', 'f');
    auto result = readUntilHelper(*stream, "abcde");
    ASSERT_EQ(5, result.size());
    ASSERT_EQ('a', result.data()[0]);
    ASSERT_EQ('e', result.data()[4]);
}

TEST_F(FramingUtilsTest, ReceiveUntil_DelimOnBorderOfChunk) {
    auto stream = createFilledStream('a', 'f');
    auto result = readUntilHelper(*stream, "bc");
    ASSERT_EQ(3, result.size());
    ASSERT_EQ('a', result.data()[0]);
    ASSERT_EQ('c', result.data()[2]);
}

TEST_F(FramingUtilsTest, ReceiveUntil_DelimNotFound) {
    auto stream = createFilledStream('a', 'e');
    ByteArray result;
    ReadStatus status = readUntil(*stream, result, "x");
    ASSERT_EQ(ReadStatus::Incomplete, status);
    ASSERT_EQ(5, result.size());
}