#pragma once

#include "mocca/net/WebsocketMessage.h"
#include "mocca/net/WebsocketDataMessage.h"
#include "mocca/net/WebsocketHandshakeMessage.h"
#include "mocca/base/ByteArray.h"

typedef unsigned char BYTE;

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
	int outgoingMessage(WebsocketMessage& packet, mocca::ByteArray& buffer, unsigned int& nExtractedBytes, bool binary);

	int incomingMessage(mocca::ByteArray& buffer, unsigned int& nWrittenBytes, IncomingPacket*& packet);
	int decodeIncomingPacket(WebsocketMessage* packet, int& serviceId);

    static bool ProcessHandshake( WebsocketHandshakeMessage& request, WebsocketHandshakeMessage& response );
};

