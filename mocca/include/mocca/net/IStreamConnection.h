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

namespace mocca {
namespace net {
class IStreamConnection {
public:
    IStreamConnection();
    virtual ~IStreamConnection() {}

    virtual std::shared_ptr<const ConnectionID> connectionID() const = 0;
    virtual bool isConnected() const = 0;
    
    virtual void send(const uint8_t* data, uint32_t size) const = 0;
    template<typename T> void sendValue(T value) {
        send(reinterpret_cast<const uint8_t*>(&value), sizeof(T));
    }

    uint32_t receive(uint8_t* buffer, uint32_t maxSize, std::chrono::milliseconds timeout = std::chrono::milliseconds(100)) const;
    template<typename T> T receiveValue(std::chrono::milliseconds timeout = std::chrono::milliseconds(100)) {
        T value;
        receive(reinterpret_cast<uint8_t*>(&value), sizeof(T), timeout);
        return value;
    }

    void putBack(const uint8_t* data, uint32_t size);
    std::mutex& sendMutex() { return sendMutex_; }
    std::mutex& receiveMutex() { return receiveMutex_; }

protected:
    virtual uint32_t readFromStream(uint8_t* buffer, uint32_t maxSize, std::chrono::milliseconds timeout = std::chrono::milliseconds(100)) const = 0;

private:
    mutable std::vector<uint8_t> putBackData_;
    mutable uint32_t putBackReadPos_;
    std::mutex sendMutex_;
    std::mutex receiveMutex_;
};
}
}