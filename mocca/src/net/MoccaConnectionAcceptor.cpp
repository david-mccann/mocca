#include "mocca/net/MoccaConnection.h"
#include "mocca/net/MoccaConnectionAcceptor.h"

using namespace mocca::net;

MoccaConnectionAcceptor::MoccaConnectionAcceptor(std::unique_ptr<IPhysicalConnectionAcceptor> physicalConnectionAcceptor)
    : physicalConnectionAcceptor_(std::move(physicalConnectionAcceptor)) {}

std::unique_ptr<IProtocolConnection> MoccaConnectionAcceptor::getConnection(std::chrono::milliseconds timeout) {
    auto physicalConnection = physicalConnectionAcceptor_->getConnection(timeout);
    if (physicalConnection != nullptr) {
        return std::unique_ptr<IProtocolConnection>(new MoccaConnection(std::move(physicalConnection)));
    }
    return nullptr;
}
