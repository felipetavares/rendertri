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

#ifndef RI_TRIANGLE_H
#define RI_TRIANGLE_H

#include "../render/shaders.h"
#include "../render/world.h"
#include "../math/vec2/vec2.h"
#include "pb.h" // Pixel Buffer

typedef union FloatToUInt {
  unsigned int i;
  float f;
}FloatToUInt;

class tRas {
public:
  RI_FragmentShader fragmentShader;
  pb *pixelBuffer;
  pb *zBuffer;
  RI_Triangle tri;
  int ap,bp,cp; // How do we sorted the things?, it is here

  vec3 startColor,endColor;
  vec2 startCoord,endCoord;
  double startZ,endZ;
  double startRZ,endRZ;

  tRas();

  unsigned int rasterColor;
  void rasterHLine (int,int,int);
  void rasterTriangle (vec2&,vec2&,vec2&);
  void clearColorBuffer (unsigned int);
  void clearZBuffer (unsigned int);
};

#endif /* RI_TRIANGLE_H */
