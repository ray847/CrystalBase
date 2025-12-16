#include <gtest/gtest.h>
#include "CrystalBase/stable_vector.h"
#include <vector>
#include <memory_resource>

TEST(StableVectorTest, BasicPushAndAccess) {
    crystal::StableVector<int> sv;
    
    size_t idx0 = sv.PushBack(100);
    size_t idx1 = sv.PushBack(200);

    EXPECT_EQ(idx0, 0);
    EXPECT_EQ(idx1, 1);
    
    EXPECT_EQ(sv[idx0], 100);
    EXPECT_EQ(sv.At(idx1), 200);
    
    // Test const access
    const auto& csv = sv;
    EXPECT_EQ(csv[idx0], 100);
    EXPECT_EQ(csv.At(idx1), 200);
}

TEST(StableVectorTest, EraseAndReuse) {
    crystal::StableVector<int> sv;
    
    size_t id0 = sv.PushBack(0); // 0
    size_t id1 = sv.PushBack(1); // 1
    size_t id2 = sv.PushBack(2); // 2
    
    EXPECT_EQ(id0, 0);
    EXPECT_EQ(id1, 1);
    EXPECT_EQ(id2, 2);

    sv.Erase(id1); // Erase index 1. Free head should now be 1.

    // Insert should reuse index 1
    size_t id3 = sv.Insert(3);
    EXPECT_EQ(id3, 1);
    EXPECT_EQ(sv[id3], 3);

    // Next insert should append since free list is empty (assuming no more erasures)
    size_t id4 = sv.Insert(4);
    EXPECT_EQ(id4, 3);
    EXPECT_EQ(sv[id4], 4);
}

TEST(StableVectorTest, MultipleErasures) {
    crystal::StableVector<int> sv;
    // 0, 1, 2, 3
    (void)sv.PushBack(10);
    (void)sv.PushBack(20);
    (void)sv.PushBack(30);
    (void)sv.PushBack(40);

    sv.Erase(0);
    sv.Erase(2);
    // Free list should likely look like 2 -> 0 -> null (LIFO usually for simple linked list)
    // Code: arr_[idx].nxt = free_head_; free_head_ = idx;
    // 1. Erase(0): node[0].nxt = null; head = 0
    // 2. Erase(2): node[2].nxt = 0; head = 2
    
    size_t new1 = sv.Insert(50);
    EXPECT_EQ(new1, 2);
    EXPECT_EQ(sv[new1], 50);

    size_t new2 = sv.Insert(60);
    EXPECT_EQ(new2, 0);
    EXPECT_EQ(sv[new2], 60);

    size_t new3 = sv.Insert(70);
    EXPECT_EQ(new3, 4);
}

TEST(StableVectorTest, Emplace) {
    struct Point {
        int x, y;
        Point(int x, int y) : x(x), y(y) {}
    };
    
    crystal::StableVector<Point> sv;
    size_t idx = sv.EmplaceBack(1, 2);
    EXPECT_EQ(sv[idx].x, 1);
    EXPECT_EQ(sv[idx].y, 2);
    
    sv.Erase(idx);
    size_t idx2 = sv.Emplace(3, 4);
    EXPECT_EQ(idx2, idx);
    EXPECT_EQ(sv[idx2].x, 3);
    EXPECT_EQ(sv[idx2].y, 4);
}

TEST(StableVectorTest, Clear) {
    crystal::StableVector<int> sv;
    (void)sv.PushBack(1);
    (void)sv.PushBack(2);
    sv.Erase(0);
    
    sv.Clear();
    // Should be empty and reset
    size_t idx = sv.PushBack(3);
    EXPECT_EQ(idx, 0);
}

TEST(StableVectorTest, CopyConstructor) {
    crystal::StableVector<int> sv1;
    (void)sv1.PushBack(10);
    sv1.Erase(0);
    (void)sv1.PushBack(20); // index 1. Index 0 is free.

    // sv1: [free, 20]
    
    crystal::StableVector<int> sv2 = sv1;
    // sv2 should have index 0 free, index 1 = 20.
    
    size_t idx = sv2.Insert(30); // Should use index 0
    EXPECT_EQ(idx, 0);
    EXPECT_EQ(sv2[idx], 30);
    EXPECT_EQ(sv2[1], 20);
}

TEST(StableVectorTest, MoveConstructor) {
    crystal::StableVector<int> sv1;
    (void)sv1.PushBack(10);
    sv1.Erase(0);
    
    crystal::StableVector<int> sv2(std::move(sv1));
    
    // sv2 should have index 0 free
    size_t idx = sv2.Insert(20);
    EXPECT_EQ(idx, 0);
    EXPECT_EQ(sv2[idx], 20);
}

TEST(StableVectorTest, IteratorConstructor) {
    std::vector<int> input = {1, 2, 3};
    crystal::StableVector<int> sv(input.begin(), input.end());
    
    EXPECT_EQ(sv[0], 1);
    EXPECT_EQ(sv[1], 2);
    EXPECT_EQ(sv[2], 3);
}

TEST(StableVectorTest, InitializerListConstructor) {
    crystal::StableVector<int> sv = {10, 20, 30};
    EXPECT_EQ(sv[0], 10);
    EXPECT_EQ(sv[1], 20);
    EXPECT_EQ(sv[2], 30);
}

// PMR Test Section

class TestMemoryResource : public std::pmr::memory_resource {
public:
    size_t allocated_bytes = 0;
    size_t deallocated_bytes = 0;
    size_t num_allocations = 0;
    size_t num_deallocations = 0;

private:
    void* do_allocate(size_t bytes, size_t alignment) override {
        allocated_bytes += bytes;
        num_allocations++;
        return std::pmr::new_delete_resource()->allocate(bytes, alignment);
    }
    void do_deallocate(void* p, size_t bytes, size_t alignment) override {
        deallocated_bytes += bytes;
        num_deallocations++;
        std::pmr::new_delete_resource()->deallocate(p, bytes, alignment);
    }
    bool do_is_equal(const std::pmr::memory_resource& other) const noexcept override {
        return this == &other;
    }
};

TEST(StableVectorTest, PMRAllocatorUsage) {
    TestMemoryResource res;
    
    {
        // Construct with the custom resource
        crystal::pmr::StableVector<int> sv(&res);
        
        // Initial state
        EXPECT_EQ(res.allocated_bytes, 0);
        EXPECT_EQ(res.num_allocations, 0);
        
        // Pushing elements should trigger allocation
        (void)sv.PushBack(1);
        (void)sv.PushBack(2);
        
        // Check that allocations happened through our resource
        EXPECT_GT(res.allocated_bytes, 0);
        EXPECT_GT(res.num_allocations, 0);
        
        size_t current_alloc_bytes = res.allocated_bytes;
        
        // Force vector resize/reallocation
        for (int i = 0; i < 100; ++i) {
            (void)sv.PushBack(i);
        }
        
        EXPECT_GT(res.allocated_bytes, current_alloc_bytes);
    }
    
    // After destruction, deallocations should have happened
    // Note: deallocated_bytes might be less than allocated if vector capacity growth logic
    // resulted in intermediate deallocations. But we expect at least some deallocation.
    EXPECT_GT(res.deallocated_bytes, 0);
    EXPECT_GT(res.num_deallocations, 0);
}