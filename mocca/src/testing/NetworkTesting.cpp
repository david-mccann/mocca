#include "mocca/testing/NetworkTesting.h"

#include "mocca/base/Error.h"

using namespace mocca::net;

std::string mocca::testing::createAddress(const std::string& protocol, int index) {
    if (protocol.find("tcp") != std::string::npos) {
        return "localhost:" + std::to_string(5678 + index);
    } else if (protocol.find("queue") != std::string::npos) {
        return "queue_" + std::to_string(index);
    } else if (protocol.find("loopback") != std::string::npos) {
        return "loopback_" + std::to_string(index);
    }
    throw Error("Invalid protocol " + protocol, __FILE__, __LINE__);
}

std::string mocca::testing::createBindingAddress(const std::string& protocol, int index) {
    if (protocol.find("tcp") != std::string::npos) {
        return std::to_string(5678 + index);
    } else if (protocol.find("queue") != std::string::npos) {
        return "queue_" + std::to_string(index);
    } else if (protocol.find("loopback") != std::string::npos) {
        return "loopback_" + std::to_string(index);
    }
    throw Error("Invalid protocol " + protocol, __FILE__, __LINE__);
}

Endpoint mocca::testing::createConnectionEndpoint(const std::string& protocol, int index) {
    return Endpoint(protocol, createAddress(protocol, index));
}

Endpoint mocca::testing::createBindingEndpoint(const std::string& protocol, int index) {
    return Endpoint(protocol, createBindingAddress(protocol, index));
}