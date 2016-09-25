#pragma once

#include <stdexcept>
#include <list>
#include <memory>

template <class T, class Compare>
class Heap {

	struct Node
	{
//		Node() : right(NULL), child(NULL) {}
		T data;
		std::auto_ptr<Node> right;
		std::auto_ptr<Node> child;
		Node *left;
		Node *parent;
		size_t degree;
	};
	
	typedef std::auto_ptr<Node> NodeRef;
	typedef std::list<NodeRef> RootList;
	RootList root_list_;
	Compare compare_;	
	
	struct NodeCompare
	{
		bool operator () (const NodeRef &node_ref1, const NodeRef &node_ref2)
		{
			return node_ref1->degree < node_ref2->degree;
		}
	};

	NodeRef MergeTrees(Node *node1, Node *node2)
	{
		size_t size = node1->degree;
		Node *child;
		if (compare_(node1->data, node2->data)) std::swap(node1, node2);
		if (node2->child.get()) child = node2->child.get();
		*node2->child = *node1;
		node1->parent = node2;
		while (child->child.get())
		{
			Node *right_brother = node1;
			Node *son = node1->child.get();
			Node *father = node1;
			while (right_brother->right.get())
			{
				right_brother->right->left = right_brother;
				*right_brother = *right_brother->right;
				*right_brother->child = *son;
			}

			right_brother->right->left = right_brother;
			*right_brother->right = *child;

			child = child->child.get();
			while (right_brother->right.get())
			{
				right_brother->right->left = right_brother;
				right_brother = right_brother->right.get();
				*right_brother->child = *son;
			}
			node1 = node1->child.get();
		}
	}

public:
    Heap()
	{
		root_list_.clear();
	}

    Heap(Compare compare):
		compare_(compare)
    {}
	
	bool IsEmpty() const
    {
		return root_list_.empty();
    }

	const T& Top() const
    {
		if (IsEmpty())
			throw std::logic_error("Empty heap: top");
		RootList::const_iterator it_result = root_list_.begin();
		for (RootList::const_iterator it = it_result; it != root_list_.end(); ++it)
		{
			if (!compare_((**it).data, (**it_result).data))
				it_result = it;
		}

		return (**it_result).data;
    }

	void Pop()
    {
		if (IsEmpty()) return;
		RootList::const_iterator it_result = root_list_.begin();
		for (RootList::const_iterator it = it_result; it != root_list_.end(); ++it)
		{
			if (!compare_((**it).data, (**it_result).data))
				it_result = it;
		}

		NodeRef child = (**it_result).child;
		Heap<T, Compare> *heap;
		if (child.get())
		{
			child->parent = NULL;
			heap->root_list_.push_back(child);
		}

		while (child->right.get())
		{
			Node *temp = child.get();
			child = child->right;
			temp->right.reset();
			child->left = NULL;
			heap->root_list_.push_back(child);
		}

		if (!heap->IsEmpty()) MeldWith(heap);
    }

    void *Push(const T& value)
    {
		std::auto_ptr<Node> node_ref(new Node);
		node_ref->data = value;
		node_ref->degree = 0;
		node_ref->parent = NULL;
		node_ref->left = NULL;
		Heap<T, Compare> *heap(new Heap);
		Node *node = node_ref.get();
		heap->root_list_.clear();
		heap->root_list_.push_back(std::move(node_ref));
		MeldWith(heap);
		delete heap;
		return node;
    }


	/*void erase(void* handle)
	{
		throw std::logic_error("heap.erase: not implemented.");
	}
*/
    void MeldWith(Heap<T, Compare>* other_heap)
    {
		if (other_heap->IsEmpty()) return;
		NodeCompare comp;
		root_list_.merge(other_heap->root_list_, comp);
		if (root_list_.size() < 2) return;
		
		RootList::iterator it1 = root_list_.begin(), it2 = it1;
		it2++;

		if (root_list_.size() == 2)
		{
			if ((**it1).degree == (**it2).degree)
			{
				*it2 = MergeTrees(std::move(it1), std::move(it2));
				root_list_.erase(it1);
			}
			return;
		}

		RootList::iterator it3 = it2;
		it3++;
		for (; it3 != root_list_.end(); ++it1, ++it2, ++it3)
		{
			if ((**it1).degree == (**it2).degree && (**it2).degree == (**it3).degree)
			{
				MergeTrees(it2->get(), it3->get());
				root_list_.erase(it2);
			}
			else
			if ((**it1).degree == (**it2).degree)
			{
				MergeTrees((*it1).get(), (*it2).get());
				root_list_.erase(it1);
			}
		}

		if ((**it1).degree == (**it2).degree)
		{
			MergeTrees((*it1).get(), (*it2).get());
			root_list_.erase(it1);
		}
    }
};
