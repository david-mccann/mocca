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

#include <numeric>
#include <future>

#include "gtest/gtest.h"

#include "mocca/base/MessageQueue.h"

using namespace mocca;

class MessageQueueTest : public ::testing::Test {
protected:
    MessageQueueTest() {
        // You can do set-up work for each test here.
    }

    virtual ~MessageQueueTest() {
        // You can do clean-up work that doesn't throw exceptions here.
    }
};


TEST_F(MessageQueueTest, EnqueueDequeue) {
    MessageQueue<std::string> target;

    ASSERT_TRUE(target.dequeue(std::chrono::milliseconds(20)).isNull());

    target.enqueue("Test");
    ASSERT_EQ("Test", target.dequeue(std::chrono::milliseconds(20)));

    ASSERT_TRUE(target.dequeue(std::chrono::milliseconds(20)).isNull());
}

TEST_F(MessageQueueTest, EnqueueDequeueParallel) {
    MessageQueue<int> target;

    const int numItems = 100;
    std::vector<int> data(numItems);
    std::iota(begin(data), end(data), 0);

    std::async(std::launch::async, [&target, data] {
        for (auto item : data) {
            target.enqueue(item);
        }
    });

    std::vector<std::future<int>> futures;
    for (int i = 0; i < numItems; i++) {
        futures.push_back(std::async(std::launch::async, [&target]() { return target.dequeue(std::chrono::milliseconds(2)).get(); }));
    }

    std::vector<int> result;
    for (auto& future : futures) {
        result.push_back(future.get());
    }

    std::sort(begin(result), end(result));
    ASSERT_EQ(data, result);
    ASSERT_TRUE(target.isEmpty());
}

TEST_F(MessageQueueTest, DequeueFiltered) {
    {
        MessageQueue<std::string> target;
        target.enqueue("Test");
        target.enqueue("Test2");
        auto s = target.dequeueFiltered([](const std::string& str) { return str == "Test2"; }, std::chrono::milliseconds(20));
        ASSERT_EQ(1, target.size());
    }
    {
        MessageQueue<std::string> target;
        target.enqueue("Test");
        auto s = target.dequeueFiltered([](const std::string& str) { return str == "Test2"; }, std::chrono::milliseconds(20));
        ASSERT_TRUE(s.isNull());
        ASSERT_EQ(1, target.size());
    }
    {
        MessageQueue<std::string> target;
        target.enqueue("Test");
        std::thread t([&target] {
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
            target.enqueue("Test2");
        });
        auto s = target.dequeueFiltered([](const std::string& str) { return str == "Test2"; }, std::chrono::milliseconds(20));
        ASSERT_EQ("Test2", s);
        ASSERT_EQ(1, target.size());
        t.join();
    }
    {
        MessageQueue<std::string> target;
        target.enqueue("Test");
        std::thread t([&target] {
            std::this_thread::sleep_for(std::chrono::milliseconds(40));
            target.enqueue("Test2");
        });
        auto s = target.dequeueFiltered([](const std::string& str) { return str == "Test2"; }, std::chrono::milliseconds(20));
        ASSERT_TRUE(s.isNull());
        t.join();
    }
}