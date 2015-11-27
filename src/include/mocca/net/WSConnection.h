//#pragma once
//
//#include "mocca/net/IPhysicalConnection.h"
//#include "mocca/net/Sockets.h"
//#include "mocca/net/TCPNetworkAddress.h"
//#include "mocca/net/WSConnection.h"
//#include "mocca/base64/base64.h"
//#include "mocca/sha1/sha1.h"
//
//#include <iostream>
//#include <string>
//#include <map>
//#include <mutex>
//
//namespace mocca {
//namespace net {
//
//	const int HANDSHAKE_MSG_SIZE_MAX = 512;
//
//	class WSConnection : public IPhysicalConnection {
//public:
//	WSConnection(const WSConnection& other) = delete;
//	~WSConnection();
//
//	TCPNetworkAddress networkAddress() const;
//
//    void send(ByteArray message) const override;
//    ByteArray receive(std::chrono::milliseconds timeout) const override;
//
//private:
//	friend class WSNetworkService;
//	friend class WSConnectionListener;
//
//	WSConnection(const TCPNetworkAddress& networkAddress, std::unique_ptr<IVDA::ConnectionSocket> socket);
//
//	TCPNetworkAddress networkAddress_;
//	std::unique_ptr<IVDA::ConnectionSocket> socket_;
//	mutable std::mutex sendMx_;
//	mutable std::mutex receiveMx_;
//};
//}
//}