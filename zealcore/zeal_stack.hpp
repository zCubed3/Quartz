/*
Copyright (C) 2023 zCubed3 (Liam R.)

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

*/

//
// zeal_stack.hpp - zealStack definition
//
// This is a linked list
//

#ifndef ZEALOT_ZEAL_STACK_HPP
#define ZEALOT_ZEAL_STACK_HPP

//============================================================================

#include "zeal_misc.hpp"

//============================================================================

// A node within a zealStack
// Is placed outside zealStack class for cleaner access
template<typename T>
struct zealNode
{
	T 			element;
	zealNode* 	next;
};

//============================================================================

// TODO: Is this even reasonable to have?
template<typename T>
class zealStack
{
public:
	// Wrapper class for range iteration
	struct Iter
	{
		zealNode<T>* 	node = nullptr;
		bool			last = false;

		// Comparison operator for stopping
		bool operator!=(Iter& rhs) const
		{
			return this->last != rhs.last;
		}

		// Increment opertator
		Iter& operator++()
		{
			Iter iter;

			iter.node = node->next;

			// Only last if there's no next
			iter.last = node->next == nullptr;

			return iter;
		}

		// Deref
		T operator*()
		{
			return node->element;
		}
	};

protected:
	size_t 			count 		= 0;
	zealNode<T>* 	node_stack 	= nullptr;

public:
	~zealStack()
	{
		while(node_stack != nullptr)
		{
			zealNode<T>* current;

			current = node_stack;
			node_stack = current->next;

			zealOptionalDelete(current->element);
			delete current;
		}

		node_stack 	= nullptr;
		count 		= 0;
	}

	zealStack() = default;

	zealStack(zealStack<T>& stack)
	{
		zealNode<T>* 	node;

		for (node = stack.Top(); node != nullptr; node = node->next)
			Push(node->element);
	}

	// Pushes to the top of the stack, shifting the contents downward
	void Push(const T& element)
	{
		zealNode<T>* new_top;

		new_top = new zealNode<T>();

		new_top->element = element;
		new_top->next = node_stack;

		node_stack = new_top;
		count++;
	}

	// Removes the top element from the stack and assigns it to out
	// Returns a boolean value of whether the pop operation was a success
	bool Pop(T& out)
	{
		zealNode<T>* ret;

		ret = node_stack;

		if (node_stack != nullptr)
			node_stack = node_stack->next;

		if (count > 0)
			count--;

		if (ret != nullptr)
		{
			out = ret->element;
			delete ret;

			return true;
		}
		else
			return false;
	}

	// Returns the top of the node stack, this is dangerous, use caution when iterating!
	zealNode<T>* Top() const
	{
		return node_stack;
	}

	// Returns the number of elements contained within this list
	size_t Count() const
	{
		return count;
	}

	//
	// Range based iteration
	//

	Iter begin()
	{
		Iter iter;

		iter.last = node_stack == nullptr;
		iter.node = node_stack;

		return iter;
	}

	Iter end()
	{
		zealNode<T>* 	node;
		Iter			iter;

		node = nullptr;

		// Find the end
		for (node = Top(); node != nullptr; node = node->next)
		{

		}

		iter.node = node;
		iter.last = true;

		return iter;
	}
};

//============================================================================

#endif //ZEALOT_ZEAL_LINKED_LIST_HPP
