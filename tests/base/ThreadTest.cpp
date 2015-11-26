#include "gtest/gtest.h"

#include "mocca/base/Thread.h"

#include <atomic>

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
        Thread t1(std::thread([&x] { for (int i = 0; i <= 100000; ++i) x.push_back(i); }));
    }
    {
        Thread t2(std::thread([&x] { for (int i = 0; i <= 100000; ++i) x.pop_back(); }));
    }
    ASSERT_TRUE(x.empty());
}

TEST_F(ThreadTest, TerminateAndJoin) {
    std::vector<int> x;
    {
        std::atomic<bool> terminate(false);
        Thread t(std::thread([&]() {
            int i = 0;
            while (!terminate) {
                x.push_back(i++);
            }
        }));
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
        terminate = true;
    }
    ASSERT_FALSE(x.empty());    
}