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

#include "mocca/base/Nullable.h"

using namespace mocca;

class NullableTest : public ::testing::Test {
protected:
    NullableTest() {
        // You can do set-up work for each test here.
    }

    virtual ~NullableTest() {
        // You can do clean-up work that doesn't throw exceptions here.
    }
};

TEST_F(NullableTest, DefaultCtor) {
    Nullable<std::string> target;
    ASSERT_TRUE(target.isNull());
}

TEST_F(NullableTest, CopyCtor) {
    {
        Nullable<std::string> first("Hello");
        Nullable<std::string> second(first);

        ASSERT_EQ("Hello", first);
        ASSERT_EQ("Hello", second);
    }
    {
        Nullable<std::string> first;
        Nullable<std::string> second(first);

        ASSERT_TRUE(first.isNull());
        ASSERT_TRUE(second.isNull());
    }
}

TEST_F(NullableTest, MoveCtor) {
    {
        Nullable<std::string> first("Hello");
        Nullable<std::string> second(std::move(first));

        ASSERT_TRUE(first.isNull());
        ASSERT_EQ("Hello", second);
    }
    {
        Nullable<std::string> first;
        Nullable<std::string> second(std::move(first));

        ASSERT_TRUE(first.isNull());
        ASSERT_TRUE(second.isNull());
    }
}

TEST_F(NullableTest, Assignment) {
    {
        Nullable<int> target;
        target = 17;
        ASSERT_EQ(17, target);
    }
    {
        Nullable<std::string> target;
        target = "Hello";
        ASSERT_EQ("Hello", target);
    }
}

TEST_F(NullableTest, ValidAccess) {
    Nullable<std::string> first("Hello");
    ASSERT_EQ("Hello", first);
    ASSERT_EQ(5, static_cast<std::string>(first.get()).size());
}

TEST_F(NullableTest, InvalidAccess) {
    Nullable<std::string> first;
    ASSERT_THROW(static_cast<std::string>(first.get()).size(), Error);
}

TEST_F(NullableTest, ValidCast) {
    Nullable<std::string> first("Hello");
    std::string str = first;
    ASSERT_EQ("Hello", str);
}

TEST_F(NullableTest, InvalidCast) {
    Nullable<std::string> first;
    ASSERT_THROW(std::string str = first, Error);
}

TEST_F(NullableTest, Equality) {
    {
        Nullable<std::string> lhs;
        Nullable<std::string> rhs;
        ASSERT_TRUE(lhs == rhs);
    }
    {
        Nullable<std::string> lhs("Hello");
        Nullable<std::string> rhs;
        ASSERT_FALSE(lhs == rhs);
    }
    {
        Nullable<std::string> lhs;
        Nullable<std::string> rhs("Hello");
        ASSERT_FALSE(lhs == rhs);
    }
    {
        Nullable<std::string> lhs("Hello");
        Nullable<std::string> rhs("Hello");
        ASSERT_TRUE(lhs == rhs);
    }
    {
        Nullable<std::string> lhs("Hello");
        ASSERT_TRUE(lhs == "Hello");
    }
    {
        Nullable<std::string> lhs;
        ASSERT_FALSE(lhs == "Hello");
    }
}

TEST_F(NullableTest, GetOr) {
    {
        Nullable<std::string> target("Hello");
        ASSERT_EQ("Hello", target.getOr("default"));
    }
    {
        Nullable<std::string> target;
        ASSERT_EQ("default", target.getOr("default"));
    }
    {
        Nullable<std::string> target;
        target = "Hello";
        ASSERT_EQ("Hello", target.getOr("default"));
    }
}

TEST_F(NullableTest, Release) {
    Nullable<std::string> target("Hello");
    auto val = target.release();
    ASSERT_TRUE(target.isNull());
    ASSERT_EQ("Hello", val);
}