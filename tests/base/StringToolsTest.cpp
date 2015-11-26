#include "gtest/gtest.h"

#include "mocca/base/StringTools.h"

using namespace mocca;

class StringTest : public ::testing::Test {
protected:
    StringTest() {
        // You can do set-up work for each test here.
    }

    virtual ~StringTest() {
        // You can do clean-up work that doesn't throw exceptions here.
    }
};

TEST_F(StringTest, SplitString) {
    {
        auto result = splitString<std::string>("this is a test", ' ');
        ASSERT_EQ(4, result.size());
        ASSERT_EQ("this", result[0]);
        ASSERT_EQ("is", result[1]);
        ASSERT_EQ("a", result[2]);
        ASSERT_EQ("test", result[3]);
    }
    {
        auto result = splitString<std::string>("dot.delimiter", '.');
        ASSERT_EQ(2, result.size());
        ASSERT_EQ("dot", result[0]);
        ASSERT_EQ("delimiter", result[1]);
    }
    {
        auto result = splitString<int>("23 42 -17", ' ');
        ASSERT_EQ(3, result.size());
        ASSERT_EQ(23, result[0]);
        ASSERT_EQ(42, result[1]);
        ASSERT_EQ(-17, result[2]);
    }
    {
        auto result = splitString<float>("3.14 -2.718", ' ');
        ASSERT_EQ(2, result.size());
        ASSERT_EQ(3.14f, result[0]);
        ASSERT_EQ(-2.718f, result[1]);
    }
}

struct Printable {
    Printable(int x, int y) : x_(x), y_(y) {}
    int x_, y_;
};
std::ostream& operator<<(std::ostream& os, const Printable& obj) {
    os << "<" << obj.x_ << "," << obj.y_ << ">";
    return os;
}

TEST_F(StringTest, MakeString) {
    ASSERT_EQ("Hello World", joinString("Hello", " ", "World"));
    ASSERT_EQ("Hello42", joinString("Hello", 42));
    ASSERT_EQ("1742", joinString(17, 42));
    ASSERT_EQ("42 3.14", joinString(42, " ", 3.14f));
    ASSERT_EQ("Hello<23,42>3.14", joinString("Hello", Printable(23, 42), 3.14f));
}

TEST_F(StringTest, FormatString) {
    ASSERT_EQ("First Second Third", formatString("First %% Third", "Second"));
    ASSERT_EQ("First Second Third", formatString("First %% %%", "Second", "Third"));
    ASSERT_EQ("First Second Third", formatString("%% %% %%", "First", "Second", "Third"));
    ASSERT_EQ("First 2 Third", formatString("First %% Third", 2));
    ASSERT_EQ("First 2 3.1415", formatString("First %% %%", 2, 3.1415f));
    ASSERT_EQ("First 2 Third 1", formatString("First %% Third %%", 2, true));
    ASSERT_EQ("First <23,42> Third 3.14",
              formatString("First %% Third %%", Printable(23, 42), 3.14f));
    ASSERT_THROW(formatString("First %% (no more placeholders)", "ok", "not ok"),
                 mocca::Error);
}