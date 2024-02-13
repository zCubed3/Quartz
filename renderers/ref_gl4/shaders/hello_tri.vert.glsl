/*
Copyright (C) 2023-2024 Liam Reese (zCubed3)

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
// hello_tri.vert.glsl - Test triangle vertex shader
//

#version 400

uniform mat4 u_ViewProjection;
uniform mat4 u_Model;

const vec3 VERTEX_POSITIONS[3] = {
    vec3(-0.5, 0.1, -0.5),
    vec3(0.0, 0.1, 0.5),
    vec3(0.5, 0.1, -0.5)
};

const vec3 VERTEX_COLORS[3] = {
    vec3(1.0, 0.0, 0.0),
    vec3(0.0, 1.0, 0.0),
    vec3(0.0, 0.0, 1.0)
};

out vec3 _ZEALOT_VERT_COLOR;

void main() {
    vec3 Vertex = VERTEX_POSITIONS[gl_VertexID] * 50;

    gl_Position = u_ViewProjection * u_Model * vec4(Vertex, 1.0);
    _ZEALOT_VERT_COLOR = VERTEX_COLORS[gl_VertexID];
}