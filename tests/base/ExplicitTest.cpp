/****************************************************************
* Copyright (C) 2016 Andrey Krekhov, David McCann
*
* The content of this file may not be copied and/or distributed
* without the expressed permission of the copyright owner.
*
****************************************************************/

#include "gtest/gtest.h"

#include "mocca/base/Explicit.h"

using namespace mocca;

class ExplicitTest : public ::testing::Test {
protected:
    ExplicitTest() {
        // You can do set-up work for each test here.
    }

    virtual ~ExplicitTest() {
        // You can do clean-up work that doesn't throw exceptions here.
    }
};

TEST_F(ExplicitTest, Ctor) {
    struct Phantom;
    using PhantomID = Explicit<int, Phantom>;
    {
        PhantomID id{ 17 };
        ASSERT_EQ(17, id);
    }
    {
        PhantomID id{ 17 };
        PhantomID id2{ id };
        ASSERT_EQ(17, id2);
    }
}

TEST_F(ExplicitTest, Assignment) {
    struct Phantom;
    using PhantomID = Explicit<int, Phantom>;
    {
        PhantomID id{ 17 };
        id = 23;
        ASSERT_EQ(23, id);
    }
    {
        PhantomID id{ 17 };
        PhantomID id2{ 23 };
        id = id2;
        ASSERT_EQ(23, id);
    }
}

TEST_F(ExplicitTest, StringComparison) {
    struct Phantom;
    using PhantomID = Explicit<std::string, Phantom>;
    PhantomID id{ "id" };
    ASSERT_EQ("id", id);
    ASSERT_EQ(id, "id");
}