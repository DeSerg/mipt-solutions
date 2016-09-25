#include <iostream>
#include "segment_tree.h"

template<class T>
T max(T a, T b)
{
	return (a > b) ? a : b;
}

struct NodeValueType
{
	long all_sum, max_sum, left, right;
};

struct IntervalCombiner
{
	NodeValueType operator() (const NodeValueType &val1, const NodeValueType &val2) const
	{
		NodeValueType node_value;
		node_value.all_sum = val1.all_sum + val2.all_sum;
		node_value.left = max(val1.left, val1.all_sum + val2.left);
		node_value.right = max(val2.right, val2.all_sum + val1.right);
		node_value.max_sum = max(max(val1.max_sum, val2.max_sum), val1.right + val2.left);
		return node_value;
	}
};

void MaxSumIntervals()
{
	size_t N;
	NodeValueType node_value = {0, 0, 0, 0};
	
	std::cout << "Insert array length\n";
	std::cin >> N;
	SegmentTree<NodeValueType, IntervalCombiner, IntervalCombiner> segment_tree(N, node_value);

	std::cout << "Insert array\n";
	for (size_t i = 0; i < N; i++)
	{
		long val;
		std::cin >> val;

		NodeValueType node_value;
		node_value.all_sum = val;
		node_value.left = node_value.right = node_value.max_sum = max(val, 0l);
		
		segment_tree.Set(i, i+1, node_value);
	}

	size_t M;

	std::cout << "Insert test number\n";
	std::cin >> M;

	std::cout << "Insert tests\n";
	for (size_t i = 0; i < M; i++)
	{
		size_t l, r;

		std::cin >> l >> r;
		l--;

		std::cout << segment_tree.Combine(l, r).max_sum << std::endl;
	}
}