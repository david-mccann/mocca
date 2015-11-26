#pragma once

#include <thread>

namespace mocca {
struct Thread {
    Thread(std::thread t);
    Thread(Thread&& other);
    Thread& operator=(Thread&& other);
    ~Thread();

private:
    std::thread thread_;
};
}