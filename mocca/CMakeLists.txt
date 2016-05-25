CMAKE_MINIMUM_REQUIRED(VERSION 3.0.0)
PROJECT(mocca LANGUAGES CXX)

ADD_LIBRARY(mocca
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

	src/testing/NetworkTesting.cpp)

OPTION(MOCCA_RUNTIME_CHECKS "Enable runtime checks (slows down execution)" ON)
IF (MOCCA_RUNTIME_CHECKS)
	TARGET_COMPILE_DEFINITIONS(mocca PRIVATE MOCCA_RUNTIME_CHECKS)
ENDIF()

TARGET_INCLUDE_DIRECTORIES(
	mocca PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
	mocca PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/thirdparty)

TARGET_COMPILE_FEATURES(mocca
	PUBLIC cxx_auto_type
	PUBLIC cxx_lambdas
	PUBLIC cxx_nullptr
	PUBLIC cxx_override
	PUBLIC cxx_range_for
	PUBLIC cxx_rvalue_references
	PUBLIC cxx_static_assert
	PUBLIC cxx_strong_enums
	PUBLIC cxx_uniform_initialization
	PUBLIC cxx_variadic_templates)
	
TARGET_LINK_LIBRARIES(mocca
	PUBLIC AES
	PUBLIC sha1
	PUBLIC base64
	PUBLIC jsoncpp)
IF (UNIX)
	TARGET_LINK_LIBRARIES(mocca PUBLIC pthread)
ENDIF()