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

enum class ReadStatus { Complete, Incomplete };

ReadStatus readUntil(IStreamConnection& stream, ByteArray& buffer, const std::string& delim,
                     std::chrono::milliseconds timeout = std::chrono::milliseconds(100), uint32_t chunkSize = 512);
ReadStatus readExactly(IStreamConnection& stream, ByteArray& buffer, uint32_t size,
                       std::chrono::milliseconds timeout = std::chrono::milliseconds(100));
}
}