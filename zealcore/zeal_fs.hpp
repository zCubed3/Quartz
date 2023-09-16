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
// zeal_fs.hpp - zealFS definition
//

#ifndef ZEALOT_ZEAL_FS_HPP
#define ZEALOT_ZEAL_FS_HPP

//============================================================================

#include "zeal_linked_list.hpp"

//============================================================================

class zealFSNode;
class zealFSStream;

//============================================================================

typedef enum
{
	ZEAL_MOUNT_SUCCESS,
	ZEAL_MOUNT_FAILURE,
	ZEAL_MOUNT_NULL_NODE,
} zeal_mount_state_t;

//============================================================================

// Zealot's FileSystem, a replacement over id's filesystem
// Completely custom implemention, therefore, my code!
class zealFS
{
protected:
	zealLinkedList<zealFSNode*> 	node_list;

public:
	// Mounts the provided node into the internal list of mount points
	// Returns a mount error code
	zeal_mount_state_t 		MountNode(zealFSNode* node);

	// Attempts to open a file at the given path, you provide the node explicitly
	// TODO: Use std::string?
	zealFSStream* 			Open(const char* node, const char* path);
	zealFSStream* 			Open(const char* url);
};

//============================================================================

class zealFSNode
{
public:
	virtual zealFSStream	Open(const char* path) = 0;
	virtual void			Close(zealFSStream* stream) = 0;
};

//============================================================================


#endif //ZEALOT_ZEAL_FS_HPP
