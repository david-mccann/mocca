#pragma once

#include "mocca/base/ByteArray.h"
#include "mocca/net/IPhysicalConnection.h"

namespace mocca {
namespace net {
ByteArray receiveUntil(IPhysicalConnection& connection, const std::string& delim,
                       std::chrono::milliseconds timeout = std::chrono::milliseconds(200), uint32_t chunkSize = 512);
ByteArray receiveExactly(IPhysicalConnection& connection, uint32_t size,
                         std::chrono::milliseconds timeout = std::chrono::milliseconds(200));
}
}