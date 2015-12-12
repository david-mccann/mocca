//#include "mocca/testing/NetworkTesting.h"
//
//
//#include <chrono>
//
//using namespace mocca::net;
//
//template <> std::string mocca::testing::createAddress<TCPObjectFactory>(int index) {
//    return "localhost:" + std::to_string(5678 + index);
//}
//
//template <> std::string mocca::testing::createBindingAddress<TCPObjectFactory>(int index) {
//    return std::to_string(5678 + index);
//}
//
//template <> std::string mocca::testing::createAddress<MessageQueueObjectFactory>(int index) {
//    return "messageQueue" + std::to_string(index);
//}
//
//template <> std::string mocca::testing::createBindingAddress<MessageQueueObjectFactory>(int index) {
//    return "messageQueue" + std::to_string(index);
//}
//
//template <> std::string mocca::testing::createAddress<LoopbackNetworkService>(int index) {
//    return "messageQueue" + std::to_string(index);
//}
//
//template <> std::string mocca::testing::createBindingAddress<LoopbackNetworkService>(int index) {
//    return "messageQueue" + std::to_string(index);
//}
//
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