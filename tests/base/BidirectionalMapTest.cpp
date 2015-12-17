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
