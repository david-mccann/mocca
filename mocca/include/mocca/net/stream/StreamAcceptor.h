#pragma once

#include <memory>

namespace mocca {
namespace net {

template <typename Derived, typename StreamType> class StreamAcceptor {
public:
    using Stream_Type = StreamType;
    std::unique_ptr<StreamType> accept(std::chrono::milliseconds timeout) {
        return static_cast<Derived*>(this)->acceptImpl(timeout);
    }
};
}
}