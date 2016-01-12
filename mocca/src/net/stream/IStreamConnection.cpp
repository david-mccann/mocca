/****************************************************************
* Copyright (C) 2016 Andrey Krekhov, David McCann
*
* The content of this file may not be copied and/or distributed
* without the expressed permission of the copyright owner.
*
****************************************************************/

#include "mocca/net/IStreamConnection.h"

using namespace mocca;
using namespace mocca::net;

IStreamConnection::IStreamConnection()
    : putBackReadPos_(0) {}

ByteArray IStreamConnection::receive(uint32_t maxSize, std::chrono::milliseconds timeout) const {
    ByteArray result;
    if (!putBackData_.isEmpty()) {
        uint32_t numPutBackBytes = putBackData_.size() - putBackReadPos_;
        if (numPutBackBytes < maxSize) {
            result.append(putBackData_.data() + putBackReadPos_, numPutBackBytes);
            putBackData_.setSize(0);
            maxSize -= numPutBackBytes;
            putBackReadPos_ = 0;
        } else {
            result.append(putBackData_.data() + putBackReadPos_, maxSize);
            putBackReadPos_ += maxSize;
            return result;
        }
    }
    result.append(readFromStream(maxSize, timeout));
    return result;
}

void IStreamConnection::putBack(const ByteArray& data) {
    ByteArray newPutBackData(putBackData_.size() - putBackReadPos_ + data.size());
    newPutBackData.append(data);
    newPutBackData.append(putBackData_.data() + putBackReadPos_, putBackData_.size() - putBackReadPos_);
    putBackData_ = std::move(newPutBackData);
    putBackReadPos_ = 0;
}