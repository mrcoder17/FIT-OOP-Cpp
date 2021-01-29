#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "CircularBuffer.h"

using testing::Eq;

namespace { class CircularBufferTest : public testing::Test {}; }

TEST_F(CircularBufferTest, basic_tests) {
    //Check empty constructor
    CircularBuffer buf;
    EXPECT_EQ(buf.size(), 0);
    EXPECT_TRUE(buf.empty());

    //Check constructor with capacity
    CircularBuffer buf2(20);
    EXPECT_EQ(buf2.capacity(), 20);
    EXPECT_TRUE(buf2.empty());

    //Check constructor with capacity and element
    CircularBuffer buf3(10, 'b');
    EXPECT_EQ(buf3.size(), 10);
    EXPECT_TRUE(buf3.full());
    EXPECT_FALSE(buf3.empty());

    //Check copy-constructor
    CircularBuffer buf4(buf3);
    EXPECT_TRUE(buf3 == buf4);
}

TEST_F(CircularBufferTest, test_at) {
    CircularBuffer buf(10, 'a');
    EXPECT_EQ(buf.at(5), 'a'); //check element
    EXPECT_ANY_THROW(buf.at(12)); //check exception when out of range
    EXPECT_EQ(buf[2], 'a'); //check []
    EXPECT_EQ(buf[12], 'a'); //check [] with out of range index

    //check with const buffer
    const CircularBuffer buf1(10, 'a');
    EXPECT_EQ(buf1.at(5), 'a'); //check element
    EXPECT_ANY_THROW(buf.at(12)); //check exception when out of range
    EXPECT_EQ(buf1[2], 'a'); //check []
    EXPECT_EQ(buf1[12], 'a'); //check [] with out of range index
}

TEST_F(CircularBufferTest, test_front_back) {
    CircularBuffer buf(10);
    buf.push_back('a');
    buf.push_back('b');
    buf.push_back('c');
    EXPECT_EQ(buf.front(), 'a');
    EXPECT_EQ(buf.back(), 'c');
    buf.push_front('0');
    EXPECT_EQ(buf.front(), '0');
    buf.pop_back();
    EXPECT_EQ(buf.back(), 'b');

    //const buffer
    const CircularBuffer buf1(10, 'a');
    EXPECT_EQ(buf1.front(), 'a');
    EXPECT_EQ(buf1.back(), 'a');
}

TEST_F(CircularBufferTest, test_erase_clear) {
    CircularBuffer buf(10);
    buf.push_back('a');
    buf.push_back('b');
    buf.push_back('c');
    buf.push_back('d');
    buf.push_back('e');
    buf.push_back('f');
    buf.erase(1, 4);
    EXPECT_EQ(buf.at(0), 'a');
    EXPECT_EQ(buf.at(1), 'e');
    EXPECT_EQ(buf.at(2), 'f');

    buf.clear();

    EXPECT_EQ(buf.capacity(), 10);
    EXPECT_EQ(buf.size(), 0);
    EXPECT_EQ(buf.reserve(), 10);
}


TEST_F(CircularBufferTest, test_linearize) {
    CircularBuffer buf(10);
    buf.push_back('a');
    buf.push_back('b');
    buf.push_back('c');
    buf.push_back('d');
    buf.push_back('e');
    buf.push_back('f');
    EXPECT_FALSE(buf.is_linearized());
    buf.linearize();
    EXPECT_TRUE(buf.is_linearized());

    CircularBuffer buf1(10);
    buf1.push_back('a');
    buf1.push_back('b');
    buf1.push_back('c');
    buf1.push_back('d');
    buf1.push_back('e');
    buf1.push_back('f');
    buf1.push_front('0');
    EXPECT_TRUE(buf1.is_linearized());
}

TEST_F(CircularBufferTest, test_rotate) {
    CircularBuffer buf(10);
    buf.push_back('a');
    buf.push_back('b');
    buf.push_back('c');
    buf.push_back('d');
    buf.push_back('e');
    buf.push_back('f');
    buf.rotate(2);
    EXPECT_EQ(buf.at(0), 'c');
    EXPECT_EQ(buf.at(4), 'a');
    EXPECT_EQ(buf.at(5), 'b');
}

TEST_F(CircularBufferTest, test_set_capacity) {
    CircularBuffer buf(10);
    buf.push_back('a');
    buf.push_back('b');
    buf.push_back('c');
    buf.push_back('d');
    buf.push_back('e');
    buf.push_back('f');

    buf.set_capacity(5);
    //capacity became smaller, element 'a' was pushed out so there shouldn't be 'a' in buf
    for (int i = 0; i < 5; i++) {
        EXPECT_NE(buf.at(i), 'a');
    }

    buf.set_capacity(4);
    //capacity became smaller, element 'b' was pushed out so there shouldn't be 'b' in buf
    for (int i = 0; i < 4; i++) {
        EXPECT_NE(buf.at(i), 'b');
    }
}

TEST_F(CircularBufferTest, test_resize) {
    CircularBuffer buf(10);
    buf.push_back('a');
    buf.push_back('b');
    buf.push_back('c');
    buf.push_back('d');
    buf.push_back('e');
    buf.push_back('f');
    buf.resize(8, '0');
    EXPECT_EQ(buf.at(6), '0');
    EXPECT_EQ(buf.at(7), '0');

    buf.resize(7, '0');
    //size became smaller, element 'a' was pushed out
    for (int i = 0; i < 7; i++) {
        EXPECT_NE(buf.at(i), 'a');
    }
}

TEST_F(CircularBufferTest, test_swap) {
    CircularBuffer buf1(5);
    for (char c = 'a'; c <= 'd'; c++) buf1.push_back(c);

    CircularBuffer buf2(10);
    for (char c = '1'; c <= '8'; c++) buf2.push_back(c);

    buf1.swap(buf2);

    EXPECT_EQ(buf1.capacity(), 10);
    EXPECT_EQ(buf2.capacity(), 5);

    EXPECT_EQ(buf1.size(), 8);
    EXPECT_EQ(buf2.size(), 4);

    EXPECT_EQ(buf1.at(0), '1');
    EXPECT_EQ(buf2.at(0), 'a');
}

TEST_F(CircularBufferTest, test_overwrite) {
    CircularBuffer buf1(3);
    buf1.push_back('a');
    buf1.push_back('b');
    buf1.push_back('c');
    EXPECT_EQ(buf1.at(0), 'a');
    buf1.push_back('d');
    EXPECT_EQ(buf1.at(0), 'b');
    EXPECT_EQ(buf1.at(1), 'c');
    EXPECT_EQ(buf1.at(2), 'd');
}