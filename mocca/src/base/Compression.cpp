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

#include "mocca/base/Compression.h"

#include "mocca/base/BinaryUtil.h"

#include "lz4/lz4.h"

#include <cassert>

std::unique_ptr<std::vector<uint8_t>> mocca::compressData(const std::vector<uint8_t>& data) {
    int32_t maxCompressSize = LZ4_compressBound(data.size());
    auto buffer = std::make_unique<std::vector<uint8_t>>(maxCompressSize + sizeof(int32_t));
    binaryWrite(buffer->data(), static_cast<int32_t>(data.size()));
    int32_t compressSize = LZ4_compress_default(reinterpret_cast<const char*>(data.data()),
                                                reinterpret_cast<char*>(buffer->data() + sizeof(int32_t)), data.size(), maxCompressSize);
    assert(compressSize > 0 && compressSize <= maxCompressSize);
    buffer->resize(compressSize + sizeof(int32_t));
    return buffer;
}

std::unique_ptr<std::vector<uint8_t>> mocca::uncompressData(const std::vector<uint8_t>& data) {
    int32_t uncompressedSize;
    binaryRead(data.data(), uncompressedSize);
    auto buffer = std::make_unique<std::vector<uint8_t>>(uncompressedSize);
    int result = LZ4_decompress_safe(reinterpret_cast<const char*>(data.data() + sizeof(int32_t)), reinterpret_cast<char*>(buffer->data()),
                                     data.size() - sizeof(int32_t), uncompressedSize);
    assert(result == uncompressedSize);
    return buffer;
}
