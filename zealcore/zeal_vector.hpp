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
// zeal_vector.hpp - zealVector definition
//

#ifndef ZEALOT_ZEAL_VECTOR_HPP
#define ZEALOT_ZEAL_VECTOR_HPP

//============================================================================

#include "zeal_misc.hpp"

#define ZEAL_VECTOR_INIT_SIZE 2

//============================================================================

template<typename T>
class zealVector
{
protected:
	size_t  size 	= 0;
    size_t  top     = 0;
    bool    empty   = true;

	T*      buffer  = nullptr;

    // Reallocates the internal buffer, copying the data
    void Reallocate(size_t new_size)
    {
        size_t  copy_size;
        T*      old_buffer;

        old_buffer = this->buffer;

        this->buffer = new T[new_size];

        // Copy each initialized element
        if (old_buffer != nullptr)
        {
            copy_size = top;

            if (copy_size > new_size) // Out of bounds?
                copy_size = new_size;

            for (size_t e = 0; e < copy_size; e++)
                this->buffer[e] = old_buffer[e];

            delete[] old_buffer;
        }

        // Set our members
        this->size = new_size;
    }

    // Ensures the buffer is valid and if it must be reallocated
    void ValidateBuffer()
    {
        if (this->buffer == nullptr)
            Reallocate(ZEAL_VECTOR_INIT_SIZE);

        if (this->top >= this->size)
            Reallocate(this->size * 2); // Double the size of our vector
    }

public:
	~zealVector()
	{
		for (size_t i = 0; i < top; i++)
			zealOptionalDelete(buffer[i]);

        delete[] buffer;
	}

	// Default ctor
	zealVector() = default;

	// Initializes the vector with the given size
	explicit zealVector(size_t reservation)
	{
		Reallocate(reservation);
	}

	// Adds a new element to the vector, resizing to fit the addition if needed
	void Append(const T& element)
	{
        ValidateBuffer();

        buffer[top] = element;

        top += 1;

        empty = false;
    }

    // Removes an element from the top of the vector
    bool Pop(T& out)
    {
        if (this->buffer == nullptr || empty)
            return false;

        if (this->top > 0)
            this->top -= 1;

        out = this->buffer[this->top];

        if (this->top == 0)
            this->empty = true;
    }

	// Removes an element from the provided index and optionally returns it
	//
	// Returns true if it was successfully removed
	bool Remove(size_t index, T* opt_out)
	{
		bool 	has_prior;
		T 		prior_value;
		T		swap_value;

		if (this->buffer == nullptr || empty)
			return false;

		// Output the element
		if (opt_out != nullptr)
			*opt_out = this->buffer[index];

		// Shift upper contents to the left
		if (top > 0)
		{
			has_prior = false;

			for (size_t i = this->top - 1; i >= index; i--)
			{
				swap_value = buffer[i];

				if (has_prior)
					buffer[i] = prior_value;

				prior_value = swap_value;
				has_prior = true;

				// Special case bc of unsigned numbers
				if (i == 0)
					break;
			}

			this->top -= 1;
		}

		if (this->top == 0)
			this->empty = true;

		return true;
	}

    // Attempts to get an element from the vector at the given index
    //
    // Returns false if out of bounds
    bool Get(size_t index, T& out)
    {
        if (index >= top)
            return false;

        if (empty)
            return false;

        out = this->buffer[index];
        return true;
    }

    // Returns if the list is empty
    [[nodiscard]]
    bool Empty() const
    {
        return empty;
    }

    // Returns the limit of elements this vector can hold with the current buffer
    [[nodiscard]]
    size_t Capacity() const
    {
        return size;
    }

	// Returns the number of elements within this vector
	[[nodiscard]]
    size_t Count() const
	{
		return top;
	}

	//
	// Operators
	//

	// Unsafe but faster indexing
	// Directly indexes the underlying buffer
	T operator[](size_t index) const
	{
		return buffer[index];
	}
};

//============================================================================

#endif //ZEALOT_ZEAL_VECTOR_HPP
