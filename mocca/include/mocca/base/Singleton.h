#pragma once

namespace mocca {

template <class T> class Singleton {
public:
    static T* instance();

private:
    Singleton(Singleton const&) {}
    Singleton& operator=(Singleton const&) { return *this; }

protected:
    static T* instance_;

    Singleton() { instance_ = static_cast<T*>(this); };

    ~Singleton() {}
};

template <class T> T* Singleton<T>::instance_ = nullptr;

template <class T> T* Singleton<T>::instance() {
    if (!instance_) {
        Singleton<T>::instance_ = new T();
    }
    return instance_;
}
}