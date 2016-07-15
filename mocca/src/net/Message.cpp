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

#include "mocca/net/Message.h"

#include "mocca/base/Memory.h"

using namespace mocca::net;

MessageBuf::MessageBuf(std::vector<uint8_t>& vec)
    : m_vec(vec) {
    vec.clear();
    auto casted = reinterpret_cast<char*>(vec.data());
    setp(casted, casted);
}

int MessageBuf::overflow(int ch) {
    if (ch != traits_type::eof()) {
        m_vec.push_back(ch);
        pbump(1);
        return ch;
    }
    return traits_type::eof();
}

std::streamsize MessageBuf::xsputn(const char_type* s, std::streamsize count) {
    m_vec.insert(end(m_vec), s, s + count);
    return count;
}


MessagePart mocca::net::createMessagePart(const std::string& str) {
    MessagePart part = std::make_shared<std::vector<uint8_t>>();
    part->assign(begin(str), end(str));
    return part;
}

std::string mocca::net::readMessagePart(const std::vector<uint8_t>& part) {
    return std::string(reinterpret_cast<const char*>(part.data()), part.size());
}

mocca::net::MessagePart mocca::net::createMessagePartFromJson(const JsonCpp::Value& json) {
    auto messagePart = std::make_shared<std::vector<uint8_t>>();
    MessageBuf buf(*messagePart);
    std::ostream os(&buf);
    JsonCpp::StyledStreamWriter streamWriter;
    streamWriter.write(os, json);
    return messagePart;
}
