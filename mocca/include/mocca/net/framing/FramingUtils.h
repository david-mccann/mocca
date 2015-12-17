#pragma once

#include "mocca/net/IStreamConnection.h"

#include <chrono>

namespace mocca {
namespace net {
ByteArray readUntil(IStreamConnection& stream, const std::string& delim, std::chrono::milliseconds timeout = std::chrono::milliseconds(100),
                    uint32_t chunkSize = 512);
ByteArray readExactly(IStreamConnection& stream, uint32_t size, std::chrono::milliseconds timeout = std::chrono::milliseconds(100));
}
}