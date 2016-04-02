/****************************************************************
* Copyright (C) 2016 Andrey Krekhov, David McCann
*
* The content of this file may not be copied and/or distributed
* without the expressed permission of the copyright owner.
*
****************************************************************/

#pragma once

#include "mocca/net/IMessageConnectionFactory.h"
#include "mocca/net/message/NewLoopbackConnectionSpawner.h"

#include <vector>

namespace mocca {
namespace net {
class NewLoopbackConnectionFactory : public IMessageConnectionFactory {
public:
    std::unique_ptr<IMessageConnection> connect(const std::string& name) override;
    std::unique_ptr<IMessageConnectionAcceptor> bind(const std::string& machine, const std::string& port) override;

private:
    std::shared_ptr<NewLoopbackConnectionSpawner> getSpawner(const std::string& name);

private:
    std::vector<std::shared_ptr<NewLoopbackConnectionSpawner>> spawners_;
};
}
}