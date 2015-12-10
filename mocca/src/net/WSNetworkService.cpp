//#include "mocca/net/WSNetworkService.h"
//
//#include "mocca/net/WSConnectionAcceptor.h"
//
//using namespace mocca::net;
//
//WSNetworkService::WSNetworkService(std::unique_ptr<IPhysicalNetworkService> physicalNetworkService)
//    : physicalNetworkService_(std::move(physicalNetworkService)) {}
//
//std::string WSNetworkService::protocolStatic() {
//    return "ws";
//}
//
//std::string WSNetworkService::protocol() const {
//    return protocolStatic();
//}
//
//std::string WSNetworkService::transport() const {
//    return physicalNetworkService_->transport();
//}
//
//std::unique_ptr<IProtocolConnection> WSNetworkService::connect(const std::string& connectionString) {
//    throw Error("WebSocket client connections not supported", __FILE__, __LINE__);
//}
//
//std::unique_ptr<IProtocolConnectionAcceptor> WSNetworkService::bind(const std::string& bindingString) {
//    auto physicalConnectionAcceptor = physicalNetworkService_->bind(bindingString);
//    if (physicalConnectionAcceptor != nullptr) {
//        return std::unique_ptr<IProtocolConnectionAcceptor>(new WSConnectionAcceptor(std::move(physicalConnectionAcceptor)));
//    }
//    return nullptr;
//}
//
//IPhysicalNetworkService& WSNetworkService::physicalService() {
//    return *physicalNetworkService_;
//}