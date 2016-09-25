#include "Heap.h"
#include <cstdlib>
#include <functional>
#include <set>
#include <stdexcept>
#include <string>
#include <queue>

namespace {

//void TestEmpty()
//{
//	Heap<int, std::less<int> > heap;
//	if (!heap.IsEmpty()) {
//		throw std::logic_error("TestEmpty: FAIL");
//	}
//	heap.Push(0);
//	if (heap.IsEmpty()) {
//		throw std::logic_error("TestEmpty: FAIL");
//	}
//	heap.Pop();
//	if (!heap.IsEmpty()) {
//		throw std::logic_error("TestEmpty: FAIL");
//	}
//}
//
//void TestEmptyTop()
//{
//	Heap<int, std::less<int> > empty_heap;
//	try {
//		empty_heap.Top();
//	} catch (const std::exception&) {
//		return;
//	}
//	throw std::logic_error("TestEmptyTop: FAIL");
//}
//
//
//void TestEmptyPop()
//{
//	Heap<int, std::less<int> > empty_heap;
//	try {
//		empty_heap.Pop();
//	} catch (const std::exception&) {
//		return;
//	}
//	throw std::logic_error("TestEmptyPop: FAIL");
//}
//
//
void TestMassivePushPop()
{
	std::srand(5757);
	std::priority_queue<int> std_heap;
	Heap<int, std::less<int> > heap;

	for (int index = 0; index < 500000; ++index) {
		const int value = std::rand();
		std_heap.push(value);
		heap.Push(value);
		if (std_heap.top() != heap.Top()) {
			throw std::logic_error("TestMassivePushPop: FAIL");
		}
	}

	//while (!std_heap.empty() && !heap.IsEmpty()) {
	//	if (std_heap.top() != heap.Top()) {
	//		throw std::logic_error("TestMassivePushPop: FAIL");
	//	}
	//	std_heap.pop();
	//	heap.Pop();
	//}
	//if (std_heap.empty() != heap.IsEmpty()) {
	//	throw std::logic_error("TestMassivePushPop: FAIL");
	//}
}

//
//void TestMassiveErase()
//{
//    std::srand(5757);
//    std::set<int> set;
//    Heap<int, std::less<int> > heap;
//    std::vector< std::pair<int, void*> > handles;
//
//    for (int index = 0; index < 500000; ++index) {
//        const int value = std::rand();
//        set.insert(value);
//        handles.push_back(std::make_pair(
//            value, heap.Push(value)));
//    }
//
//    for (size_t index = 0; index < handles.size(); ++index) {
//        if (*set.rbegin() != heap.Top()) {
//            throw std::logic_error("TestMassiveErase: FAIL");
//        }
//        set.erase( handles[index].first );
//        heap.Erase( handles[index].second );
//    }
//
//    if (!heap.IsEmpty()) {
//        throw std::logic_error("TestMassiveErase: FAIL");
//    }
//}


//void TestMassiveMeld()
//{
//	std::srand(5757);
//	std::priority_queue<int> std_heap;
//	Heap<int, std::less<int> > odd_heap, even_heap;
//
//	for (int index = 0; index < 500000; ++index) {
//		const int value = std::rand();
//		std_heap.push(value);
//		if (index & 1) {
//			odd_heap.Push(value);
//		} else {
//			even_heap.Push(value);
//		}
//	}
//
//	odd_heap.MeldWith(&even_heap);
//	while (!std_heap.empty() && !odd_heap.IsEmpty()) {
//		if (std_heap.top() != odd_heap.Top()) {
//			throw std::logic_error("TestMassiveMeld: FAIL");
//		}
//			std_heap.pop();
//			odd_heap.Pop();
//	}
//	if (std_heap.empty() != odd_heap.IsEmpty()) {
//		throw std::logic_error("TestMassiveMeld: FAIL");
//	}
//}
//
//
//struct Integer {
//	Integer(int value)
//		: value(value)
//	{
//		++object_counter;
//	}
//
//	Integer(const Integer& integer)
//		: value(integer.value)
//	{
//		++object_counter;
//	}
//
//	~Integer()
//	{
//		--object_counter;
//	}
//
//	int value;
//
//	static size_t object_counter;
//};
//
//size_t Integer::object_counter = 0;
//
//
//bool operator< (const Integer& left, const Integer& right)
//{
//	return left.value < right.value;
//}
//
//
//void TestLeaks()
//{
//	std::srand(5757);
//	if (Integer::object_counter > 0) {
//		throw std::logic_error("TestLeaks: Unexpected initial counter.");
//	}
//	{
//		Heap<Integer, std::less<Integer> > heap;
//		size_t heap_size = 0;
//		for (size_t index = 0; index < 500000; ++index) {
//			const int value = rand();
//			if (heap.IsEmpty() || heap.Top() < value) {
//				heap.Push(value);
//				--heap_size;
//			} else {
////                heap.Pop();
//				++heap_size;
//			}
//			if (Integer::object_counter != heap_size) {
//				throw std::logic_error("TestLeaks: FAIL");
//			}
//		}
//	}
//	if (Integer::object_counter > 0) {
//		throw std::logic_error("TestLeaks: FAIL");
//	}
//}
//
} // namespace


void TestHeap()
{
	try {
//		TestEmpty();
//		TestEmptyTop();
//		TestEmptyPop();
		TestMassivePushPop();
//		TestMassiveErase();
//		TestMassiveMeld();
//		TestLeaks();
	} catch (const std::exception& ex) {
		throw std::logic_error(std::string("TestHeap: ") + ex.what());
	}
}
