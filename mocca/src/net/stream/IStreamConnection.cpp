/****************************************************************
* Copyright (C) 2016 Andrey Krekhov, David McCann
*
* The content of this file may not be copied and/or distributed
* without the expressed permission of the copyright owner.
*
****************************************************************/

#include "mocca/net/IStreamConnection.h"

#include <cstring>

using namespace mocca;
using namespace mocca::net;

IStreamConnection::IStreamConnection()
    : putBackReadPos_(0) {}

uint32_t IStreamConnection::receive(uint8_t* buffer, uint32_t maxSize, std::chrono::milliseconds timeout) const {
    uint32_t bytesRead = 0;
    if (!putBackData_.empty()) {
        uint32_t numPutBackBytes = putBackData_.size() - putBackReadPos_;
        if (numPutBackBytes < maxSize) {
            std::memcpy(buffer, putBackData_.data() + putBackReadPos_, numPutBackBytes);
            bytesRead += numPutBackBytes;
            putBackData_.clear();
            maxSize -= numPutBackBytes;
            putBackReadPos_ = 0;
        } else {
            std::memcpy(buffer, putBackData_.data() + putBackReadPos_, maxSize);
            putBackReadPos_ += maxSize;
            return maxSize;
        }
    }
    bytesRead += readFromStream(buffer + bytesRead, maxSize, timeout);
    return bytesRead;
}

void IStreamConnection::putBack(const uint8_t* data, uint32_t size) {
    std::vector<uint8_t> newPutBackData;
    newPutBackData.reserve(putBackData_.size() - putBackReadPos_ + size);
    newPutBackData.assign(data, data + size);
    newPutBackData.insert(end(newPutBackData), begin(putBackData_) + putBackReadPos_, end(putBackData_));
    putBackData_ = std::move(newPutBackData);
    putBackReadPos_ = 0;
}