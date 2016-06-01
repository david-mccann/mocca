/*The MIT License(MIT)

Copyright(c) 2016 David McCann

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files(the
"Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and / or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to
the following conditions :

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.*/

#pragma once

#include "jsoncpp/json.h"

#include <string>
#include <vector>


namespace mocca {
namespace net {

class MethodDescription {
public:
    class ParameterDescription {
    public:
        ParameterDescription(const std::string name, JsonCpp::ValueType type, const JsonCpp::Value& defaultValue);

        std::string name() const;
        JsonCpp::ValueType type() const;
        JsonCpp::Value defaultValue() const;

        std::string toString() const;

        bool operator<(const ParameterDescription& other) const;

    private:
        std::string name_;
        JsonCpp::ValueType type_;
        JsonCpp::Value defaultValue_;
    };

    static JsonCpp::Value toJson(const MethodDescription& description);
    static MethodDescription fromJson(const JsonCpp::Value& json);

    MethodDescription(const std::string& name, const std::vector<ParameterDescription>& parameters);

    std::string name() const;
    std::vector<ParameterDescription> parameters() const;

    std::string toString() const;

private:
    std::string name_;
    std::vector<ParameterDescription> parameters_;
};
}
}

std::ostream& operator<<(std::ostream& os, const mocca::net::MethodDescription::ParameterDescription& obj);
std::ostream& operator<<(std::ostream& os, const mocca::net::MethodDescription& obj);