#pragma once

#include "mocca/base/ByteArray.h"

#include <algorithm>
#include <chrono>

namespace mocca {
namespace net {

template <typename IOStreamType>
ByteArray readUntil(IOStreamType& stream, const std::string& delim, std::chrono::milliseconds timeout = std::chrono::milliseconds(100),
                    uint32_t chunkSize = 512);

template <typename IOStreamType>
ByteArray readExactly(IOStreamType& stream, uint32_t size, std::chrono::milliseconds timeout = std::chrono::milliseconds(100));
}
}