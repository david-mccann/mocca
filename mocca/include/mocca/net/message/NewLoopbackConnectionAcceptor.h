/****************************************************************
* Copyright (C) 2016 Andrey Krekhov, David McCann
*
* The content of this file may not be copied and/or distributed
* without the expressed permission of the copyright owner.
*
****************************************************************/

#pragma once

#include "mocca/net/IMessageConnectionAcceptor.h"
#include "mocca/net/message/NewLoopbackConnectionSpawner.h"

namespace mocca {
namespace net {

class NewLoopbackConnectionAcceptor : public IMessageConnectionAcceptor {
public:
    NewLoopbackConnectionAcceptor(std::shared_ptr<NewLoopbackConnectionSpawner> spawner);
    std::unique_ptr<IMessageConnection> accept(std::chrono::milliseconds timeout) override;
    std::shared_ptr<const Endpoint> localEndpoint() const override;
    
private:
    std::shared_ptr<NewLoopbackConnectionSpawner> spawner_;
    std::shared_ptr<const Endpoint> endpoint_;
};
}
}