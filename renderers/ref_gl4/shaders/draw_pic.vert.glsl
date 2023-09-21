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
// draw_pic.vert.glsl - Pic drawing vertex shader
//

#version 400

//
// Shader I/O
//
layout(location = 0) in vec4 in_Vertex;

out vec2 v_TexCoord;

uniform vec4 u_PicInfo;

//
// Program
//
void main()
{
    vec2 Offset;
    vec2 Size;
    vec2 TexelSize;
    vec2 Scaled;

    Offset  = u_PicInfo.xy;
    Size    = u_PicInfo.zw;

    Scaled  = (in_Vertex.xy * Size);

    gl_Position     = vec4(Scaled + Offset, 0, 1);
    v_TexCoord      = in_Vertex.zw;

    // Flip the Y axis
    v_TexCoord.y    = 1.0 - v_TexCoord.y;
}