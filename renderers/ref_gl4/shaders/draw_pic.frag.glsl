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
// draw_pic.frag.glsl - Pic drawing fragment shader
//

#version 400

//
// Shader I/O
//
in vec2 v_TexCoord;

out vec4 FragColor;

uniform sampler2D u_TexPic;

void main()
{
    //FragColor = vec4(v_TexCoord, 1, 1);
    FragColor = texture(u_TexPic, v_TexCoord);

    if (FragColor.a < 0.01)
        discard;
}