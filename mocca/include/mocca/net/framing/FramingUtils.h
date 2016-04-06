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

ReadStatus readUntil(IStreamConnection& stream, std::vector<uint8_t>& buffer, const std::string& delim,
                     std::chrono::milliseconds timeout = std::chrono::milliseconds(100));
ReadStatus readExactly(IStreamConnection& stream, std::vector<uint8_t>& buffer, uint32_t size,
                       std::chrono::milliseconds timeout = std::chrono::milliseconds(100));
}
}