#pragma once

#include "mocca/base/MessageQueue.h"
#include "mocca/net/stream/IOStreamBase.h"

namespace mocca {
namespace net {
class MessageQueueStream : public IOStreamBase<MessageQueueStream> {
friend class IOStreamBase<MessageQueueStream>;
private:
    ByteArray readImpl(uint32_t maxSize, std::chrono::milliseconds timeout);
    void writeImpl(ByteArray data, std::chrono::milliseconds timeout);

private:
    MessageQueue<unsigned char> queue_;
};
}
}