/****************************************************************
* Copyright (C) 2016 Andrey Krekhov, David McCann
*
* The content of this file may not be copied and/or distributed
* without the expressed permission of the copyright owner.
*
****************************************************************/

#include "gtest/gtest.h"

#include "mocca/base/ObjectPool.h"

using namespace mocca;

class ObjectPoolTest : public ::testing::Test {
protected:
    ObjectPoolTest() {
        // You can do set-up work for each test here.
    }

    virtual ~ObjectPoolTest() {
        // You can do clean-up work that doesn't throw exceptions here.
    }

    struct MyObject {
        void clear() { str.clear(); }
        std::string str;
    };

};

TEST_F(ObjectPoolTest, CreateAndReturnOneObject) {
    ObjectPool<MyObject> pool(20);
    for (int i = 1; i <= 50; ++i) {
        auto obj = pool.getObject();
        ASSERT_TRUE(obj->str.empty());
        obj->str = "bla";
        ASSERT_EQ("bla", obj->str);
        ASSERT_EQ(19, pool.numFreeObjects());
    }
}

TEST_F(ObjectPoolTest, CreateAndReturnMultipleObjects) {
    ObjectPool<MyObject> pool(20);
    std::vector<ObjectPool<MyObject>::ObjectPtr> objects;
    for (int i = 1; i <= 20; ++i) {
        auto obj = pool.getObject();
        ASSERT_TRUE(obj->str.empty());
        obj->str = "bla";
        objects.push_back(std::move(obj));
        ASSERT_EQ(20 - i, pool.numFreeObjects());
    }
    objects.clear();
    ASSERT_EQ(20, pool.numFreeObjects());
}

TEST_F(ObjectPoolTest, IncreasePoolSize) {
    ObjectPool<MyObject> pool(20);
    std::vector<ObjectPool<MyObject>::ObjectPtr> objects;
    for (int i = 1; i <= 50; ++i) {
        objects.push_back(pool.getObject());
    }
    objects.clear();
    ASSERT_EQ(60, pool.numFreeObjects()); // pool size is increase twice by 20
}