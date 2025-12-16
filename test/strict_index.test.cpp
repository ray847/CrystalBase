#include <gtest/gtest.h>
#include <unordered_set>
#include "CrystalBase/strict_index.h"

namespace {

struct TagA {};
struct TagB {};

using IndexA = crystal::StrictIdx<TagA>;
using IndexB = crystal::StrictIdx<TagB, uint32_t>;

} // namespace

TEST(StrictIdxTest, ConstructionAndDefaults) {
    // Default constructor sets to nullvalue (max)
    IndexA idx1;
    EXPECT_EQ(idx1.get(), std::numeric_limits<size_t>::max());
    
    // operator bool returns true if value == nullvalue
    EXPECT_TRUE(static_cast<bool>(idx1));

    // Value constructor
    IndexA idx2(10);
    EXPECT_EQ(idx2.get(), 10);
    // operator bool returns false if value != nullvalue
    EXPECT_FALSE(static_cast<bool>(idx2));
}

TEST(StrictIdxTest, Comparison) {
    IndexA idx1(5);
    IndexA idx2(5);
    IndexA idx3(10);

    EXPECT_EQ(idx1, idx2);
    EXPECT_NE(idx1, idx3);
    EXPECT_LT(idx1, idx3);
    EXPECT_LE(idx1, idx2);
    EXPECT_GT(idx3, idx1);
    EXPECT_GE(idx3, idx1);
}

TEST(StrictIdxTest, Increment) {
    IndexA idx(5);
    
    // Pre-increment
    IndexA& ref = ++idx;
    EXPECT_EQ(idx.get(), 6);
    EXPECT_EQ(ref.get(), 6);
    EXPECT_EQ(&ref, &idx);

    // Post-increment
    IndexA old = idx++;
    EXPECT_EQ(idx.get(), 7);
    EXPECT_EQ(old.get(), 6);
}

TEST(StrictIdxTest, TypeConversion) {
    IndexB idx(42);
    uint32_t val = static_cast<uint32_t>(idx);
    EXPECT_EQ(val, 42);
}

TEST(StrictIdxTest, Hashing) {
    std::unordered_set<IndexA> set;
    IndexA idx1(1);
    IndexA idx2(2);
    IndexA idx3(1); // duplicate

    set.insert(idx1);
    set.insert(idx2);
    
    EXPECT_EQ(set.size(), 2);
    EXPECT_NE(set.find(idx1), set.end());
    EXPECT_NE(set.find(idx2), set.end());

    set.insert(idx3);
    EXPECT_EQ(set.size(), 2);
}
