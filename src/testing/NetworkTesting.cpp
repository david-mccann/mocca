#include "mocca/testing/NetworkTesting.h"
#include "mocca/net/LoopbackConnectionListener.h"

#include <chrono>

using namespace mocca::net;

template <> std::string mocca::testing::createConnectionString<TCPNetworkService>(int index) {
    return "localhost:" + std::to_string(5678 + index);
}

template <> std::string mocca::testing::createBindingString<TCPNetworkService>(int index) {
    return std::to_string(5678 + index);
}

template <> std::string mocca::testing::createConnectionString<LoopbackNetworkService>(int index) {
    return "messageQueue" + std::to_string(index);
}

template <> std::string mocca::testing::createBindingString<LoopbackNetworkService>(int index) {
    return "messageQueue" + std::to_string(index);
}

template <> Endpoint mocca::testing::createConnectionEndpoint<TCPNetworkService>(int index) {
    return Endpoint(TCPNetworkService::transportStatic(),
                    createConnectionString<TCPNetworkService>(index));
}

template <> Endpoint mocca::testing::createBindingEndpoint<TCPNetworkService>(int index) {
    return Endpoint(TCPNetworkService::transportStatic(),
                    createBindingString<TCPNetworkService>(index));
}

template <> Endpoint mocca::testing::createConnectionEndpoint<LoopbackNetworkService>(int index) {
    return Endpoint(LoopbackNetworkService::transportStatic(),
                    createConnectionString<LoopbackNetworkService>(index));
}

template <> Endpoint mocca::testing::createBindingEndpoint<LoopbackNetworkService>(int index) {
    return Endpoint(LoopbackNetworkService::transportStatic(),
                    createBindingString<LoopbackNetworkService>(index));
}