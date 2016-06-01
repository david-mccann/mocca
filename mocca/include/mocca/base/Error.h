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

#include <sstream>
#include <stdexcept>
#include <string>

// VS2013 doesn't support the 'noexcept' directive
#ifndef _MSC_VER
#define NOEXCEPT noexcept
#else
#define NOEXCEPT
#endif

#define CODE_LOCATION_IN_ERROR

namespace mocca {

class Error : public std::runtime_error {
public:
    Error(const std::string& msg, const std::string& file, int line)
        : std::runtime_error("")
        , msg_(msg)
        , file_(file)
        , line_(line) {
        std::stringstream str;
        str << msg_;
        std::string internalMsg = internalError();
        if (!internalMsg.empty())
            str << " (internal error: " << internalMsg << ")";
#ifdef CODE_LOCATION_IN_ERROR
        if (file_.empty() && line_ == -1)
            str << " (" << file_ << ":" << std::to_string(line_) << ")";
#endif
        msg_ = str.str();
    }

    const char* what() const NOEXCEPT override { return msg_.c_str(); }

protected:
    virtual std::string internalError() const { return std::string(); }

private:
    std::string msg_;
    std::string file_;
    int line_;
};
}