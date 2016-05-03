#pragma once

#include <string>
#include <vector>
#include <memory>

namespace mocca {
namespace net {
using MessagePart = std::shared_ptr<std::vector<uint8_t>>;
using Message = std::vector<MessagePart>;

MessagePart createMessagePart(const std::string& str);
std::string readMessagePart(const std::vector<uint8_t>& part);
}
}