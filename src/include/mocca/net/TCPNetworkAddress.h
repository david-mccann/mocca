#pragma once

#include <string>
#include <iostream>

namespace mocca {
namespace net {

class TCPNetworkAddress {
	friend std::ostream& operator<<(std::ostream& os, const TCPNetworkAddress& obj);

public:
    TCPNetworkAddress(const std::string& ip, int port);
	TCPNetworkAddress(const std::string& connectionString);

    std::string toString() const;

    std::string ip() const;
    int port() const;

	std::string connectionString() const;
	std::string bindingString() const;

	static int parsePort(const std::string& str);

private:
	static void checkIp(const std::string& ip);
	static void checkPort(int port);

private:
    std::string ip_;
    int port_;
};
}
}