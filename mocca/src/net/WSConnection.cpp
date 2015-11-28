//#include "mocca/net/WSConnection.h"
//
//namespace mocca {
//namespace net {
//
//	WSConnection::WSConnection(const TCPNetworkAddress& networkAddress,
//		std::unique_ptr<IVDA::ConnectionSocket> socket)
//		: networkAddress_(networkAddress), socket_(move(socket)) {}
//
//	mocca::net::WSConnection::~WSConnection()
//	{
//		if (socket_ != nullptr) {
//			try {
//				socket_->Disconnect();
//				socket_->Close();
//			}
//			catch (IVDA::SocketException&) {
//				// ignore exception; exceptions must not escape the destructor!
//			}
//		}
//	}
//
//	TCPNetworkAddress WSConnection::networkAddress() const { return networkAddress_; }
//
//	void WSConnection::send(ByteArray message) const {
//		try {
//			std::lock_guard<std::mutex> lock(sendMx_);
//			//socket_->SendInt(message.size());
//			std::string request(message.data());
//			//std::cout << request << ", " << message.size() << std::endl;
//			socket_->SendData((const int8_t*)message.data(), message.size());
//		}
//		catch (const IVDA::SocketConnectionException& err) {
//			throw ConnectionClosedError("Connection to peer " + socket_->GetPeerAddress() +
//				" lost during send operation (internal error: " + err.what() + ")",
//				__FILE__, __LINE__);
//		}
//		catch (const IVDA::SocketException& err) {
//			std::string internalError(err.what());
//			throw NetworkError("Network error in send operation (internal error: " + internalError + ")", __FILE__, __LINE__);
//		}
//	}
//
//	ByteArray WSConnection::receive(std::chrono::milliseconds timeout) const {
//		try {
//
//			int messageSize = HANDSHAKE_MSG_SIZE_MAX;
//			std::lock_guard<std::mutex> lock(receiveMx_);
//
//			ByteArray receiveMsg(messageSize);
//			auto bytesRead = socket_->ReceiveData((int8_t*)receiveMsg.data(), messageSize, static_cast<uint32_t>(timeout.count() / 2));
//
//			std::string receiveMsgString(receiveMsg.data());
//
//			receiveMsg.setSize(bytesRead);
//
//			return receiveMsg;
//
//			/*if (receiveMsgString.substr(0, 3) == "GET")
//			{
//				//Handshake
//				receiveMsg.setSize(bytesRead);
//
//				return receiveMsg;
//			}
//			else
//			{
//				return ByteArray();
//			}*/
//
//			/*std::string request(message1.data());
//			std::cout << "message: " << request << std::endl;*/
//			//-----------------------------------------------------------------
//
//			/*auto bytesRead = socket_->ReceiveInt(messageSize, static_cast<uint32_t>(timeout.count() / 2));
//			std::cout << std::endl;
//
//			std::cout << "bytesRead: " << bytesRead << std::endl;
//			std::cout << "sizeof(int): " << sizeof(int) << std::endl;
//			std::cout << "messageSize: " << messageSize << std::endl;
//
//			if (bytesRead == sizeof(int)) {
//				ByteArray message(messageSize);
//
//				auto bytesRead = socket_->ReceiveData((int8_t*)message.data(), messageSize, static_cast<uint32_t>(timeout.count() / 2));
//
//				std::cout << "bytesRead: " << bytesRead << std::endl;
//				std::cout << std::endl;
//
//				/*if (bytesRead == 4)
//				{
//					std::string request(message.data());
//
//					// get status line
//					//std::getline(message.data(), request);
//
//					for (size_t i = 0; i <= request.length() - 4; ++i) {
//						if (request[i] == '\r' &&
//							request[i + 1] == '\n' &&
//							request[i + 2] == '\r' &&
//							request[i + 3] == '\n') {
//							std::cout << "es klappt" << std::endl;
//						}
//					}
//				}*/
//
//				/*if (bytesRead == messageSize) {
//					message.setSize(messageSize);
//					return message;
//				}
//			}
//			return ByteArray();*/
//		}
//		catch (const IVDA::SocketConnectionException& err) {
//			throw ConnectionClosedError("Connection to peer " + socket_->GetPeerAddress() +
//				" lost during receive operation (internal error: " + err.what() + ")",
//				__FILE__, __LINE__);
//		}
//		catch (const IVDA::SocketException& err) {
//			std::string internalErr(err.what());
//			throw NetworkError("Network error in receive operation (internal error: " + internalErr + ")", __FILE__, __LINE__);
//		}
//	}
//}
//}