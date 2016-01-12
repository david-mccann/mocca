/****************************************************************
* Copyright (C) 2016 Andrey Krekhov, David McCann
*
* The content of this file may not be copied and/or distributed
* without the expressed permission of the copyright owner.
*
****************************************************************/

#pragma once

#include "mocca/net/IMessageConnectionFactory.h"
#include "mocca/net/IStreamConnectionFactory.h"
#include "mocca/net/framing/FramingStrategy.h"

namespace mocca {
namespace net {

class FramingConnectionFactory : public IMessageConnectionFactory {
public:
    FramingConnectionFactory(std::unique_ptr<IStreamConnectionFactory> streamConnectionFactory,
                         std::unique_ptr<FramingStrategy> framingStrategy);

    std::unique_ptr<IMessageConnection> connect(const std::string& address) override;
    std::unique_ptr<IMessageConnectionAcceptor> bind(const std::string& address) override;

    IStreamConnectionFactory& streamConnectionFactory();

private:
    std::unique_ptr<IStreamConnectionFactory> streamConnectionFactory_;
    std::unique_ptr<FramingStrategy> framingStrategy_;
};
}
}
