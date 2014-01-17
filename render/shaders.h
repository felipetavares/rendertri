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

#ifndef RI_SHADERS_H
#define RI_SHADERS_H

#include "world.h"
#include <functional>
using std::function;

typedef function <RI_Vertex (RI_Vertex&)> RI_VertexShader;

// returns the fragment color, receives interpolated color, texture & texture position
typedef function <vec3 (vec3&,vec2&,RI_Texture*)> RI_FragmentShader;

#endif /* RI_SHADERS_H */
