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

#include "zeal_stack.hpp"
#include "zeal_string.hpp"
#include "zeal_vector.hpp"
//#include "fs/zeal_folder_node.hpp"

#include <cstdio>

int main(int argc, char** argv)
{
    printf("[zealLinkedList<T> Tests]\n\n");

	// Test our linked list
	{
		zealStack<float> 	float_list;
		zealStack<float*>	float_ptr_list;
		float 				float_out;

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

    printf("\n[zealString Tests]\n\n");

	// Test our string type
	{
		zealString	str_default;
		zealString	str_alloc("Hello, World!");
		zealString 	str_copy(str_alloc);

		zealString 	str_added;

		str_added += str_alloc;
		str_added += " -- ";

		for (char s = 'A'; s <= 'z'; s++)
		{
			str_added += s;
		}

		// Print each string
		printf("str_default = %s\n", str_default.CStr());
		printf("str_alloc = %s\n", str_alloc.CStr());
		printf("str_copy = %s\n", str_copy.CStr());
		printf("str_added = %s\n", str_added.CStr());
	}

    printf("\n[zealVector Tests]\n\n");

    // Test our vector type
    {
        zealVector<float>   vec;
        float               pop_out;

        for (float i = 0.0F; i < 12.0F; i += 1.0F)
        {
            vec.Append(i);
        }

        // Then print the results
        for (int i = 0; i < vec.Count(); i++)
        {
            float   f = -1.0F;

            vec.Get(i, f);
            printf("%f\n", f);
        }

        printf("\n\n---------\n\n");

        while (vec.Remove(0, &pop_out))
        {
            printf("%f\n", pop_out);
        }
    }

    /*
	printf("\n[zealFS Tests]\n\n");

	// Test our file system
	{
		zealFS* fs;

		fs = new zealFS;

		// Mount a folder
		{
			zealFSFolderNode* 			folder;
			std::vector<zeal_file_t> 	files;

			folder = zealFSFolderNode::OpenFolder("baseq2");

			fs->MountNode("game", folder);

			// Then ask what files are under the folder
			files = fs->GetFiles("game");

			for (zeal_file_t& file : files)
			{
				printf("File = %s\n", file.path.c_str());
			}
		}
	}
     */
}

#endif