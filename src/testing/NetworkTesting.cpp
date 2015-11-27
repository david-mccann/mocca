#include "mocca/testing/NetworkTesting.h"
#include "mocca/net/LoopbackConnectionAcceptor_tmp.h"

#include <chrono>

using namespace mocca::net;

template <> std::string mocca::testing::createConnectionString<TCPNetworkService>(int index) {
    return "localhost:" + std::to_string(5678 + index);
}

template <> std::string mocca::testing::createBindingString<TCPNetworkService>(int index) {
    return std::to_string(5678 + index);
}

template <> std::string mocca::testing::createConnectionString<LoopbackNetworkService_tmp>(int index) {
    return "messageQueue" + std::to_string(index);
}

template <> std::string mocca::testing::createBindingString<LoopbackNetworkService_tmp>(int index) {
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

template <> Endpoint mocca::testing::createConnectionEndpoint<LoopbackNetworkService_tmp>(int index) {
    return Endpoint(LoopbackNetworkService_tmp::transportStatic(),
                    createConnectionString<LoopbackNetworkService_tmp>(index));
}

template <> Endpoint mocca::testing::createBindingEndpoint<LoopbackNetworkService_tmp>(int index) {
    return Endpoint(LoopbackNetworkService_tmp::transportStatic(),
                    createBindingString<LoopbackNetworkService_tmp>(index));
}