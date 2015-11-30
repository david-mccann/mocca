#include "mocca/testing/NetworkTesting.h"

#include "mocca/testing/LoopbackPhysicalConnectionAcceptor.h"
#include "mocca/net/MoccaNetworkService.h"

#include <chrono>

using namespace mocca::net;

template <> std::string mocca::testing::createConnectionString<TCPNetworkService>(int index) {
    return "localhost:" + std::to_string(5678 + index);
}

template <> std::string mocca::testing::createBindingString<TCPNetworkService>(int index) {
    return std::to_string(5678 + index);
}

template <> std::string mocca::testing::createConnectionString<LoopbackPhysicalNetworkService>(int index) {
    return "physicalMessageQueue" + std::to_string(index);
}

template <> std::string mocca::testing::createBindingString<LoopbackPhysicalNetworkService>(int index) {
    return "physicalMessageQueue" + std::to_string(index);
}

template <> std::string mocca::testing::createConnectionString<LoopbackNetworkService>(int index) {
    return "messageQueue" + std::to_string(index);
}

template <> std::string mocca::testing::createBindingString<LoopbackNetworkService>(int index) {
    return "messageQueue" + std::to_string(index);
}

template <> Endpoint mocca::testing::createConnectionEndpoint<TCPNetworkService>(int index) {
    return Endpoint(MoccaNetworkService::protocolStatic(), TCPNetworkService::transportStatic(),
                    createConnectionString<TCPNetworkService>(index));
}

template <> Endpoint mocca::testing::createBindingEndpoint<TCPNetworkService>(int index) {
    return Endpoint(MoccaNetworkService::protocolStatic(), TCPNetworkService::transportStatic(),
                    createBindingString<TCPNetworkService>(index));
}

template <> Endpoint mocca::testing::createConnectionEndpoint<LoopbackPhysicalNetworkService>(int index) {
    return Endpoint(MoccaNetworkService::protocolStatic(), LoopbackPhysicalNetworkService::transportStatic(),
                    createConnectionString<LoopbackPhysicalNetworkService>(index));
}

template <> Endpoint mocca::testing::createBindingEndpoint<LoopbackPhysicalNetworkService>(int index) {
    return Endpoint(MoccaNetworkService::protocolStatic(), LoopbackPhysicalNetworkService::transportStatic(),
                    createBindingString<LoopbackPhysicalNetworkService>(index));
}

template <> Endpoint mocca::testing::createConnectionEndpoint<LoopbackNetworkService>(int index) {
    return Endpoint(LoopbackNetworkService::protocolStatic(), LoopbackNetworkService::transportStatic(),
        createConnectionString<LoopbackNetworkService>(index));
}

template <> Endpoint mocca::testing::createBindingEndpoint<LoopbackNetworkService>(int index) {
    return Endpoint(LoopbackNetworkService::protocolStatic(), LoopbackNetworkService::transportStatic(),
        createBindingString<LoopbackNetworkService>(index));
}