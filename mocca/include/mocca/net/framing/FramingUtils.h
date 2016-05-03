/****************************************************************
* Copyright (C) 2016 Andrey Krekhov, David McCann
*
* The content of this file may not be copied and/or distributed
* without the expressed permission of the copyright owner.
*
****************************************************************/

#pragma once

#include "mocca/net/IStreamConnection.h"

#include <chrono>

namespace mocca {
namespace net {

void readUntil(IStreamConnection& stream, std::vector<uint8_t>& buffer, const std::string& delim);
void readExactly(IStreamConnection& stream, std::vector<uint8_t>& buffer, uint32_t size);
void sendAll(IStreamConnection& stream, const uint8_t* buffer, uint32_t size);

template <typename T> T readValue(IStreamConnection& stream) {
    std::vector<uint8_t> buffer;
    readExactly(stream, buffer, sizeof(T));
    return *reinterpret_cast<T*>(buffer.data());
}

template <typename T> void sendValue(IStreamConnection& stream, T value) {
    sendAll(stream, reinterpret_cast<uint8_t*>(&value), sizeof(T));
}
}
}