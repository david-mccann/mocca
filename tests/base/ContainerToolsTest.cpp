/****************************************************************
* Copyright (C) 2016 Andrey Krekhov, David McCann
*
* The content of this file may not be copied and/or distributed
* without the expressed permission of the copyright owner.
*
****************************************************************/

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
    auto x = makeUniquePtrVec(makeUnique<std::string>("Hello"), makeUnique<std::string>("World"));
    ASSERT_EQ(2, x.size());
    ASSERT_EQ("Hello", *x[0]);
    ASSERT_EQ("World", *x[1]);
}