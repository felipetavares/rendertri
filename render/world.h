/*
  Copyright (C) 2013 by Felipe Tavares

  This file is part of Render TRI.
  
  Render TRI is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.
  
  Render TRI is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
  
  You should have received a copy of the GNU General Public License
  along with Render TRI.  If not, see <http://www.gnu.org/licenses/>. 
*/

#ifndef RI_WORLD_H
#define RI_WORLD_H

#include <vector>
#include "../math/vec2/vec2.h"
#include "../math/vec3/vec3.h"
#include "../rasterizer/pb.h"
using namespace std;

class RI_Texture {
public:
  pb pixelBuffer;
  vec3 getTexel (vec2);

  RI_Texture (unsigned int,unsigned int);
};

class RI_Vertex {
public:
  vec3 position;
  vec3 normal;
  vec3 color;
  vec2 texCoord;
};

class RI_Triangle {
public:
  RI_Vertex vertices[3];
  RI_Texture *texture;
};

class RI_Light {
public:
  vec3 position;
  vec3 normal;
};

class RI_World {
public:
  vector <RI_Triangle> triangles;
  vector <RI_Light> lights;
};

#endif /* RI_WORLD_H */
