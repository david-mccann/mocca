#include "mocca/net/stream/MessageQueueStream.h"

using namespace mocca;
using namespace mocca::net;

ByteArray MessageQueueStream::readImpl(uint32_t maxSize, std::chrono::milliseconds timeout) {
    ByteArray result;
    while (result.size() < maxSize) {
        auto dataNullable = queue_.tryDequeue(timeout);
        if (dataNullable.isNull()) {
            return result;
        }
        auto data = dataNullable.get();
        result.append(&data, sizeof(unsigned char));
    }
    return result;
}

void MessageQueueStream::writeImpl(ByteArray data, std::chrono::milliseconds timeout) {
    auto dataPtr = data.data();
    auto dataEnd = dataPtr + data.size();
    while (dataPtr != dataEnd) {
        queue_.enqueue(*dataPtr);
        ++dataPtr;
    }
}