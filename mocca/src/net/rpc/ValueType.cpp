#include "mocca/net/rpc/ValueType.h"

const mocca::net::ValueTypeMapper& mocca::net::valueTypeMapper() {
    static mocca::net::ValueTypeMapper mapper;
    if (mapper.empty()) {
        mapper.insert(JsonCpp::ValueType::intValue, "int");
        mapper.insert(JsonCpp::ValueType::realValue, "double");
        mapper.insert(JsonCpp::ValueType::stringValue, "string");
        mapper.insert(JsonCpp::ValueType::booleanValue, "bool");
    }
    return mapper;
}