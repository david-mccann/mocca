#pragma once

#include "mocca/net/IProtocolConnectionAcceptor.h"
#include "mocca/net/NetworkServiceLocator.h"
#include "mocca/net/LoopbackNetworkService_tmp.h"
#include "mocca/net/TCPNetworkService.h"

namespace mocca {    
    namespace testing
	{
		template<typename T>
		class TempProvideNetworkService {
		public:
			TempProvideNetworkService() {
				net::NetworkServiceLocator::provideService(std::make_shared<T>());
			}
			~TempProvideNetworkService() {
                net::NetworkServiceLocator::removeAll();
			}
		};

        template<typename T> std::string createConnectionString(int index = 0);
        template<typename T> std::string createBindingString(int index = 0);
        template<> std::string createConnectionString<net::TCPNetworkService>(int index);
        template<> std::string createBindingString<net::TCPNetworkService>(int index);
        template<> std::string createConnectionString<net::LoopbackNetworkService_tmp>(int index);
        template<> std::string createBindingString<net::LoopbackNetworkService_tmp>(int index);

        template<typename T> net::Endpoint createConnectionEndpoint(int index = 0);
        template<typename T> net::Endpoint createBindingEndpoint(int index = 0);
        template<> net::Endpoint createConnectionEndpoint<net::TCPNetworkService>(int index);
        template<> net::Endpoint createBindingEndpoint<net::TCPNetworkService>(int index);
        template<> net::Endpoint createConnectionEndpoint<net::LoopbackNetworkService_tmp>(int index);
        template<> net::Endpoint createBindingEndpoint<net::LoopbackNetworkService_tmp>(int index);
	}
}