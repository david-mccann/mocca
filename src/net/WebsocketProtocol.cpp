// WebsocketProtocol.cpp : Defines the exported functions for the DLL application.
//

#ifdef _WIN32
// include Sockets.h and other files that include it before anything else that might include windows.h to make sure windwos.h is not included before winsock2.h
// see http://msdn.microsoft.com/en-us/library/ms737629%28VS.85%29.aspx
#include <winsock2.h>
#else
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
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

#include "mocca/net/WebsocketProtocol.h"
#include "mocca/base64/base64.h"
#include "mocca/sha1/sha1.h"

#include "mocca/net/WebsocketHandshakeMessage.h"
#include "mocca/net/WebsocketDataMessage.h"
#include "mocca/net/DataBuffer.h"

using namespace std;

// This is the constructor of a class that has been exported.
// see WebsocketProtocol.h for the class definition
WebsocketProtocol::WebsocketProtocol()
{
	return;
}

int WebsocketProtocol::incomingMessage(DataBuffer& buffer, unsigned int& nExtractedBytes, IncomingPacket*& packet)
{
	std::string message(buffer.GetBuffer());

	if (message.length() == 0)
        return eIncompletePacket;

	if (message.substr(0, 3) == "GET")
	{
		WebsocketHandshakeMessage* pMessage = new WebsocketHandshakeMessage(buffer.GetBuffer(), buffer.GetDataSize());
		packet = pMessage;
		return Success;
	}

	//In the other cases, we should expect a data message : 
	int nMinExpectedSize = 6;
	if (buffer.GetDataSize() < nMinExpectedSize)
		return eIncompletePacket;

	BYTE payloadFlags = buffer.getAt(0);
	if (payloadFlags != 129)
		return eUndefinedFailure;

	BYTE basicSize = buffer.getAt(1) & 0x7F;
	unsigned long int payloadSize;
	int masksOffset;

	if (basicSize <= 125)
	{
		payloadSize = basicSize;
		masksOffset = 2;
	}
	else if (basicSize == 126)
	{
		nMinExpectedSize += 2;
		if (buffer.GetDataSize() < nMinExpectedSize)
			return eIncompletePacket;
		payloadSize = ntohs(*(u_short*)(buffer.GetBuffer() + 2));
		masksOffset = 4;
	}
	else if (basicSize == 127)
	{
		nMinExpectedSize += 8;
		if (buffer.GetDataSize() < nMinExpectedSize)
			return eIncompletePacket;
		payloadSize = ntohl(*(u_long*)(buffer.GetBuffer() + 2));
		masksOffset = 10;
	}
	else
		return eUndefinedFailure;

	nMinExpectedSize += payloadSize;
	if (buffer.GetDataSize() < nMinExpectedSize)
		return eIncompletePacket;

	BYTE masks[4];
	memcpy(masks, buffer.GetBuffer() + masksOffset, 4);

	char* payload = new char[payloadSize + 1];
	memcpy(payload, buffer.GetBuffer() + masksOffset + 4, payloadSize);
	for (unsigned long int i = 0; i < payloadSize; i++) {
		payload[i] = (payload[i] ^ masks[i % 4]);
	}
	payload[payloadSize] = '\0';

	WebsocketDataMessage* pMessage = new WebsocketDataMessage(payload);
	//serviceId = 1;
	nExtractedBytes = nMinExpectedSize;
	packet = pMessage;

	delete payload;
	return Success;
}

int WebsocketProtocol::outgoingMessage(WebsocketMessage& packet, DataBuffer& buffer, unsigned int& nWrittenBytes, bool binary)
{
	WebsocketMessage& message = (WebsocketMessage&) packet;

	if (message.GetType() == WebsocketMessage::Handshake)
	{
		WebsocketHandshakeMessage& handshake = (WebsocketHandshakeMessage&) message;

        string strRaw = handshake.Serialize();
        if(strRaw.size() > buffer.getRemainingSize())
            return eInsufficientBuffer;

        buffer.Append((char*)strRaw.c_str(), strRaw.size());
        nWrittenBytes = strRaw.size();
        return Success;
	}

	//https://tools.ietf.org/html/rfc6455
	if (message.GetType() == WebsocketMessage::DataMessage)
	{
		WebsocketDataMessage& dataMessage = (WebsocketDataMessage&) message;
        string strData = dataMessage.GetEncodedData();

		std::cout << strData << ": aus" << std::endl;

		unsigned long int payloadSize = strData.size();

		if (binary)
			payloadSize++;

		int expectedSize = payloadSize + 1; //flags byte.
		if(payloadSize <= 125  && payloadSize <= 65535 )
			expectedSize += 1;
		else if(payloadSize > 125  && payloadSize <= 65535)
			expectedSize += 3;
		else
			expectedSize += 9;

		if (expectedSize > buffer.getRemainingSize())
			return eInsufficientBuffer;

		//create the flags byte
		char payloadFlags = 129;

		if (binary)
			payloadFlags = 130;

		buffer.Append(&payloadFlags, 1);

		//create the length byte
		if (payloadSize <= 125)
		{
			char basicSize = payloadSize;
			buffer.Append(&basicSize, 1);
		}
		else if (payloadSize > 125 & payloadSize <= 65535)
		{
			char basicSize = 126;
			buffer.Append(&basicSize, 1);

			char len[2];
			len[0] = ( payloadSize >> 8 ) & 255;
			len[1] = ( payloadSize ) & 255;
			buffer.Append(len, 2);
		}
		else
		{
			char basicSize = 127;
			buffer.Append(&basicSize, 1);

			char len[8];
			len[0] = ( payloadSize >> 56 ) & 255;
			len[1] = ( payloadSize >> 48  ) & 255;
			len[2] = ( payloadSize >> 40 ) & 255;
			len[3] = ( payloadSize >> 32  ) & 255;
			len[4] = ( payloadSize >> 24 ) & 255;
			len[5] = ( payloadSize >> 16  ) & 255;
			len[6] = ( payloadSize >> 8 ) & 255;
			len[7] = ( payloadSize ) & 255;
			buffer.Append(len, 8);
		}

        buffer.Append((char*)strData.c_str(), strData.size());

		if (binary)
		{
			char nonAsci = 150;
			buffer.Append(&nonAsci, 1);
		}

        nWrittenBytes = expectedSize;
        return Success;
	}
	
	return eUndefinedFailure;
}

bool WebsocketProtocol::ProcessHandshake( WebsocketHandshakeMessage& request, WebsocketHandshakeMessage& response )
{
    std::string server_key = request.GetField("Sec-WebSocket-Key");
    server_key += "258EAFA5-E914-47DA-95CA-C5AB0DC85B11";

    SHA1        sha;
    unsigned int    message_digest[5];

    sha.Reset();
    sha << server_key.c_str();

    sha.Result(message_digest);
    // convert sha1 hash bytes to network byte order because this sha1
    //  library works on ints rather than bytes
    for (int i = 0; i < 5; i++) {
        message_digest[i] = htonl(message_digest[i]);
    }

    server_key = base64_encode(
        reinterpret_cast<const unsigned char*>
        (message_digest),20
        );


    response.SetField("Upgrade", "websocket");
    response.SetField("Connection", "Upgrade");
    response.SetField("Sec-WebSocket-Accept", server_key);

    return true;
}

int WebsocketProtocol::decodeIncomingPacket(WebsocketMessage* packet, int& serviceId)
{
	WebsocketMessage* pMessage = (WebsocketMessage*) packet;
	if (pMessage->GetType() == WebsocketMessage::DataMessage)
	{
		WebsocketDataMessage* pDataMessage = (WebsocketDataMessage*) pMessage;
		if (!pDataMessage->Decode())
		{
			return eDecodingFailure;
		}
		serviceId = pDataMessage->GetServiceId();
		return Success;
	}
	return Success;
}

int WebsocketProtocol::encodeOutgoingPacket(WebsocketMessage& packet)
{
	WebsocketMessage& message = (WebsocketMessage&) packet;
	if (message.GetType() == WebsocketMessage::DataMessage)
	{
		WebsocketDataMessage& dataMessage = (WebsocketDataMessage&) message;
		return dataMessage.Encode() ? Success : eEncodingFailure;
	}
	return Success;
}




