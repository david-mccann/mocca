/****************************************************************
* Copyright (C) 2016 Andrey Krekhov, David McCann
*
* The content of this file may not be copied and/or distributed
* without the expressed permission of the copyright owner.
*
****************************************************************/

#include "mocca/net/framing/SizePrefixedProtocol.h"

#include "mocca/net/framing/FramingUtils.h"

using namespace mocca;
using namespace mocca::net;

std::unique_ptr<FramingStrategy> mocca::net::SizePrefixedProtocol::clone() const {
    return std::unique_ptr<FramingStrategy>(new SizePrefixedProtocol(*this));
}

std::string SizePrefixedProtocol::name() const {
    return "prefixed";
}

ByteArray SizePrefixedProtocol::readFrameFromStream(IStreamConnection& connection, std::chrono::milliseconds timeout) {
    std::lock_guard<std::mutex> lock(connection.receiveMutex());

    std::vector<uint8_t> sizeBuffer;
    if (readExactly(connection, sizeBuffer, sizeof(uint32_t), timeout) == ReadStatus::Incomplete) {
        connection.putBack(sizeBuffer.data(), sizeBuffer.size());
        return ByteArray();
    }

    std::vector<uint8_t> buffer;
    auto frameSize = *reinterpret_cast<uint32_t*>(sizeBuffer.data());
    if (readExactly(connection, buffer, frameSize, timeout) == ReadStatus::Incomplete) {
        connection.putBack(buffer.data(), buffer.size());
        connection.putBack(sizeBuffer.data(), sizeBuffer.size());
        return ByteArray();
    }
    return ByteArray::createFromRaw(buffer.data(), buffer.size());
}

void SizePrefixedProtocol::writeFrameToStream(IStreamConnection& connection, ByteArray frame) {
    // fixme: performance loss; implement prepend method for ByteArray
    std::lock_guard<std::mutex> lock(connection.sendMutex());
    ByteArray newFrame(frame.size() + sizeof(uint32_t));
    newFrame << frame.size();
    newFrame.append(frame);
    connection.send(newFrame.data(), newFrame.size());
}