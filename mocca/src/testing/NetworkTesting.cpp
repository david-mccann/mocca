/****************************************************************
* Copyright (C) 2016 Andrey Krekhov, David McCann
*
* The content of this file may not be copied and/or distributed
* without the expressed permission of the copyright owner.
*
****************************************************************/

#include "mocca/testing/NetworkTesting.h"

#include "mocca/base/Error.h"

using namespace mocca::net;

std::string mocca::testing::createAddress(const std::string& protocol, int index) {
    if (protocol.find("tcp") != std::string::npos) {
        return "localhost:" + std::to_string(5678 + index);
    } else if (protocol.find("queue") != std::string::npos) {
        return "local:queue_" + std::to_string(index);
    } else if (protocol.find("loopback") != std::string::npos) {
        return "local:loopback_" + std::to_string(index);
    }
    throw Error("Invalid protocol " + protocol, __FILE__, __LINE__);
}

std::string mocca::testing::createBindingMachine(const std::string& protocol) {
    if (protocol.find("tcp") != std::string::npos) {
        return "0.0.0.0";
    } else if (protocol.find("queue") != std::string::npos) {
        return "local";
    } else if (protocol.find("loopback") != std::string::npos) {
        return "local";
    }
    throw Error("Invalid protocol " + protocol, __FILE__, __LINE__);
}

std::string mocca::testing::createBindingPort(const std::string& protocol, int index) {
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
    return Endpoint(protocol + ":" + createAddress(protocol, index));
}

Endpoint mocca::testing::createBindingEndpoint(const std::string& protocol, int index) {
    return Endpoint(protocol, createBindingMachine(protocol), createBindingPort(protocol, index));
}