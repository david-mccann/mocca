#include "mocca/testing/NetworkTesting.h"
#include "mocca/testing/LoopbackPhysicalConnectionAcceptor.h"

#include <chrono>

using namespace mocca::net;

template <> std::string mocca::testing::createConnectionString<TCPNetworkService>(int index) {
    return "localhost:" + std::to_string(5678 + index);
}

template <> std::string mocca::testing::createBindingString<TCPNetworkService>(int index) {
    return std::to_string(5678 + index);
}

template <> std::string mocca::testing::createConnectionString<LoopbackPhysicalNetworkService>(int index) {
    return "messageQueue" + std::to_string(index);
}

template <> std::string mocca::testing::createBindingString<LoopbackPhysicalNetworkService>(int index) {
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

template <> Endpoint mocca::testing::createConnectionEndpoint<LoopbackPhysicalNetworkService>(int index) {
    return Endpoint(LoopbackPhysicalNetworkService::transportStatic(),
                    createConnectionString<LoopbackPhysicalNetworkService>(index));
}

template <> Endpoint mocca::testing::createBindingEndpoint<LoopbackPhysicalNetworkService>(int index) {
    return Endpoint(LoopbackPhysicalNetworkService::transportStatic(),
                    createBindingString<LoopbackPhysicalNetworkService>(index));
}