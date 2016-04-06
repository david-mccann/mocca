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

Message SizePrefixedProtocol::readMessageFromStream(IStreamConnection& connection, std::chrono::milliseconds timeout) {
    std::lock_guard<std::mutex> lock(connection.receiveMutex());

    auto numParts = connection.receiveValue<uint32_t>(timeout);

    Message message;
    for (uint32_t i = 0; i < numParts; ++i) {
        uint32_t size = connection.receiveValue<uint32_t>(timeout);
        auto buffer = std::make_shared<std::vector<uint8_t>>();
        readExactly(connection, *buffer, size, timeout);
        message.push_back(buffer);
    }
    return message;
}

void SizePrefixedProtocol::writeMessageToStream(IStreamConnection& connection, Message message) {
    std::lock_guard<std::mutex> lock(connection.sendMutex());
    connection.sendValue(static_cast<uint32_t>(message.size()));
    for (const auto& part : message) {
        connection.sendValue(static_cast<uint32_t>(part->size()));
        connection.send(part->data(), part->size());
    }
}