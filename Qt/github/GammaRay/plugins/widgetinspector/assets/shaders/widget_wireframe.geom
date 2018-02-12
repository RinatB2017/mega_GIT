#version 150 core

/*
  widget_wireframe.geom

  This file is part of GammaRay, the Qt application inspection and
  manipulation tool.

  Copyright (C) 2011-2018 Klarälvdalens Datakonsult AB, a KDAB Group company, info@kdab.com
  Author: Daniel Vrátil <daniel.vratil@kdab.com>

  Licensees holding valid commercial KDAB GammaRay licenses may use this file in
  accordance with GammaRay Commercial License Agreement provided with the Software.

  Contact info@kdab.com if any conditions of this licensing are not clear to you.

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

layout(triangles) in;
layout(line_strip, max_vertices = 4 /* 2 lines = 4 vertices */) out;

in VertexData {
    vec3 position;
} gs_in[];

void drawLine(vec4 a, vec4 b)
{
    gl_Position = a;
    EmitVertex();

    gl_Position = b;
    EmitVertex();

    EndPrimitive();
}

void main()
{
    // This GS takes all three sides of a rectangle, but only emits vertices for
    // its catheti, which gives us a nice wireframe.

    vec3 aSide = (gs_in[0].position - gs_in[1].position);
    vec3 bSide = (gs_in[0].position - gs_in[2].position);
    vec3 cSide = (gs_in[1].position - gs_in[2].position);

    if (dot(aSide, bSide) == 0) {
        drawLine(gl_in[0].gl_Position, gl_in[1].gl_Position);
        drawLine(gl_in[0].gl_Position, gl_in[2].gl_Position);
    } else if (dot(aSide, cSide) == 0) {
        drawLine(gl_in[0].gl_Position, gl_in[1].gl_Position);
        drawLine(gl_in[1].gl_Position, gl_in[2].gl_Position);
    } else if (dot(bSide, cSide) == 0) {
        drawLine(gl_in[0].gl_Position, gl_in[2].gl_Position);
        drawLine(gl_in[1].gl_Position, gl_in[2].gl_Position);
    }
}
