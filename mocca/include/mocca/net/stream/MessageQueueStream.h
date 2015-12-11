#pragma once

#include "mocca/base/MessageQueue.h"
#include "mocca/net/stream/StreamBase.h"

namespace mocca {
namespace net {
class MessageQueueStream : public StreamBase<MessageQueueStream> {
friend class StreamBase<MessageQueueStream>;
private:
    ByteArray readImpl(uint32_t maxSize, std::chrono::milliseconds timeout);
    void writeImpl(ByteArray data, std::chrono::milliseconds timeout);

private:
    MessageQueue<unsigned char> queue_;
};
}
}