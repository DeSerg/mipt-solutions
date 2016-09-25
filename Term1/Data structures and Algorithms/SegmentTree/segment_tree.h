#pragma once

#include <cstddef>
#include <stdexcept>
#include <vector>




template <class T, class Combiner, class Modifier>
class SegmentTree {

	struct NodeRef
	{
		size_t level;
		size_t index;
	};

	NodeRef LeftChild(const NodeRef &node_ref) const
	{
		NodeRef result;
		result.level = node_ref.level - 1;
		result.index = node_ref.index * 2;
		return result;
	}

	NodeRef RightChild(const NodeRef &node_ref) const
	{
		NodeRef result;
		result.level = node_ref.level - 1;
		result.index = node_ref.index * 2 + 1;
		return result;
	}

	size_t NodeBegin(NodeRef node_ref) const
	{
		return node_ref.index<<node_ref.level;
	}

	size_t NodeEnd(NodeRef node_ref) const
	{
		return (node_ref.index+1) << node_ref.level;
	}

	struct Node
	{
		T value;
		T lazy_arg;
		enum {OP_NONE, OP_SET, OP_MODIFY} lazy_op;
	};

	NodeRef Root() const
	{
		NodeRef root = {root_level_, 0};
		return root;
	}

	typedef std::vector<Node> Level;
	std::vector<Level> tree_;

	size_t root_level_;
	size_t size_;
	Combiner combiner_;
	Modifier modifier_;

public:

	size_t RootLevel(size_t size)
	{
		size_t level = 0;
		while (size > 1) {
			++level;
			size = (size + 1) / 2;
		}
		return level;
	}


	Node At(const NodeRef &node_ref) const
	{
		return tree_[node_ref.level][node_ref.index];
	}

	void SetNode(const NodeRef& node_ref, T arg)
	{
		Node &node = tree_[node_ref.level][node_ref.index];
		if (node_ref.level > 0)
		{
			node.lazy_arg = arg;
			node.lazy_op = Node::OP_SET;
			node.value = Power(arg, NodeEnd(node_ref) - NodeBegin(node_ref));
		}
		else 
		{
			node.value = arg;
		}
	}

	void ModifyNode(const NodeRef& node_ref, T arg)
	{
		Node &node = tree_[node_ref.level][node_ref.index];
		if (node_ref.level > 0)
		{
			if (node.lazy_op == Node::OP_NONE)
			{
				node.lazy_arg = arg;
				node.lazy_op = Node::OP_MODIFY; 
				node.value = modifier_(arg, node.value);
			}
			else
			{
//				node.lazy_arg = Power(node.lazy_arg, NodeEnd(node_ref) - NodeBegin(node_ref));
				node.value = modifier_(arg, node.value);
				node.lazy_arg = modifier_(arg, node.lazy_arg);
			}
		}
		else 
		{
			node.value = modifier_(arg, node.value);
		}
	}

	void ResetNode(const NodeRef& node_ref)
	{
		if (node_ref.level >= tree_.size()) 
			return;
		Level &tree_level = tree_[node_ref.level];
		if (node_ref.index >= tree_level.size())
			return;
		Node &node = tree_level[node_ref.index];
		node.lazy_op = Node::OP_NONE;
		if (node_ref.level > 0) node.value = combiner_(At(LeftChild(node_ref)).value, At(RightChild(node_ref)).value);
		else node.value = node.lazy_arg;
	}

	void PushNode(const NodeRef& node_ref)
	{
		if (node_ref.level > root_level_ || node_ref.level == 0) 
			return;
		Level &tree_level = tree_[node_ref.level];
		if (node_ref.index >= tree_level.size())
			return;
		Node &node = tree_level[node_ref.index];
		if (node.lazy_op == Node::OP_NONE)
			return;
		Node &left = tree_[LeftChild(node_ref).level][LeftChild(node_ref).index],
			 &right = tree_[RightChild(node_ref).level][RightChild(node_ref).index];

		if (node.lazy_op == Node::OP_SET)
		{
			node.lazy_op = Node::OP_NONE;
			SetNode(LeftChild(node_ref), node.lazy_arg);
			SetNode(RightChild(node_ref), node.lazy_arg);
		}
		else
		if (node.lazy_op == Node::OP_MODIFY)
		{
			node.lazy_op = Node::OP_NONE;
			ModifyNode(LeftChild(node_ref), node.lazy_arg);
			ModifyNode(RightChild(node_ref), node.lazy_arg);
		}
	}

	T Power(T value, long degree) const
	{
		T ans = value; 
		--degree;

		while (degree>0)
		{
			if (degree%2) 
			{
				ans = combiner_(ans, value);
				--degree;
			}
			else
			{
				value = combiner_(value, value);
				degree /= 2;
			}
		}
		return ans;
	}

    SegmentTree(size_t size, const T& init_value):
		root_level_(RootLevel(size)), size_(size)
    {
		size = 1 << root_level_;
		Node node = {init_value, init_value, Node::OP_NONE};
		tree_.push_back(Level(size, node));
		for (size_t i = 1; i <= root_level_; i++)
		{
			size /= 2;
			Node node = {combiner_(tree_[i-1][0].value, tree_[i-1][0].value), init_value, Node::OP_NONE};
			tree_.push_back(Level(size, node));
		}
    }

	SegmentTree(size_t size, const T& init_value, Combiner combiner, Modifier modifier):
		root_level_(RootLevel(size)), combiner_(combiner), modifier_(modifier), size_(size)
    {
		size = 1 << root_level_;
		Node node = {init_value, init_value, Node::OP_NONE};
		tree_.push_back(Level(size, node));
		for (size_t i = 1; i <= root_level_; i++)
		{
			size /= 2;
			Node node = {combine(tree_[i-1][0], tree_[i-1][0]), init_value, Node::OP_NONE};
			tree_.push_back(Level(size, node));
		}
    }

    size_t Size() const
    {
		return size_;
    }

    void Set(size_t range_begin, size_t range_end, const T& value, const NodeRef& node_ref)
    {
		size_t begin = NodeBegin(node_ref), end = NodeEnd(node_ref);
		if (range_begin==NodeBegin(node_ref) && range_end==NodeEnd(node_ref))
		{
			SetNode(node_ref, value);
			return;
		}

		PushNode(node_ref);
		const size_t middle = NodeEnd(LeftChild(node_ref));
		if (range_end <= middle)
			Set(range_begin, range_end, value, LeftChild(node_ref));
		else
		if (range_begin >= middle)
			Set(range_begin, range_end, value, RightChild(node_ref));
		else
		{
			Set(range_begin, middle, value, LeftChild(node_ref));
			Set(middle, range_end, value, RightChild(node_ref));
		}
		ResetNode(node_ref);
    }

	void Set(size_t range_begin, size_t range_end, const T& value)
    {
		Set(range_begin, range_end, value, Root());
	}

	T CombineChildren(size_t range_begin, size_t range_end, const NodeRef& node_ref) const
	{
		size_t middle = NodeEnd(LeftChild(node_ref));
		if (range_end <= middle)
			return Combine(range_begin, range_end, LeftChild(node_ref));
		else if (range_begin >= middle)
			return Combine(range_begin, range_end, RightChild(node_ref));
		else
			return combiner_(Combine(range_begin, middle, LeftChild(node_ref)),
							Combine(middle, range_end, RightChild(node_ref)));
	}

    T Combine(size_t range_begin, size_t range_end, const NodeRef& node_ref) const
    {
		if (range_begin==NodeBegin(node_ref) && range_end==NodeEnd(node_ref)) {
			return At(node_ref).value;
		}

		if (NodeEnd(node_ref) <= Size()) {
			const Node& node = At(node_ref);
			if (node.lazy_op == Node::OP_SET) {
				return Power(node.lazy_arg, range_end-range_begin);
			} else if (node.lazy_op == Node::OP_MODIFY)	{
				return modifier_(node.lazy_arg, CombineChildren(range_begin, range_end, node_ref));
			}
		}
		return CombineChildren(range_begin, range_end, node_ref);
    }

    T Combine(size_t range_begin, size_t range_end) const
    {
		NodeRef root_ref = Root();
		if (range_end <= NodeBegin(root_ref) || range_begin > NodeEnd(root_ref))
			throw std::logic_error("Segment Tree: Combine: Wrong range");
		return Combine(range_begin, range_end, root_ref);
    }

	void Modify(size_t range_begin, size_t range_end, const T& modifier_left_argument, const NodeRef& node_ref)
    {
		if (range_begin==NodeBegin(node_ref) && range_end==NodeEnd(node_ref))
		{
			ModifyNode(node_ref, modifier_left_argument);
			return;
		}

		PushNode(node_ref);
		const size_t middle = NodeEnd(LeftChild(node_ref));
		if (range_end <= middle)
			Modify(range_begin, range_end, modifier_left_argument, LeftChild(node_ref));
		else
		if (range_begin >= middle)
			Modify(range_begin, range_end, modifier_left_argument, RightChild(node_ref));
		else
		{
			Modify(range_begin, middle, modifier_left_argument, LeftChild(node_ref));
			Modify(middle, range_end, modifier_left_argument, RightChild(node_ref));
		}
		ResetNode(node_ref);
    }
	
	void Modify(size_t range_begin, size_t range_end, const T& modifier_left_argument)
    {
		Modify(range_begin, range_end, modifier_left_argument, Root());
	}
};
