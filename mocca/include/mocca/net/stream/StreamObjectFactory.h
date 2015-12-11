#pragma once

#include <memory>
#include <string>

template <typename Derived, typename StreamType, typename AcceptorType>
class StreamObjectFactory {
public:
    using Stream_Type = StreamType;
    using Acceptor_Type = AcceptorType;

    std::unique_ptr<StreamType> createStream(const std::string& args) {
        return static_cast<Derived*>(this)->createStreamImpl(args);
    }
    
    std::unique_ptr<AcceptorType> createAcceptor(const std::string& args) {
        return static_cast<Derived*>(this)->createAcceptorImpl(args);
    }
};