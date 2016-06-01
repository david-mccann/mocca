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

#pragma once

#include "mocca/base/Error.h"

#include <atomic>
#include <memory>
#include <mutex>
#include <thread>
#include <vector>

namespace mocca {
class AutoJoinThread {
public:
    template <class F, class... Args>
    AutoJoinThread(F&& fun, Args&&... args)
        : thread_(std::forward<F>(fun), std::forward<Args>(args)...) {}
    ~AutoJoinThread() { thread_.join(); }

private:
    std::thread thread_;
};

class ThreadInterrupt : public Error {
public:
    ThreadInterrupt(const std::string& file, int line)
        : Error("Thread has been interuppted", file, line) {}
};


class Runnable {
public:
    Runnable();
    virtual ~Runnable();

    virtual void run() = 0;
    void start();

    virtual void interrupt();
    bool isInterrupted() const;
    void join();

    void setException(const std::exception_ptr& exception);
    void rethrowException();

    std::thread::id id() const;

    static bool isCurrentInterrupted();

private:
    std::atomic<bool> interrupted_;
    std::mutex exceptionMx_;
    std::exception_ptr exception_;
    std::thread thread_;

    static std::mutex activeMx_;
    static std::vector<Runnable*> active_;
};

class RunnableGroup {
public:
    ~RunnableGroup();

    void addRunnable(std::unique_ptr<Runnable> thread);
    void removeRunnable(const std::thread::id& threadID);

    void interruptAll();
    void joinAll();

    void rethrowException();

private:
    std::vector<std::unique_ptr<Runnable>> threads_;
};
}