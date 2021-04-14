#include "LocalVector.h"
#include <fstream>

void test_case_1() {

    LocalVector<size_t> vec;
    assert(vec.size() == 0);
    assert(vec.capacity() == LOCAL_VECTOR_BUFFER_SIZE);
    assert(vec.empty());

    size_t size = 126;
    for (size_t i = 0; i < size; i++) {
        vec.push_back(i);
    }

    assert(vec.size() == size);
    assert(vec.capacity() == 256);
    assert(vec.empty() != true);


    LocalVector<size_t> vec2(vec);
    assert(vec2.capacity() == vec.capacity());
    assert(vec2.size() == vec.size());
}

void test_case_2() {

    LocalVector<size_t> vec;
    assert(vec.size() == 0);
    assert(vec.capacity() == LOCAL_VECTOR_BUFFER_SIZE);
    assert(vec.empty());

    size_t size = 1;
    for (size_t i = 0; i < size; i++) {
        vec.push_back(i);
    }

    assert(vec.size() == size);
    assert(vec.capacity() == LOCAL_VECTOR_BUFFER_SIZE);
    assert(vec.empty() != true);


    LocalVector<size_t> vec2;
    vec2 = vec;
    assert(vec2.capacity() == vec.capacity());
    assert(vec2.size() == vec.size());
}

int main() {

    test_case_2();
    return 0;
}