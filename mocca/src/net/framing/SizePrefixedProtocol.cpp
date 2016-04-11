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

Message SizePrefixedProtocol::readMessageFromStream(IStreamConnection& connection) {
    std::lock_guard<std::mutex> lock(connection.receiveMutex());
    auto numParts = readValue<uint32_t>(connection);
    Message message;
    for (uint32_t i = 0; i < numParts; ++i) {
        auto size = readValue<uint32_t>(connection);
        auto buffer = std::make_shared<std::vector<uint8_t>>();
        readExactly(connection, *buffer, size);
        message.push_back(buffer);
    }
    return message;
}

void SizePrefixedProtocol::writeMessageToStream(IStreamConnection& connection, Message message) {
    std::lock_guard<std::mutex> lock(connection.sendMutex());
    sendValue(connection, static_cast<uint32_t>(message.size()));
    for (const auto& part : message) {
        sendValue(connection, static_cast<uint32_t>(part->size()));
        sendAll(connection, part->data(), part->size());
    }
}