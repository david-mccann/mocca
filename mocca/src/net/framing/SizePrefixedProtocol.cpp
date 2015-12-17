#include "mocca/net/framing/SizePrefixedProtocol.h"

#include "mocca/net/framing/FramingUtils.h"

using namespace mocca;
using namespace mocca::net;

std::unique_ptr<FramingStrategy> mocca::net::SizePrefixedProtocol::clone() const {
    return std::unique_ptr<FramingStrategy>(new SizePrefixedProtocol(*this));
}

ByteArray SizePrefixedProtocol::readFrameFromStream(IStreamConnection& connection, std::chrono::milliseconds timeout) {
    std::lock_guard<IStreamConnection> lock(connection);
    auto sizeData = readExactly(connection, sizeof(uint32_t), timeout);
    if (sizeData.isEmpty()) {
        return ByteArray();
    }
    auto frameSize = sizeData.read<uint32_t>();
    auto frame = readExactly(connection, frameSize, timeout);
    return frame;
}

void SizePrefixedProtocol::writeFrameToStream(IStreamConnection& connection, ByteArray frame, std::chrono::milliseconds timeout) {
    // fixme: performance loss; implement prepend method for ByteArray
    ByteArray newFrame(frame.size() + sizeof(uint32_t));
    newFrame << frame.size();
    newFrame.append(frame);
    connection.send(std::move(newFrame));
}