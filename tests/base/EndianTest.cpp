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

#include "mocca/base/Endian.h"

using namespace mocca;

class EndianTest : public ::testing::Test {
protected:
    EndianTest() {
        // You can do set-up work for each test here.
    }

    virtual ~EndianTest() {
        // You can do clean-up work that doesn't throw exceptions here.
    }
};

TEST_F(EndianTest, Swap16) {
    uint16_t val = 0x1234;
    ASSERT_EQ(0x3412, swap_uint16(val));
    ASSERT_EQ(val, swap_uint16(swap_uint16(val)));
}

TEST_F(EndianTest, Swap32) {
    uint32_t val = 0x12345678;
    ASSERT_EQ(0x78563412, swap_uint32(val));
    ASSERT_EQ(val, swap_uint32(swap_uint32(val)));
}

TEST_F(EndianTest, Swap64) {
    uint64_t val = 0x123456789ABCDEF1;
    ASSERT_EQ(0xF1DEBC9A78563412, swap_uint64(val));
    ASSERT_EQ(val, swap_uint64(swap_uint64(val)));
}