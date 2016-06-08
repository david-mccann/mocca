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