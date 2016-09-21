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