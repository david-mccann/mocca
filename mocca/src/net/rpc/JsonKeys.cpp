#include "mocca/net/rpc/JsonKeys.h"

const std::string& mocca::net::methodKey() {
    static std::string key = "method";
    return key;
}

const std::string& mocca::net::paramsKey() {
    static std::string key = "params";
    return key;
}

const std::string& mocca::net::statusKey() {
    static std::string key = "status";
    return key;
}

const std::string& mocca::net::errorKey() {
    static std::string key = "error";
    return key;
}

const std::string& mocca::net::resultKey() {
    static std::string key = "result";
    return key;
}

const std::string& mocca::net::successStatus() {
    static std::string key = "success";
    return key;
}

const std::string& mocca::net::errorStatus() {
    static std::string key = "error";
    return key;
}

const std::string& mocca::net::describe() {
    static std::string key = "mocca::net::describe";
    return key;
}