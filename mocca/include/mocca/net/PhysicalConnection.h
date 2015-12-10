#pragma once

#include "mocca/base/ByteArray.h"

#include <algorithm>
#include <chrono>


namespace mocca {
namespace net {

template <typename PhysicalConnectionType>
ByteArray receiveUntil(PhysicalConnectionType& connection, const std::string& delim,
                       std::chrono::milliseconds timeout = std::chrono::milliseconds(100), uint32_t chunkSize = 512);

template <typename PhysicalConnectionType>
ByteArray receiveExactly(PhysicalConnectionType& connection, uint32_t size,
                         std::chrono::milliseconds timeout = std::chrono::milliseconds(100));
}
}