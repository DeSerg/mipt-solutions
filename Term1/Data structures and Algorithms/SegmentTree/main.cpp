#include "test_segment_tree.h"
#include "sequence.h"
#include <exception>
#include <iostream>
#include <cstdio>

int main()
{

//	freopen("input.txt", "r", stdin); freopen("output.txt", "w", stdout);

    try {
        TestSegmentTree();

    } catch (const std::exception& ex) {
        std::cerr << "Exception: " << ex.what() << std::endl;
        return -1;
    }

	try
	{
		MaxSumIntervals();
	}
	catch (const std::exception& ex) {
        std::cerr << "Exception: " << ex.what() << std::endl;
        return -1;
    }
    return 0;
}
