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