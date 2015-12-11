#pragma once

#include <deque>

#include "mocca/net/stream/MessageQueueStream.h"
#include "mocca/net/stream/StreamAcceptor.h"

namespace mocca {
namespace net {

class MessageQueueStreamAcceptor : public StreamAcceptor<MessageQueueStreamAcceptor, MessageQueueStream> {
public:
    using LoopbackConnectionQueue = MessageQueue<std::unique_ptr<MessageQueueStream>>;
    friend class StreamAcceptor<MessageQueueStreamAcceptor, MessageQueueStream>;
    MessageQueueStreamAcceptor(std::shared_ptr<LoopbackConnectionQueue> connectionQueue);

private:
    std::unique_ptr<MessageQueueStream> acceptImpl(std::chrono::milliseconds timeout);

private:
    std::shared_ptr<LoopbackConnectionQueue> connectionQueue_;
};
}
}