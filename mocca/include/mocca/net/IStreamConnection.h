/****************************************************************
* Copyright (C) 2016 Andrey Krekhov, David McCann
*
* The content of this file may not be copied and/or distributed
* without the expressed permission of the copyright owner.
*
****************************************************************/

#pragma once

#include "mocca/base/ByteArray.h"
#include "mocca/net/ConnectionID.h"

#include <chrono>
#include <mutex>
#include <string>

namespace mocca {
namespace net {
class IStreamConnection {
public:
    IStreamConnection();
    virtual ~IStreamConnection() {}

    virtual std::shared_ptr<const ConnectionID> connectionID() const = 0;
    virtual bool isConnected() const = 0;
    virtual void send(ByteArray message, std::chrono::milliseconds timeout = std::chrono::milliseconds(100)) const = 0;
    ByteArray receive(uint32_t maxSize, std::chrono::milliseconds timeout = std::chrono::milliseconds(100)) const;

    void putBack(const ByteArray& data);
    std::mutex& sendMutex() { return sendMutex_; }
    std::mutex& receiveMutex() { return receiveMutex_; }

protected:
    virtual ByteArray readFromStream(uint32_t maxSize, std::chrono::milliseconds timeout = std::chrono::milliseconds(100)) const = 0;

private:
    mutable ByteArray putBackData_;
    mutable uint32_t putBackReadPos_;
    std::mutex sendMutex_;
    std::mutex receiveMutex_;
};
}
}