#include "gtest/gtest.h"

#include "mocca/base/Endian.h"
#include "mocca/net/NetworkError.h"
#include "mocca/net/framing/FramingUtils.h"
#include "mocca/net/framing/WebSocketProtocol.h"
#include "mocca/net/message/FramingConnectionFactory.h"
#include "mocca/net/stream/QueueConnectionFactory.h"

#include <array>

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

    ByteArray createPayloadData(uint32_t size) {
        ByteArray payload(size);
        payload.setSize(size);
        for (uint32_t i = 0; i < size; ++i) {
            payload.data()[i] = ('a' + (i % ('z' - 'a')));
        }
        return payload;
    }

    const unsigned char mediumPayload = 126;
    const unsigned char bigPayload = 127;
    const unsigned char flags = 0x81;
    std::array<unsigned char, 4> mask;

    ByteArray maskData(const ByteArray& data) {
        ByteArray result(data.size());
        result.setSize(data.size());
        for (uint32_t i = 0; i < data.size(); ++i) {
            result[i] = data[i] ^ mask[i % 4];
        }
        return result;
    }

    std::unique_ptr<FramingConnectionFactory> factory;
};

TEST_F(WebsocketTest, ReceiveSmallPayload) {
    auto wsAcceptor = factory->bind("mq");
    auto& lbService = factory->streamConnectionFactory();
    auto lbClientConnection = lbService.connect("mq");
    lbClientConnection->send(ByteArray::createFromRaw((unsigned char*)clientHandshakeStr.c_str(), clientHandshakeStr.size()));
    auto wsServerConnection = wsAcceptor->accept();

    unsigned char payloadSize = 125;
    ByteArray data(payloadSize + 6); // 6 bytes header
    data.append(&flags, 1);
    data.append(&payloadSize, 1);
    data.append(&mask, 4);
    auto payload = createPayloadData(payloadSize);
    data.append(maskData(payload));
    lbClientConnection->send(std::move(data));
    auto receivedData = wsServerConnection->receive();
    ASSERT_EQ(payloadSize, receivedData.size());
    ASSERT_TRUE(std::memcmp(payload.data(), receivedData.data(), payloadSize) == 0);
}

TEST_F(WebsocketTest, ReceiveMediumPayload) {
    auto wsAcceptor = factory->bind("mq");
    auto& lbService = factory->streamConnectionFactory();
    auto lbClientConnection = lbService.connect("mq");
    lbClientConnection->send(ByteArray::createFromRaw((unsigned char*)clientHandshakeStr.c_str(), clientHandshakeStr.size()));
    auto wsServerConnection = wsAcceptor->accept();

    uint16_t payloadSize = 40000;
    uint16_t payloadSizeBE = swap_uint16(payloadSize);
    ByteArray data(payloadSize + 8); // 8 bytes header
    data.append(&flags, 1);
    data.append(&mediumPayload, 1);
    data.append(&payloadSizeBE, 2);
    data.append(&mask, 4);
    auto payload = createPayloadData(payloadSize);
    data.append(maskData(payload));
    lbClientConnection->send(std::move(data));
    auto receivedData = wsServerConnection->receive();
    ASSERT_EQ(payloadSize, receivedData.size());
    ASSERT_TRUE(std::memcmp(payload.data(), receivedData.data(), payloadSize) == 0);
}

TEST_F(WebsocketTest, ReceiveBigPayload) {
    auto wsAcceptor = factory->bind("mq");
    auto& lbService = factory->streamConnectionFactory();
    auto lbClientConnection = lbService.connect("mq");
    lbClientConnection->send(ByteArray::createFromRaw((unsigned char*)clientHandshakeStr.c_str(), clientHandshakeStr.size()));
    auto wsServerConnection = wsAcceptor->accept();

    uint64_t payloadSize = 70000;
    uint64_t payloadSizeBE = swap_uint64(payloadSize);
    ByteArray data(static_cast<uint32_t>(payloadSize + 14)); // 14 bytes header
    data.append(&flags, 1);
    data.append(&bigPayload, 1);
    data.append(&payloadSizeBE, 8);
    data.append(&mask, 4);
    auto payload = createPayloadData(static_cast<uint32_t>(payloadSize));
    data.append(maskData(payload));
    lbClientConnection->send(std::move(data));
    auto receivedData = wsServerConnection->receive();
    ASSERT_EQ(payloadSize, receivedData.size());
    ASSERT_TRUE(std::memcmp(payload.data(), receivedData.data(), static_cast<uint32_t>(payloadSize)) == 0);
}

TEST_F(WebsocketTest, SendSmallPayload) {
    auto wsAcceptor = factory->bind("mq");
    auto& lbService = factory->streamConnectionFactory();
    auto lbClientConnection = lbService.connect("mq");
    lbClientConnection->send(ByteArray::createFromRaw((unsigned char*)clientHandshakeStr.c_str(), clientHandshakeStr.size()));
    auto wsServerConnection = wsAcceptor->accept();
    readUntil(*lbClientConnection, ByteArray(), "\r\n\r\n"); // remove hanshake response from client buffer

    unsigned char payloadSize = 125;
    auto payload = createPayloadData(payloadSize);
    wsServerConnection->send(payload.clone());

    ByteArray expectedData(payloadSize + 2); // 2 bytes header
    expectedData.append(&flags, 1);
    expectedData.append(&payloadSize, 1);
    expectedData.append(payload);

    auto receivedData = lbClientConnection->receive(200);
    ASSERT_EQ(expectedData.size(), receivedData.size());
    ASSERT_TRUE(std::memcmp(expectedData.data(), receivedData.data(), expectedData.size()) == 0);
}

TEST_F(WebsocketTest, SendMediumPayload) {
    auto wsAcceptor = factory->bind("mq");
    auto& lbService = factory->streamConnectionFactory();
    auto lbClientConnection = lbService.connect("mq");
    lbClientConnection->send(ByteArray::createFromRaw((unsigned char*)clientHandshakeStr.c_str(), clientHandshakeStr.size()));
    auto wsServerConnection = wsAcceptor->accept();
    readUntil(*lbClientConnection, ByteArray(), "\r\n\r\n"); // remove hanshake response from client buffer

    uint16_t payloadSize = 40000;
    uint16_t payloadSizeBE = swap_uint16(payloadSize);
    auto payload = createPayloadData(payloadSize);
    wsServerConnection->send(payload.clone());

    ByteArray expectedData(payloadSize + 4); // 4 bytes header
    expectedData.append(&flags, 1);
    expectedData.append(&mediumPayload, 1);
    expectedData.append(&payloadSizeBE, 2);
    expectedData.append(payload);

    auto receivedData = lbClientConnection->receive(41000);
    ASSERT_EQ(expectedData.size(), receivedData.size());
    ASSERT_TRUE(std::memcmp(expectedData.data(), receivedData.data(), expectedData.size()) == 0);
}

TEST_F(WebsocketTest, SendBigPayload) {
    auto wsAcceptor = factory->bind("mq");
    auto& lbService = factory->streamConnectionFactory();
    auto lbClientConnection = lbService.connect("mq");
    lbClientConnection->send(ByteArray::createFromRaw((unsigned char*)clientHandshakeStr.c_str(), clientHandshakeStr.size()));
    auto wsServerConnection = wsAcceptor->accept();
    readUntil(*lbClientConnection, ByteArray(), "\r\n\r\n"); // remove hanshake response from client buffer

    uint64_t payloadSize = 70000;
    uint64_t payloadSizeBE = swap_uint64(payloadSize);
    auto payload = createPayloadData(static_cast<uint32_t>(payloadSize));
    wsServerConnection->send(payload.clone());

    ByteArray expectedData(static_cast<uint32_t>(payloadSize) + 12); // 12 bytes header
    expectedData.append(&flags, 1);
    expectedData.append(&bigPayload, 1);
    expectedData.append(&payloadSizeBE, 8);
    expectedData.append(payload);

    auto receivedData = lbClientConnection->receive(71000);
    ASSERT_EQ(expectedData.size(), receivedData.size());
    ASSERT_TRUE(std::memcmp(expectedData.data(), receivedData.data(), expectedData.size()) == 0);
}

TEST_F(WebsocketTest, CloseConnection) {
    auto wsAcceptor = factory->bind("mq");
    auto& lbService = factory->streamConnectionFactory();
    auto lbClientConnection = lbService.connect("mq");
    lbClientConnection->send(ByteArray::createFromRaw((unsigned char*)clientHandshakeStr.c_str(), clientHandshakeStr.size()));
    auto wsServerConnection = wsAcceptor->accept();
    const unsigned char closeData[] = {0x88};
    lbClientConnection->send(ByteArray::createFromRaw(&closeData, 1));
    ASSERT_THROW(wsServerConnection->receive(), ConnectionClosedError);
}