#include <gtest/gtest.h>
#include "CircularBuffer.h"

int main() {
    CircularBuffer<char> buffer(4);
    buffer.push_back('a');
    buffer.push_back('c');
    buffer.push_back('b');
    buffer.push_front('d');
    buffer.push_back('e');
    buffer.push_front('f');
    for (auto elem : buffer) {
        std::cout << elem << " ";
    }
    std::cout << std::endl;
    testing::InitGoogleTest();
    RUN_ALL_TESTS();
    return 0;
}
