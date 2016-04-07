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

    bool ok;
    auto numParts = connection.receiveValue<uint32_t>(ok, timeout);
    if (!ok) {
        return Message();
    }

    Message message;
    for (uint32_t i = 0; i < numParts; ++i) {
        uint32_t size = connection.receiveValue<uint32_t>(ok, timeout);
        if (!ok) {
            return Message();
        }
        auto buffer = std::make_shared<std::vector<uint8_t>>();
        if (readExactly(connection, *buffer, size, timeout) == ReadStatus::Incomplete) 
        {
            connection.putBack(reinterpret_cast<uint8_t*>(&numParts), sizeof(uint32_t));
            for (auto m : message) {
                uint32_t s = m->size();
                connection.putBack(reinterpret_cast<uint8_t*>(&s), sizeof(uint32_t));
                connection.putBack(m->data(), s);
            }
            connection.putBack(reinterpret_cast<uint8_t*>(&size), sizeof(uint32_t));
            connection.putBack(buffer->data(), buffer->size());
            return Message();
        }
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