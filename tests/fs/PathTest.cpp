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

#include "mocca/fs/Filesystem.h"

using namespace mocca::fs;

class PathTest : public ::testing::Test {
protected:
    PathTest() {
        // You can do set-up work for each test here.
    }

    virtual ~PathTest() {
        // You can do clean-up work that doesn't throw exceptions here.
    }
};

TEST_F(PathTest, Decompose) {
    Path path("C:\\mocca\\src\\fs\\Path.h");
    ASSERT_EQ("C:/mocca/src/fs", path.directory());
    ASSERT_EQ("Path.h", path.filename());
    ASSERT_EQ("Path", path.filenameNoExt());
    ASSERT_EQ(".h", path.extension());
}

TEST_F(PathTest, Parent) {
    Path path("C:\\mocca\\src\\fs\\Path.h");
    ASSERT_EQ(Path("C:\\mocca\\src\\fs"), path.parent());
    ASSERT_EQ(Path("C:\\mocca\\src"), path.parent().parent());
    ASSERT_EQ(Path("C:\\mocca"), path.parent().parent().parent());
    ASSERT_EQ(Path("C:"), path.parent().parent().parent().parent());
    ASSERT_EQ(Path(), path.parent().parent().parent().parent().parent());
}

TEST_F(PathTest, Append) {
    Path path1("C:\\mocca\\src\\fs");
    Path path2("C:\\mocca\\src\\fs\\");
    ASSERT_EQ(Path("C:\\mocca\\src\\fs\\Path.h"), path1 + "Path.h");
    ASSERT_EQ(Path("C:\\mocca\\src\\fs\\Path.h"), path2 + "Path.h");
}