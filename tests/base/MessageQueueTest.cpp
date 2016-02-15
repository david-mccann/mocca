/****************************************************************
* Copyright (C) 2016 Andrey Krekhov, David McCann
*
* The content of this file may not be copied and/or distributed
* without the expressed permission of the copyright owner.
*
****************************************************************/

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