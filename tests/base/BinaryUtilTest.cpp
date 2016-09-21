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

#include "gtest/gtest.h"

#include "mocca/base/BinaryUtil.h"

using namespace mocca;

class BinaryUtilTest : public ::testing::Test {
protected:
    BinaryUtilTest() {
        // You can do set-up work for each test here.
    }

    virtual ~BinaryUtilTest() {
        // You can do clean-up work that doesn't throw exceptions here.
    }
};

TEST_F(BinaryUtilTest, ReadWrite) {
    std::vector<uint8_t> buffer(sizeof(int) + sizeof(double) + sizeof(bool));
    uint8_t* writePtr = buffer.data();
    
    writePtr = binaryWrite(writePtr, 42);
    writePtr = binaryWrite(writePtr, 3.14);
    writePtr = binaryWrite(writePtr, true);

    const uint8_t* readPtr = buffer.data();
    int intVal;
    readPtr = binaryRead(readPtr, intVal);
    ASSERT_EQ(42, intVal);

    double doubleVal;
    readPtr = binaryRead(readPtr, doubleVal);
    ASSERT_EQ(3.14, doubleVal);

    bool boolVal;
    readPtr = binaryRead(readPtr, boolVal);
    ASSERT_EQ(true, boolVal);
}