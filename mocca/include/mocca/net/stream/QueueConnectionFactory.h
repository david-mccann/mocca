/****************************************************************
* Copyright (C) 2016 Andrey Krekhov, David McCann
*
* The content of this file may not be copied and/or distributed
* without the expressed permission of the copyright owner.
*
****************************************************************/

#pragma once

#include "mocca/net/IStreamConnectionFactory.h"
#include "mocca/net/stream/QueueConnectionSpawner.h"

#include <vector>

namespace mocca {
namespace net {
class QueueConnectionFactory : public IStreamConnectionFactory {
public:
    std::unique_ptr<IStreamConnection> connect(const std::string& name) override;
    std::unique_ptr<IStreamConnectionAcceptor> bind(const std::string& name) override;

private:
    std::shared_ptr<QueueConnectionSpawner> getSpawner(const std::string& name);

private:
    std::vector<std::shared_ptr<QueueConnectionSpawner>> spawners_;
};
}
}
