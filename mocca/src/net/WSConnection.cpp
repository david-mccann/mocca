#include "mocca/net/WSConnection.h"

using namespace mocca;
using namespace mocca::net;

WSConnection::WSConnection(std::unique_ptr<IPhysicalConnection> physicalConnection)
    : physicalConnection_(std::move(physicalConnection)) {}

std::string mocca::net::WSConnection::identifier() const {
    return physicalConnection_->identifier();
}

void WSConnection::send(ByteArray message) const {}

ByteArray WSConnection::receive(std::chrono::milliseconds timeout) const {
    return ByteArray();
}
