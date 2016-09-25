#include "test_heap.h"
#include <iostream>


int main()
{
    try {
        TestHeap();
    } catch (const std::exception& ex) {
        std::cerr << "Exception: " << ex.what() << std::endl;
        return -1;
    }
    return 0;
}
