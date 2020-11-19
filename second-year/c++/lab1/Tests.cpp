#include <gtest/gtest.h>
#include <iostream>
#include "TritSet.h"

TEST (copyOperator, cwerawr){
    TritSet s1;
    TritSet s2(123);
    s2[12] = Trit::False;
    s1 = s2;
    s2[12] = Trit::True;
    ASSERT_EQ(s1[12], Trit::False);
}

TEST(MemoryAllocationTest, ConstructorAllocationTest) {
    for (int i = 0; i <= 1000; ++i) {
        TritSet set(i);
        size_t allocLength = set.capacity();
        ASSERT_GE(allocLength, i * 2 / 8 / sizeof(uint));
    }
}

TEST(MemoryAllocationTest, TritMemoryUnknown){
    TritSet set(6);
    set[0] = Trit::True;
    ASSERT_EQ(set[6], Trit::Unknown);
}

TEST(MemoryAllocationTest, TritMemory) {
    TritSet set(6);
    set[0] = Trit::False;
    set[1] = Trit::Unknown;
    set[2] = Trit::True;
    set[3] = Trit::True;
    set[4] = Trit::False;
    set[5] = Trit::False;
    ASSERT_EQ(set[0], Trit::False);
    ASSERT_EQ(set[1], Trit::Unknown);
    ASSERT_EQ(set[2], Trit::True);
    ASSERT_EQ(set[3], Trit::True);
    ASSERT_EQ(set[4], Trit::False);
    ASSERT_EQ(set[5], Trit::False);
}

TEST(MemoryAllocationTest, UnknownOutOfBound) {
    TritSet set(1000);
    size_t allocLength = set.capacity();
    set[1000000000] = Trit::Unknown;
    ASSERT_EQ(allocLength, set.capacity());
}

TEST(MemoryAllocationTest, ComparingOutOfBound){
    TritSet set(1000);
    size_t allocLength = set.capacity();
    ASSERT_FALSE(set[2000000000] == Trit::True);
    ASSERT_EQ(allocLength, set.capacity());
}

TEST(MemoryAllocationTest, OutOfBoundAllocation) {
    TritSet set(1000);
    size_t allocLength = set.capacity();
    set[1000000000] = Trit::True;
    ASSERT_LT(allocLength, set.capacity());
}

TEST(MemoryAllocationTest, ShrinkMemoryFree) {
    TritSet set(1000);
    set[100100] = Trit::False;
    set[100100] = Trit::Unknown;
    uLL secondBlockEndIdx = 31;//два полных блока тритов
    set[secondBlockEndIdx] = Trit::True;
    set.shrink();
    uLL expectedCapacity = ceil((2 * (secondBlockEndIdx + 1)) / (8.0 * sizeof(uint)));
    ASSERT_EQ(expectedCapacity, set.capacity());
    ASSERT_EQ(secondBlockEndIdx + 1, set.size());
}

TEST(MemoryAllocationTest, tritOperationAllocation) {
    TritSet a(1000);
    TritSet b(2000);
    TritSet c = a | b;
    ASSERT_EQ(b.capacity(), c.capacity());
}

TEST(TritSetOperationsTest, OrTest) {
    TritSet a(3);
    TritSet b(3);
    a[0] = Trit::Unknown;
    a[1] = Trit::Unknown;
    a[2] = Trit::Unknown;
    b[0] = Trit::True;
    b[1] = Trit::False;
    b[2] = Trit::Unknown;
    TritSet c = a | b;
    ASSERT_EQ(c[0], Trit::True);
    ASSERT_EQ(c[1], Trit::Unknown);
    ASSERT_EQ(c[2], Trit::Unknown);
}

TEST(TritSetOperationsTest, AndTest) {
    TritSet a(3);
    TritSet b(3);
    a[0] = Trit::Unknown;
    a[1] = Trit::Unknown;
    a[2] = Trit::Unknown;
    b[0] = Trit::True;
    b[1] = Trit::False;
    b[2] = Trit::Unknown;
    TritSet c = a & b;
    ASSERT_EQ(c[0], Trit::Unknown);
    ASSERT_EQ(c[1], Trit::False);
    ASSERT_EQ(c[2], Trit::Unknown);
}

TEST(TritSetOperationsTest, NotTest) {
    TritSet a(3);
    a[0] = Trit::True;
    a[1] = Trit::False;
    a[2] = Trit::Unknown;
    TritSet b = !a;
    ASSERT_EQ(b[0], Trit::False);
    ASSERT_EQ(b[1], Trit::True);
    ASSERT_EQ(b[2], Trit::Unknown);
}

TEST(TritSetOperationsTest, assignmentTest) {
    TritSet a(3);
    a[0] = Trit::False;
    a[1] = Trit::Unknown;
    a[2] = Trit::True;
    ASSERT_EQ(a[0], Trit::False);
    ASSERT_EQ(a[1], Trit::Unknown);
    ASSERT_EQ(a[2], Trit::True);
}

TEST(TritSetOperationsTest, assignmentOrTest) {
    TritSet a(3);
    a[0] |= Trit::False;
    a[1] |= Trit::Unknown;
    a[2] |= Trit::True;
    ASSERT_EQ(a[0], Trit::Unknown);
    ASSERT_EQ(a[1], Trit::Unknown);
    ASSERT_EQ(a[2], Trit::True);
}

TEST(TritSetOperationsTest, assignmentAndTest) {
    TritSet a(3);
    a[0] &= Trit::False;
    a[1] &= Trit::Unknown;
    a[2] &= Trit::True;
    ASSERT_EQ(a[0], Trit::False);
    ASSERT_EQ(a[1], Trit::Unknown);
    ASSERT_EQ(a[2], Trit::Unknown);
}

TEST(TritSetOperationsTest, tritInSetAccessTest) {
    TritSet a(3);
    a[0] = Trit::False;
    a[1] = Trit::Unknown;
    a[2] = Trit::True;
    Trit e0 = a[0];
    Trit e1 = a[1];
    Trit e2 = a[2];
    ASSERT_EQ(e0, Trit::False);
    ASSERT_EQ(e1, Trit::Unknown);
    ASSERT_EQ(e2, Trit::True);
}

TEST(TritSetFunctionsTests, CardinalityTest) {
    TritSet set(3);
    set[0] = Trit::False;
    set[1] = Trit::Unknown;
    set[2] = Trit::True;
    ASSERT_EQ(set.cardinality(Trit::False), 1);
    ASSERT_EQ(set.cardinality(Trit::Unknown), 1);
    ASSERT_EQ(set.cardinality(Trit::True), 1);
}

TEST(TritSetFunctionsTests, cardinalityMapTest) {
    TritSet set(11);
    set[0] = Trit::True;
    set[1] = Trit::Unknown;
    set[2] = Trit::False;
    set[3] = Trit::False;
    set[4] = Trit::Unknown;
    set[5] = Trit::False;
    set[6] = Trit::Unknown;
    set[7] = Trit::Unknown;
    set[8] = Trit::Unknown;
    set[9] = Trit::Unknown;
    set[10] = Trit::Unknown;
    auto cardinalityMap = set.cardinality();
    ASSERT_EQ(cardinalityMap[Trit::False], 3);
    ASSERT_EQ(cardinalityMap[Trit::Unknown], 2);
    ASSERT_EQ(cardinalityMap[Trit::True], 1);
}

TEST(TritSetFunctionsTests, trimTest) {
    TritSet set(7);
    set[0] = Trit::True;
    set[1] = Trit::True;
    set[2] = Trit::False;
    set[3] = Trit::False;
    set[4] = Trit::Unknown;
    set[5] = Trit::True;
    set[6] = Trit::Unknown;
    set.trim(2);
    ASSERT_EQ(set[0], Trit::True);
    ASSERT_EQ(set[1], Trit::True);
    ASSERT_EQ(set[2], Trit::Unknown);
    ASSERT_EQ(set[3], Trit::Unknown);
    ASSERT_EQ(set[4], Trit::Unknown);
    ASSERT_EQ(set[5], Trit::Unknown);
    ASSERT_EQ(set[6], Trit::Unknown);
}

TEST(TritSetFunctionsTests, lengthTest) {
    TritSet set(4);
    set[0] = Trit::Unknown;
    set[1] = Trit::Unknown;
    set[2] = Trit::False;
    set[3] = Trit::Unknown;
    ASSERT_EQ(set.length(), 3);
}
