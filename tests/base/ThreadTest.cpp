#include "gtest/gtest.h"

#include "mocca/base/Thread.h"

using namespace mocca;

class ThreadTest : public ::testing::Test {
protected:
    ThreadTest() {
        // You can do set-up work for each test here.
    }

    virtual ~ThreadTest() {
        // You can do clean-up work that doesn't throw exceptions here.
    }
};

TEST_F(ThreadTest, JoinOnDestruction) {
    std::vector<int> x;
    {
        Thread t1([&x] { for (int i = 0; i <= 100000; ++i) x.push_back(i); });
    }
    {
        Thread t2([&x] { for (int i = 0; i <= 100000; ++i) x.pop_back(); });
    }
    ASSERT_TRUE(x.empty());
}

TEST_F(ThreadTest, Interrupt) {
    struct TestStruct {
        TestStruct(bool& done) : done_(done), t_(&TestStruct::fun, this) {}
        ~TestStruct() { t_.interrupt(); }
        void fun() {
            while (!t_.isInterrupted()) {}
            done_ = true;
        }
        bool& done_;
        Thread t_;
    };

    bool funDone = false;
    {
        TestStruct s(funDone);
    }
    ASSERT_TRUE(funDone);
}