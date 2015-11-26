
#pragma once

#ifdef _WIN32
// include Sockets.h and other files that include it before anything else that might include windows.h to make sure windwos.h is not included before winsock2.h
// see http://msdn.microsoft.com/en-us/library/ms737629%28VS.85%29.aspx
#include <winsock2.h>
#else
#include <sys/types.h>
#include <sys/socket.h>
#endif

/*#include <windows.h>

#include <winsock2.h>
#pragma comment(lib,"ws2_32.lib")*/

#include <stdio.h>

#include <ctype.h>
#include <iostream>
#include <fstream>
#include <sstream>

#include <string.h>

#include <map>
#include <vector>
#include <set>

#include "mocca/net/IncomingPacket.h"

using namespace std;

class WebsocketMessage : public IncomingPacket{
public:
	enum
	{
		Handshake = 0,
		DataMessage,
	};
	WebsocketMessage(int type)
	{
		this->type = type;
	}
	~WebsocketMessage(void)
	{
		//
	}
	int GetType()
	{
		return type;
	}
private:
	int type;
};
