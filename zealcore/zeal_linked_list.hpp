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
// zeal_linked_list.hpp - zealLinkedList definition
//

#ifndef ZEALOT_ZEAL_LINKED_LIST_HPP
#define ZEALOT_ZEAL_LINKED_LIST_HPP

//============================================================================

#include "zeal_misc.hpp"

//============================================================================

// TODO: Is this even reasonable to have?
template<typename T>
class zealLinkedList
{
public:
	typedef struct node_s
	{
		T value;
		struct node_s* next;
	} node_t;

protected:
	node_t* node_stack = nullptr;

public:
	~zealLinkedList()
	{
		while(node_stack != nullptr)
		{
			node_t* 	current;

			current = node_stack;
			node_stack = current->next;

			zealOptionalDelete(current->value);
			delete current;
		}
	}

	// Pushes to the top of the stack, shifting the contents downward
	void Push(T value)
	{
		node_t* 	new_top;

		new_top = new node_t;

		new_top->value = value;
		new_top->next = node_stack;

		node_stack = new_top;
	}

	// Removes the top element from the stack and assigns it to out
	// Returns a boolean value of whether the pop operation was a success
	bool Pop(T& out)
	{
		node_t* 	ret;

		ret = node_stack;

		if (node_stack != nullptr)
			node_stack = node_stack->next;

		if (ret != nullptr)
		{
			out = ret->value;
			delete ret;

			return true;
		}
		else
			return false;
	}

	// Returns the top of the node stack, this is dangerous, use caution when iterating!
	[[nodiscard]]
	node_t* Top()
	{
		return node_stack;
	}
};

//============================================================================

#endif //ZEALOT_ZEAL_LINKED_LIST_HPP
