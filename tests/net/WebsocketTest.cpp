/****************************************************************
* Copyright (C) 2016 Andrey Krekhov, David McCann
*
* The content of this file may not be copied and/or distributed
* without the expressed permission of the copyright owner.
*
****************************************************************/

#include "gtest/gtest.h"

#include "mocca/base/Endian.h"
#include "mocca/net/NetworkError.h"
#include "mocca/net/framing/FramingUtils.h"
#include "mocca/net/framing/WebSocketProtocol.h"
#include "mocca/net/message/FramingConnectionFactory.h"
#include "mocca/net/stream/QueueConnectionFactory.h"

#include <array>
#include <cstring>

using namespace mocca;
using namespace mocca::net;

class WebsocketTest : public ::testing::Test {
protected:
    WebsocketTest()
        : mask({{0x11, 0x22, 0x33, 0x44}}) {
        factory.reset(new FramingConnectionFactory(std::unique_ptr<IStreamConnectionFactory>(new QueueConnectionFactory()),
                                                   std::unique_ptr<FramingStrategy>(new WebSocketProtocol())));
    }

    virtual ~WebsocketTest() {
        // You can do clean-up work that doesn't throw exceptions here.
    }

    std::string clientHandshakeStr =
        "GET / HTTP/1.1\r\n"
        "Host: localhost\r\n"
        "Connection: Upgrade\r\n"
        "Pragma: no-cache\r\n"
        "Cache-Control: no-cache\r\n"
        "Upgrade: websocket\r\n"
        "Origin: chrome-extension://pfdhoblngboilpfeibdedpjgfnlcodoo\r\n"
        "Sec-WebSocket-Version: 13\r\n"
        "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/46.0.2490.86 Safari/537.36\r\n"
        "Accept-Encoding: gzip, deflate, sdch\r\n"
        "Accept-Language: de-DE,de;q=0.8,en-US;q=0.6,en;q=0.4\r\n"
        "Sec-WebSocket-Key: cdsNUQEVKJWSMNeiLaaCbw==\r\n"
        "Sec-WebSocket-Extensions: permessage-deflate; client_max_window_bits\r\n\r\n";

    std::vector<uint8_t> createPayloadData(uint32_t size) {
        std::vector<uint8_t> payload(size);
        for (uint32_t i = 0; i < size; ++i) {
            payload[i] = ('a' + (i % ('z' - 'a')));
        }
        return payload;
    }

    const unsigned char mediumPayload = 126;
    const unsigned char bigPayload = 127;
    const unsigned char flags = 0x81;
    std::array<unsigned char, 4> mask;

    std::vector<uint8_t> maskData(const std::vector<uint8_t>& data) {
        std::vector<uint8_t> result(data.size());
        for (uint32_t i = 0; i < data.size(); ++i) {
            result[i] = data[i] ^ mask[i % 4];
        }
        return result;
    }

    std::unique_ptr<FramingConnectionFactory> factory;
};

TEST_F(WebsocketTest, ReceiveSmallPayload) {
    auto wsAcceptor = factory->bind("local", "mq");
    auto& lbService = factory->streamConnectionFactory();
    auto lbClientConnection = lbService.connect("local:mq");
    lbClientConnection->send((unsigned char*)clientHandshakeStr.c_str(), clientHandshakeStr.size());
    auto wsServerConnection = wsAcceptor->accept();

    unsigned char payloadSize = 125;
    std::vector<uint8_t> data(6); // 6 bytes header
    data[0] = flags;
    data[1] = payloadSize;
    for (int i = 0; i < 4; ++i) {
        data[i + 2] = mask[i];
    }
    auto payload = createPayloadData(payloadSize);
    auto masked = maskData(payload);
    data.insert(end(data), begin(masked), end(masked));
    lbClientConnection->send(data.data(), data.size());
    auto receivedData = wsServerConnection->receive();
    ASSERT_EQ(payloadSize, receivedData[0]->size());
    ASSERT_TRUE(std::memcmp(payload.data(), receivedData[0]->data(), payloadSize) == 0);
}

TEST_F(WebsocketTest, ReceiveMediumPayload) {
    auto wsAcceptor = factory->bind("local", "mq");
    auto& lbService = factory->streamConnectionFactory();
    auto lbClientConnection = lbService.connect("local:mq");
    lbClientConnection->send((unsigned char*)clientHandshakeStr.c_str(), clientHandshakeStr.size());
    auto wsServerConnection = wsAcceptor->accept();

    uint16_t payloadSize = 40000;
    uint16_t payloadSizeBE = swap_uint16(payloadSize);
    std::vector<uint8_t> data(8); // 8 bytes header
    data[0] = flags;
    data[1] = mediumPayload;
    *reinterpret_cast<uint16_t*>(&data[2]) = payloadSizeBE;
    for (int i = 0; i < 4; ++i) {
        data[i + 4] = mask[i];
    }
    auto payload = createPayloadData(payloadSize);
    auto masked = maskData(payload);
    data.insert(end(data), begin(masked), end(masked));
    lbClientConnection->send(data.data(), data.size());
    auto receivedData = wsServerConnection->receive();
    ASSERT_EQ(payloadSize, receivedData[0]->size());
    ASSERT_TRUE(std::memcmp(payload.data(), receivedData[0]->data(), payloadSize) == 0);
}

TEST_F(WebsocketTest, ReceiveBigPayload) {
    auto wsAcceptor = factory->bind("local", "mq");
    auto& lbService = factory->streamConnectionFactory();
    auto lbClientConnection = lbService.connect("local:mq");
    lbClientConnection->send((unsigned char*)clientHandshakeStr.c_str(), clientHandshakeStr.size());
    auto wsServerConnection = wsAcceptor->accept();

    uint64_t payloadSize = 70000;
    uint64_t payloadSizeBE = swap_uint64(payloadSize);
    std::vector<uint8_t> data(14); // 14 bytes header
    data[0] = flags;
    data[1] = bigPayload;
    *reinterpret_cast<uint64_t*>(&data[2]) = payloadSizeBE;
    for (int i = 0; i < 4; ++i) {
        data[i + 10] = mask[i];
    }
    auto payload = createPayloadData(static_cast<uint32_t>(payloadSize));
    auto masked = maskData(payload);
    data.insert(end(data), begin(masked), end(masked));
    lbClientConnection->send(data.data(), data.size());
    auto receivedData = wsServerConnection->receive();
    ASSERT_EQ(payloadSize, receivedData[0]->size());
    ASSERT_TRUE(std::memcmp(payload.data(), receivedData[0]->data(), static_cast<uint32_t>(payloadSize)) == 0);
}

TEST_F(WebsocketTest, SendSmallPayload) {
    auto wsAcceptor = factory->bind("local", "mq");
    auto& lbService = factory->streamConnectionFactory();
    auto lbClientConnection = lbService.connect("local:mq");
    lbClientConnection->send((unsigned char*)clientHandshakeStr.c_str(), clientHandshakeStr.size());
    auto wsServerConnection = wsAcceptor->accept();
    std::vector<uint8_t> tmp;
    readUntil(*lbClientConnection, tmp, "\r\n\r\n"); // remove hanshake response from client buffer

    unsigned char payloadSize = 125;
    auto payload = createPayloadData(payloadSize);
    wsServerConnection->send(Message{ std::make_shared<std::vector<uint8_t>>(payload) });

    std::vector<uint8_t> expectedData(2); // 2 bytes header
    expectedData[0] = flags;
    expectedData[1] = payloadSize;
    expectedData.insert(end(expectedData), begin(payload), end(payload));

    std::vector<uint8_t> receivedData(expectedData.size());
    auto bytesReceived = lbClientConnection->receive(receivedData.data(), 200);
    ASSERT_EQ(expectedData.size(), bytesReceived);
    ASSERT_TRUE(std::memcmp(expectedData.data(), receivedData.data(), expectedData.size()) == 0);
}

TEST_F(WebsocketTest, SendMediumPayload) {
    auto wsAcceptor = factory->bind("local", "mq");
    auto& lbService = factory->streamConnectionFactory();
    auto lbClientConnection = lbService.connect("local:mq");
    lbClientConnection->send((unsigned char*)clientHandshakeStr.c_str(), clientHandshakeStr.size());
    auto wsServerConnection = wsAcceptor->accept();
    std::vector<uint8_t> tmp;
    readUntil(*lbClientConnection, tmp, "\r\n\r\n"); // remove hanshake response from client buffer

    uint16_t payloadSize = 40000;
    uint16_t payloadSizeBE = swap_uint16(payloadSize);
    auto payload = createPayloadData(payloadSize);
    wsServerConnection->send(Message{ std::make_shared<std::vector<uint8_t>>(payload) });

    std::vector<uint8_t> expectedData(4); // 4 bytes header
    expectedData[0] = flags;
    expectedData[1] = mediumPayload;
    *reinterpret_cast<uint16_t*>(&expectedData[2]) = payloadSizeBE;
    expectedData.insert(end(expectedData), begin(payload), end(payload));

    std::vector<uint8_t> receivedData(expectedData.size());
    auto bytesReceived = lbClientConnection->receive(receivedData.data(), 41000);
    ASSERT_EQ(expectedData.size(), bytesReceived);
    ASSERT_TRUE(std::memcmp(expectedData.data(), receivedData.data(), expectedData.size()) == 0);
}

TEST_F(WebsocketTest, SendBigPayload) {
    auto wsAcceptor = factory->bind("local", "mq");
    auto& lbService = factory->streamConnectionFactory();
    auto lbClientConnection = lbService.connect("local:mq");
    lbClientConnection->send((unsigned char*)clientHandshakeStr.c_str(), clientHandshakeStr.size());
    auto wsServerConnection = wsAcceptor->accept();
    std::vector<uint8_t> tmp;
    readUntil(*lbClientConnection, tmp, "\r\n\r\n"); // remove hanshake response from client buffer

    uint64_t payloadSize = 70000;
    uint64_t payloadSizeBE = swap_uint64(payloadSize);
    auto payload = createPayloadData(static_cast<uint32_t>(payloadSize));
    wsServerConnection->send(Message{ std::make_shared<std::vector<uint8_t>>(payload) });

    std::vector<uint8_t> expectedData(10); // 10 bytes header
    expectedData[0] = flags;
    expectedData[1] = bigPayload;
    *reinterpret_cast<uint64_t*>(&expectedData[2]) = payloadSizeBE;
    expectedData.insert(end(expectedData), begin(payload), end(payload));

    std::vector<uint8_t> receivedData(expectedData.size());
    auto bytesReceived = lbClientConnection->receive(receivedData.data(), 71000);
    ASSERT_EQ(expectedData.size(), bytesReceived);
    ASSERT_TRUE(std::memcmp(expectedData.data(), receivedData.data(), expectedData.size()) == 0);
}

TEST_F(WebsocketTest, CloseConnection) {
    auto wsAcceptor = factory->bind("local", "mq");
    auto& lbService = factory->streamConnectionFactory();
    auto lbClientConnection = lbService.connect("local:mq");
    lbClientConnection->send((unsigned char*)clientHandshakeStr.c_str(), clientHandshakeStr.size());
    auto wsServerConnection = wsAcceptor->accept();
    const unsigned char closeData[] = {0x88};
    lbClientConnection->send(closeData, 1);
    ASSERT_THROW(wsServerConnection->receive(), ConnectionClosedError);
}