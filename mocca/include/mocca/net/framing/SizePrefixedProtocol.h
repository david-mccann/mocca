#pragma once

#include "mocca/net/framing/FramingProtocolBase.h"
#include "mocca/net/framing/FramingUtils.h"

namespace mocca {
namespace net {

template <typename IOStreamType> class SizePrefixedProtocol : public FramingProtocolBase<SizePrefixedProtocol<IOStreamType>, IOStreamType> {
public:
    using FramingProtocolBase::FramingProtocolBase; // inherit constructor

private:
    ByteArray readFrameFromStreamImpl() {
        std::lock_guard<IOStreamType> lock(*ioStream_);
        auto sizeData = readExactly(*ioStream_, sizeof(uint32_t), timeout);
        if (sizeData.isEmpty()) {
            return ByteArray();
        }
        auto size = sizeData.read<uint32_t>();
        auto data = readExactly(*ioStream_, size, timeout);
        return data;
    }
    
    void writeFrameToStreamImpl(ByteArray frame) {
        // fixme: performance loss; implement prepend method for ByteArray
        ByteArray newMessage(message.size() + sizeof(uint32_t));
        newMessage << message.size();
        newMessage.append(message);
        ioStream_->send(std::move(newMessage));
    }
};
}
}
