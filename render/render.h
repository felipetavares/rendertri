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

#ifndef RI_RENDER_H
#define RI_RENDER_H

#include "../rasterizer/pb.h"
#include "../rasterizer/triangle.h"
#include "../transform/perspective/perspective.h"
#include "world.h"
#include "shaders.h"
using namespace std;

class RI_ObjLoader {
public:
  vector <RI_Triangle> triangles;
  RI_Texture *defaultTex;

  RI_ObjLoader ();
  void load (string);
};

class RI_Renderer {
public:
  double a;

  pCamera camera;
  tRas rasterizer;

  RI_World *world;
  RI_VertexShader vertexShader;
  RI_FragmentShader fragmentShader;

  pb *colorBuffer;
  pb *zBuffer;

  RI_Renderer ();

  bool render();
  void renderTriangle (RI_Triangle&);
};

vec3 SimpleFragmentShader (vec3&,vec2&,RI_Texture*);
RI_Vertex SimpleVertexShader (RI_Vertex&);

#endif /* RI_RENDER_H */
