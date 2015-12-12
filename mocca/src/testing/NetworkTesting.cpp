#include "mocca/testing/NetworkTesting.h"

#include "mocca/base/Error.h"

std::string mocca::testing::createAddress(const std::string & protocol, int index)
{
    if (protocol.find("tcp") != std::string::npos) {
        return "localhost:" + std::to_string(5678 + index);
    }
    else if (protocol.find("queue") != std::string::npos) {
        return "queue_" + std::to_string(index);
    }
    throw Error("Invalid protocol " + protocol, __FILE__, __LINE__);
}

std::string mocca::testing::createBindingAddress(const std::string & protocol, int index)
{
    if (protocol.find("tcp") != std::string::npos) {
        return std::to_string(5678 + index);
    }
    else if (protocol.find("queue") != std::string::npos) {
        return "queue_" + std::to_string(index);
    }
    throw Error("Invalid protocol " + protocol, __FILE__, __LINE__);
}


//template <> Endpoint mocca::testing::createConnectionEndpoint<TCPNetworkService>(int index) {
//    return Endpoint(MoccaNetworkService<TCPNetworkService>::protocolStatic(), createAddress<TCPNetworkService>(index));
//}
//
//template <> Endpoint mocca::testing::createBindingEndpoint<TCPNetworkService>(int index) {
//    return Endpoint(MoccaNetworkService<TCPNetworkService>::protocolStatic(), createBindingAddress<TCPNetworkService>(index));
//}
//
//template <> Endpoint mocca::testing::createConnectionEndpoint<LoopbackPhysicalNetworkService>(int index) {
//    return Endpoint(MoccaNetworkService<TCPNetworkService>::protocolStatic(), createAddress<LoopbackPhysicalNetworkService>(index));
//}
//
//template <> Endpoint mocca::testing::createBindingEndpoint<LoopbackPhysicalNetworkService>(int index) {
//    return Endpoint(MoccaNetworkService<TCPNetworkService>::protocolStatic(), createBindingAddress<LoopbackPhysicalNetworkService>(index));
//}
//
//template <> Endpoint mocca::testing::createConnectionEndpoint<LoopbackNetworkService>(int index) {
//    return Endpoint(LoopbackNetworkService::protocolStatic(), createAddress<LoopbackNetworkService>(index));
//}
//
//template <> Endpoint mocca::testing::createBindingEndpoint<LoopbackNetworkService>(int index) {
//    return Endpoint(LoopbackNetworkService::protocolStatic(), createBindingAddress<LoopbackNetworkService>(index));
//}