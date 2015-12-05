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

TEST_F(ThreadTest, AutoJoinThread) {
    std::vector<int> x;
    {
        AutoJoinThread t1([&x] { for (int i = 0; i <= 100000; ++i) x.push_back(i); });
    }
    {
        AutoJoinThread t2([&x] { for (int i = 0; i <= 100000; ++i) x.pop_back(); });
    }
    ASSERT_TRUE(x.empty());
}

TEST_F(ThreadTest, Runnable_Interrupt) {
    struct TestRunnable : public Runnable {
        TestRunnable(bool& done) : done_(done) {}
        void run() override {
            while (!isInterrupted()) {}
            done_ = true;
        }
        bool& done_;
    };

    bool funDone = false;
    {
        TestRunnable target(funDone);
        target.start();
        target.join(); // interrupt and join
    }
    ASSERT_TRUE(funDone);
}

TEST_F(ThreadTest, Runnable) {
    struct TestRunnable : public Runnable {
        TestRunnable(bool& done) : done_(done) {}
        void run() override {
            done_ = true;
        }
        bool& done_;
    };

    bool done = false;
    {
        TestRunnable target(done);
        target.start();
        target.join();
    }
    ASSERT_TRUE(done);
}