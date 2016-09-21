/*The MIT License(MIT)

Copyright(c) 2016 David McCann

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files(the
"Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and / or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to
the following conditions :

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.*/

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

    std::vector<uint8_t> readExactlyHelper(IStreamConnection& stream, uint32_t size) {
        std::vector<uint8_t> result;
        readExactly(stream, result, size);
        return result;
    }

    std::vector<uint8_t> readUntilHelper(IStreamConnection& stream, const std::string& delim) {
        std::vector<uint8_t> result;
        readUntil(stream, result, delim);
        return result;
    }
};

TEST_F(FramingUtilsTest, ReceiveExactly_SufficientData) {
    auto stream = createFilledStream('a', 'e');
    std::vector<uint8_t> result;
    readExactly(*stream, result, 3);
    ASSERT_EQ(3, result.size());
    ASSERT_EQ('a', result.data()[0]);
    ASSERT_EQ('b', result.data()[1]);
    ASSERT_EQ('c', result.data()[2]);
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
    auto result = readUntilHelper(*stream, "e");
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