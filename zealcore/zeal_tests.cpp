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
// zeal_main - nuqlib main function
//

#ifdef USE_ZEALCORE_MAIN

#include "zeal_linked_list.hpp"
#include "zeal_fs.hpp"

#include <cstdio>

int main(int argc, char** argv)
{
	// Test our linked list
	{
		zealLinkedList<float> 	float_list;
		zealLinkedList<float*>	float_ptr_list;
		float 					float_out;

		for (float f = 0; f < 1000; f += 1.0F)
		{
			float 	val;
			float* 	val_ptr;

			val = f;
			val_ptr = new float(f);

			float_list.Push(val);
			float_ptr_list.Push(val_ptr);
		}

		while (float_list.Pop(float_out))
		{
			printf("%f\n", float_out);
		}
	}
}

#endif