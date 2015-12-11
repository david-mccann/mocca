//#include "mocca/net/MoccaConnection.h"
//#include "mocca/net/PhysicalConnection.h"
//#include "mocca/net/stream/TCPConnection.h"
//#include "mocca/testing/LoopbackPhysicalConnection.h"
//
//#include <mutex>
//
//using namespace mocca;
//using namespace mocca::net;
//
//template <typename PhysicalConnectionType>
//MoccaConnection<PhysicalConnectionType>::MoccaConnection(std::unique_ptr<PhysicalConnectionType> physicalConnection)
//    : physicalConnection_(std::move(physicalConnection)) {}
//
//template <typename PhysicalConnectionType> std::string MoccaConnection<PhysicalConnectionType>::identifier() const {
//    return physicalConnection_->identifier();
//}
//
//template <typename PhysicalConnectionType> void MoccaConnection<PhysicalConnectionType>::send(ByteArray message) const {
//    // fixme: performance loss; implement prepend method for ByteArray
//    ByteArray newMessage(message.size() + sizeof(uint32_t));
//    newMessage << message.size();
//    newMessage.append(message);
//    physicalConnection_->send(std::move(newMessage));
//}
//
//template <typename PhysicalConnectionType>
//ByteArray MoccaConnection<PhysicalConnectionType>::receive(std::chrono::milliseconds timeout) const {
//    std::lock_guard<PhysicalConnectionType> lock(*physicalConnection_);
//    auto sizeData = receiveExactly(*physicalConnection_, sizeof(uint32_t), timeout);
//    if (sizeData.isEmpty()) {
//        return ByteArray();
//    }
//    auto size = sizeData.read<uint32_t>();
//    auto data = receiveExactly(*physicalConnection_, size, timeout);
//    return data;
//}
//
//template class MoccaConnection<TCPConnection>;
//template class MoccaConnection<LoopbackPhysicalConnection>;
