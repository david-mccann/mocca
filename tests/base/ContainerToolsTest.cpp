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

#include "mocca/base/ContainerTools.h"
#include "mocca/base/Memory.h"

using namespace mocca;

class ContainerToolsTest : public ::testing::Test {
protected:
    ContainerToolsTest() {
        // You can do set-up work for each test here.
    }

    virtual ~ContainerToolsTest() {
        // You can do clean-up work that doesn't throw exceptions here.
    }
};


TEST_F(ContainerToolsTest, FindMemberEqual) {
    struct TestStruct {
        int val;
        int getVal() const { return val; }
    };

    {
        std::vector<TestStruct> v{{17}, {23}, {42}};
        auto result = mocca::findMemberEqual(begin(v), end(v), &TestStruct::getVal, 23);
        ASSERT_EQ(23, result->val);
    }
    {
        std::vector<TestStruct> v{{17}, {23}, {42}};
        auto result = mocca::findMemberEqual(begin(v), end(v), &TestStruct::getVal, 4711);
        ASSERT_EQ(end(v), result);
    }
}

TEST_F(ContainerToolsTest, CollectMembers) {
    struct SomeStruct {
        std::string val1;
        int val2;
    };
    std::vector<SomeStruct> vec{{"first", 1}, {"second", 2}};
    auto res = collectMembers(begin(vec), end(vec), &SomeStruct::val1);
    ASSERT_EQ(2, res.size());
    ASSERT_EQ("first", res[0]);
    ASSERT_EQ("second", res[1]);
}

TEST_F(ContainerToolsTest, MakeUniquePtrVec) {
    auto x = makeUniquePtrVec(mocca::make_unique<std::string>("Hello"), mocca::make_unique<std::string>("World"));
    ASSERT_EQ(2, x.size());
    ASSERT_EQ("Hello", *x[0]);
    ASSERT_EQ("World", *x[1]);
}