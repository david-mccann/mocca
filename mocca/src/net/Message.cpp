#include "mocca/net/Message.h"

#include "mocca/base/Memory.h"

using namespace mocca::net;

MessagePart mocca::net::createMessagePart(const std::string& str) {
    MessagePart part = std::make_shared<std::vector<uint8_t>>();
    part->assign(begin(str), end(str));
    return part;
}

std::string mocca::net::readMessagePart(const std::vector<uint8_t>& part) {
    return std::string(reinterpret_cast<const char*>(part.data()), part.size());
}