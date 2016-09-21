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

#include "mocca/base/Explicit.h"

using namespace mocca;

class ExplicitTest : public ::testing::Test {
protected:
    ExplicitTest() {
        // You can do set-up work for each test here.
    }

    virtual ~ExplicitTest() {
        // You can do clean-up work that doesn't throw exceptions here.
    }
};

TEST_F(ExplicitTest, Ctor) {
    struct Phantom;
    using PhantomID = Explicit<int, Phantom>;
    {
        PhantomID id{ 17 };
        ASSERT_EQ(17, id);
    }
    {
        PhantomID id{ 17 };
        PhantomID id2{ id };
        ASSERT_EQ(17, id2);
    }
}

TEST_F(ExplicitTest, Assignment) {
    struct Phantom;
    using PhantomID = Explicit<int, Phantom>;
    {
        PhantomID id{ 17 };
        id = 23;
        ASSERT_EQ(23, id);
    }
    {
        PhantomID id{ 17 };
        PhantomID id2{ 23 };
        id = id2;
        ASSERT_EQ(23, id);
    }
}

TEST_F(ExplicitTest, StringComparison) {
    struct Phantom;
    using PhantomID = Explicit<std::string, Phantom>;
    PhantomID id{ "id" };
    ASSERT_EQ("id", id);
    ASSERT_EQ(id, "id");
}