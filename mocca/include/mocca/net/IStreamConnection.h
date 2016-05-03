/****************************************************************
* Copyright (C) 2016 Andrey Krekhov, David McCann
*
* The content of this file may not be copied and/or distributed
* without the expressed permission of the copyright owner.
*
****************************************************************/

#pragma once

#include "mocca/net/ConnectionID.h"

#include <chrono>
#include <mutex>
#include <string>
#include <vector>
#include <memory>

namespace mocca {
namespace net {
class IStreamConnection {
public:
    virtual ~IStreamConnection() {}

    virtual std::shared_ptr<const ConnectionID> connectionID() const = 0;
    virtual bool isConnected() const = 0;

    virtual uint32_t send(const uint8_t* data, uint32_t size, std::chrono::milliseconds timeout = std::chrono::milliseconds(100)) const = 0;
    virtual uint32_t receive(uint8_t* buffer, uint32_t maxSize, std::chrono::milliseconds timeout = std::chrono::milliseconds(100)) const = 0;

    std::mutex& sendMutex() { return sendMutex_; }
    std::mutex& receiveMutex() { return receiveMutex_; }

private:
    std::mutex sendMutex_;
    std::mutex receiveMutex_;
};
}
}