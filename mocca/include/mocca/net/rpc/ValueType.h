#pragma once

#include "mocca/base/BidirectionalMap.h"

#include "jsoncpp/json.h"

namespace mocca {
namespace net {

using ValueTypeMapper = mocca::BidirectionalMap<JsonCpp::ValueType, std::string>;
const ValueTypeMapper& valueTypeMapper();

}
}