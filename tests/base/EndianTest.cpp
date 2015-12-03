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