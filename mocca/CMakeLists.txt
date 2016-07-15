CMAKE_MINIMUM_REQUIRED(VERSION 3.2.0)
PROJECT(mocca LANGUAGES CXX)

SET(MOCCA_HEADER
	include/mocca/base/Compression.h
	include/mocca/base/Thread.h
	include/mocca/base/BidirectionalMap.h
	include/mocca/base/CommandLineParser.h
	include/mocca/base/ContainerTools.h
	include/mocca/base/Encryption.h
	include/mocca/base/Endian.h
	include/mocca/base/Error.h
	include/mocca/base/Explicit.h
	include/mocca/base/Memory.h
	include/mocca/base/MessageQueue.h
	include/mocca/base/Nullable.h
	include/mocca/base/ObjectPool.h
	include/mocca/base/Singleton.h
	include/mocca/base/StringTools.h
	include/mocca/base/BinaryUtil.h

	include/mocca/fs/Filesystem.h
	include/mocca/fs/Path.h
	include/mocca/fs/File.h

	include/mocca/log/ConsoleLog.h
	include/mocca/log/HTMLLog.h
	include/mocca/log/Log.h
	include/mocca/log/LogManager.h
	include/mocca/log/StreamLog.h
	include/mocca/log/TextLog.h
	include/mocca/log/CallbackLog.h

	include/mocca/net/NetworkError.h
	include/mocca/net/ConnectionAggregator.h
	include/mocca/net/ConnectionFactorySelector.h
	include/mocca/net/ConnectionID.h
	include/mocca/net/Endpoint.h
	include/mocca/net/IMessageConnection.h
	include/mocca/net/IMessageConnectionAcceptor.h
	include/mocca/net/IMessageConnectionFactory.h
	include/mocca/net/IStreamConnection.h
	include/mocca/net/IStreamConnectionAcceptor.h
	include/mocca/net/IStreamConnectionFactory.h
	include/mocca/net/Message.h
	include/mocca/net/AutoConnection.h

	include/mocca/net/framing/SizePrefixedProtocol.h
	include/mocca/net/framing/WebSocketProtocol.h
	include/mocca/net/framing/FramingStrategy.h
	include/mocca/net/framing/FramingUtils.h

	include/mocca/net/message/FramingConnectionAcceptor.h
	include/mocca/net/message/FramingConnectionFactory.h
	include/mocca/net/message/LoopbackConnection.h
	include/mocca/net/message/LoopbackConnectionAcceptor.h
	include/mocca/net/message/LoopbackConnectionFactory.h
	include/mocca/net/message/LoopbackConnectionSpawner.h
	include/mocca/net/message/FramingConnection.h

	include/mocca/net/rpc/Method.h
	include/mocca/net/rpc/MethodDescription.h
	include/mocca/net/rpc/RpcClient.h
	include/mocca/net/rpc/ValueType.h
	include/mocca/net/rpc/Dispatcher.h
	include/mocca/net/rpc/JsonKeys.h

	include/mocca/net/stream/QueueConnectionSpawner.h
	include/mocca/net/stream/Sockets.h
	include/mocca/net/stream/stddefines.h
	include/mocca/net/stream/TCPConnection.h
	include/mocca/net/stream/TCPConnectionAcceptor.h
	include/mocca/net/stream/TCPConnectionFactory.h
	include/mocca/net/stream/TCPNetworkAddress.h
	include/mocca/net/stream/QueueConnection.h
	include/mocca/net/stream/QueueConnectionAcceptor.h
	include/mocca/net/stream/QueueConnectionFactory.h

	include/mocca/testing/NetworkTesting.h)

SET(MOCCA_SOURCE
	src/base/Compression.cpp
	src/base/Thread.cpp
	src/base/CommandLineParser.cpp
	src/base/Encryption.cpp
	src/base/StringTools.cpp

	src/fs/Filesystem.cpp
	src/fs/Path.cpp
	src/fs/File.cpp

	src/log/LogManager.cpp
	src/log/StreamLog.cpp
	src/log/TextLog.cpp
	src/log/CallbackLog.cpp
	src/log/ConsoleLog.cpp
	src/log/HTMLLog.cpp
	src/log/Log.cpp

	src/net/framing/FramingUtils.cpp
	src/net/framing/SizePrefixedProtocol.cpp
	src/net/framing/WebSocketProtocol.cpp

	src/net/message/FramingConnectionAcceptor.cpp
	src/net/message/FramingConnectionFactory.cpp
	src/net/message/LoopbackConnection.cpp
	src/net/message/LoopbackConnectionAcceptor.cpp
	src/net/message/LoopbackConnectionFactory.cpp
	src/net/message/LoopbackConnectionSpawner.cpp
	src/net/message/FramingConnection.cpp

	src/net/rpc/Method.cpp
	src/net/rpc/MethodDescription.cpp
	src/net/rpc/RpcClient.cpp
	src/net/rpc/ValueType.cpp
	src/net/rpc/Dispatcher.cpp
	src/net/rpc/JsonKeys.cpp

	src/net/stream/QueueConnectionSpawner.cpp
	src/net/stream/Sockets.cpp
	src/net/stream/TCPConnection.cpp
	src/net/stream/TCPConnectionAcceptor.cpp
	src/net/stream/TCPConnectionFactory.cpp
	src/net/stream/TCPNetworkAddress.cpp
	src/net/stream/QueueConnection.cpp
	src/net/stream/QueueConnectionAcceptor.cpp
	src/net/stream/QueueConnectionFactory.cpp

	src/net/Message.cpp
	src/net/ConnectionAggregator.cpp
	src/net/ConnectionFactorySelector.cpp
	src/net/Endpoint.cpp
	src/net/AutoConnection.cpp

	src/testing/NetworkTesting.cpp)
	
ADD_LIBRARY(mocca ${MOCCA_HEADER} ${MOCCA_SOURCE})

OPTION(MOCCA_RUNTIME_CHECKS "Enable runtime checks (slows down execution)" ON)
IF (MOCCA_RUNTIME_CHECKS)
	TARGET_COMPILE_DEFINITIONS(mocca PRIVATE MOCCA_RUNTIME_CHECKS)
ENDIF()

TARGET_INCLUDE_DIRECTORIES(
	mocca PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

SET_PROPERTY(TARGET mocca PROPERTY CXX_STANDARD 11)
SET_PROPERTY(TARGET mocca PROPERTY CXX_STANDARD_REQUIRED ON)
	
TARGET_LINK_LIBRARIES(mocca
	PUBLIC AES
	PUBLIC sha1
	PUBLIC base64
	PUBLIC jsoncpp
	PUBLIC liblz4)
IF (UNIX)
	TARGET_LINK_LIBRARIES(mocca PUBLIC pthread)
ENDIF()