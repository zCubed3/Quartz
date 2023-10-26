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
// unlit_model.vert.glsl - Unlit model drawing vertex shader
//

#version 400

layout(location = 0) in vec3 in_Position;
layout(location = 1) in vec2 in_TexCoord;
layout(location = 2) in vec3 in_Normal;

uniform mat4 u_ViewProjection;
uniform mat4 u_Model;

out vec4 v_Color;

void main() {
    gl_Position = u_ViewProjection * u_Model * vec4(in_Position.xyz, 1.0);
    //v_Color = vec4(in_Normal, 1);
    v_Color = vec4(0, 1, 0, 1);
}