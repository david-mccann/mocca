/****************************************************************
* Copyright (C) 2016 Andrey Krekhov, David McCann
*
* The content of this file may not be copied and/or distributed
* without the expressed permission of the copyright owner.
*
****************************************************************/

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