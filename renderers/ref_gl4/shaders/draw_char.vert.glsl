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
// draw_char.vert.glsl - (Text) Character drawing vertex shader
//

#version 400

//
// Shader I/O
//
layout(location = 0) in vec4 in_Vertex;
layout(location = 1) in vec4 in_CharInfo;

out vec2 v_TexCoord;

uniform vec2 u_CharAspect;

//
// Constant data
//

const vec4 DEBUG_VERTS[4] =
{
    vec4(-1, -1, 0, 0),
    vec4(1, -1, 1, 0),
    vec4(-1, 1, 0, 1),
    vec4(1, 1, 1, 1)
};

// TODO: Whenever we make actually configurable characters, change this
const float CHAR_SIZE = 16.0;

const float INV_CHAR_SIZE = 1.0 / CHAR_SIZE;

void main()
{
    vec4 Vertex;
    vec2 Offset;
    vec2 AtlasMin;
    vec2 AtlasMax;
    vec2 AtlasCoord;
    vec2 AtlasAlpha;

    int Char;
    int Row;
    int Col;

    Char = int(in_CharInfo.z);
    Char = Char & 255;

    Row = Char >> 4;
    Col = Char & 15;

    Vertex      = in_Vertex;
    Vertex.xy   = Vertex.xy * u_CharAspect * CHAR_SIZE;
    //Vertex = DEBUG_VERTS[gl_VertexID];

    Offset = in_CharInfo.xy * vec2(1, -1);
    //Offset = vec2(0, 0);

    AtlasMin = vec2(Col, Row) * INV_CHAR_SIZE;
    AtlasMax = AtlasMin + vec2(INV_CHAR_SIZE, INV_CHAR_SIZE);
    AtlasAlpha = Vertex.zw;
    AtlasAlpha.y = 1.0 - AtlasAlpha.y;

    AtlasCoord = mix(AtlasMin, AtlasMax, AtlasAlpha);

    gl_Position = vec4(Vertex.xy + Offset, 0, 1);
    v_TexCoord = AtlasCoord;
}