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

#include "mocca/base/BidirectionalMap.h"

using namespace mocca;

class BidirectionalMapTest : public ::testing::Test {
protected:
	BidirectionalMapTest() {
		// You can do set-up work for each test here.
	}

	virtual ~BidirectionalMapTest() {
		// You can do clean-up work that doesn't throw exceptions here.
	}
};

TEST_F(BidirectionalMapTest, Size) {
	BidirectionalMap<int, std::string> target;
	ASSERT_EQ(0, target.size());
	target.insert(42, "meaning of life");
	ASSERT_EQ(1, target.size());
	target.insert(666, "number of the beast");
	ASSERT_EQ(2, target.size());
}

TEST_F(BidirectionalMapTest, Empty) {
	BidirectionalMap<int, std::string> target;
	ASSERT_TRUE(target.empty());
	target.insert(42, "meaning of life");
	ASSERT_FALSE(target.empty());
}

TEST_F(BidirectionalMapTest, InsertReplace) {
	BidirectionalMap<int, std::string> target;
	target.insert(42, "meaning of life");
	target.insert(42, "meaningless number");
	ASSERT_EQ(1, target.size());
	ASSERT_EQ("meaningless number", target.getByFirst(42));
}

TEST_F(BidirectionalMapTest, GetByFirst) {
	BidirectionalMap<int, std::string> target;
	target.insert(42, "meaning of life");
	target.insert(666, "number of the beast");
	ASSERT_EQ("meaning of life", target.getByFirst(42));
	ASSERT_EQ("number of the beast", target.getByFirst(666));
}
TEST_F(BidirectionalMapTest, GetByFirstNotFound) {
	BidirectionalMap<int, std::string> target;
	target.insert(42, "meaning of life");
	ASSERT_THROW(target.getByFirst(43), Error);
}

TEST_F(BidirectionalMapTest, GetBySecond) {
	BidirectionalMap<int, std::string> target;
	target.insert(42, "meaning of life");
	target.insert(666, "number of the beast");
	ASSERT_EQ(42, target.getBySecond("meaning of life"));
	ASSERT_EQ(666, target.getBySecond("number of the beast"));
}

TEST_F(BidirectionalMapTest, GetBySecondNotFound) {
	BidirectionalMap<int, std::string> target;
	target.insert(42, "meaning of life");
	ASSERT_THROW(target.getBySecond("meaningless number"), Error);
}

TEST_F(BidirectionalMapTest, SubscriptInsert) {
	BidirectionalMap<int, std::string> target;
	target[42] = "meaning of life";
	ASSERT_EQ(1, target.size());
	ASSERT_EQ("meaning of life", target.getByFirst(42));
	ASSERT_EQ(42, target.getBySecond("meaning of life"));
}

TEST_F(BidirectionalMapTest, SubscriptReplace) {
	BidirectionalMap<int, std::string> target;
	target[42] = "meaning of life";
	target[42] = "meaningless number";
	ASSERT_EQ(1, target.size());
	ASSERT_EQ("meaningless number", target.getByFirst(42));
}

TEST_F(BidirectionalMapTest, SubscriptRead) {
	BidirectionalMap<int, std::string> target;
	ASSERT_EQ(std::string(), target[42]);
	target[42] = "meaning of life";
	ASSERT_EQ("meaning of life", target[42]);
}
