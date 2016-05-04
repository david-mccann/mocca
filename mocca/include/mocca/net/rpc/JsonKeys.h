#pragma once

#include <string>

namespace mocca {
namespace net {
const std::string& methodKey();
const std::string& paramsKey();
const std::string& statusKey();
const std::string& errorKey();
const std::string& resultKey();

const std::string& successStatus();
const std::string& errorStatus();

const std::string& describe();
}
}