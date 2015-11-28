#pragma once

#include "mocca/net/WebsocketMessage.h"
#include "mocca/net/WebsocketDataMessage.h"
#include "mocca/net/WebsocketHandshakeMessage.h"
#include "mocca/net/DataBuffer.h"

//class WebsocketHandshakeMessage;

typedef unsigned char       BYTE;

class WebsocketProtocol {
public:
	enum Result
	{
		Success = 0,
		eInsufficientBuffer,
		eIncompletePacket,
		eCorruptPacket,
		eEncodingFailure,
		eDecodingFailure,
		eUndefinedFailure,
		ePacketizingError,
	};

	WebsocketProtocol(void);

    int encodeOutgoingPacket(WebsocketMessage& packet);
	int outgoingMessage(WebsocketMessage& packet, DataBuffer& buffer, unsigned int& nExtractedBytes, bool binary);

	int incomingMessage(DataBuffer& buffer, unsigned int& nWrittenBytes, IncomingPacket*& packet);
	int decodeIncomingPacket(WebsocketMessage* packet, int& serviceId);

    static bool ProcessHandshake( WebsocketHandshakeMessage& request, WebsocketHandshakeMessage& response );
	

};

