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

#include "triangle.h"

#include <cmath>
#include <vector>
#include <iostream>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
using namespace std;

tRas::tRas () {
  rasterColor = 0xFFFFFFFF;
}

void tRas::rasterHLine (register int x1,
			register int x2,
			int y) {
  if (x1 < 0)
    x1 = 0;

  if (x2-x1 < 1)
    return;

  if (y < 0 || y >= pixelBuffer->height)
    return;

  register vec3 colorChange;
  register vec2 coordChange;
  register double zChange;

  double d = 1/(x2-x1);

  colorChange = (endColor-startColor)*d;
  coordChange = (endCoord-startCoord)*d;
  zChange = (endZ-startZ)*d;

  vec3 color = startColor;
  vec2 coord = startCoord;
  double z = startZ;

  while (x1 <= x2) {
    if (x1 >= pixelBuffer->width)
      return;

    double fz = 1/z;

    FloatToUInt conversor;

    conversor.i = zBuffer->getPixel(x1,y);

    if (float(fz) >= conversor.f) {
      conversor.f = float(fz);
      
      vec2 fcoord = coord*fz;
      vec3 fcolor = fragmentShader(color, fcoord, tri.texture);
      unsigned char r=fcolor.x*255,g=fcolor.y*255,b=fcolor.z*255;
      unsigned int fc = (0xFF<<24) | (r<<16) | (g<<8) | b; 
      
      pixelBuffer->setPixel (x1,y,fc);
      zBuffer->setPixel (x1,y, conversor.i);
    }

    color += colorChange;
    coord += coordChange;
    z += zChange;
    x1++;
  }
}

void tRas::rasterTriangle (vec2& _a, vec2& _b, vec2& _c) {
  vec2 a,b,c;

  // First, sort vertices by y
  vector <vec2> v {_a,_b,_c};
  int i,smaller=0;
  for (i=0;i<v.size();i++) {
    if (v[i].y <= v[smaller].y)
      smaller = i;
  }
  ap = smaller;
  a = v[smaller];
  v.erase(v.begin()+smaller);

  smaller = 0;
  for (i=0;i<v.size();i++) {
    if (v[i].y <= v[smaller].y)
      smaller = i;
  }
  if (smaller >= ap)
    bp = smaller+1;
  else
    bp = smaller;
  b = v[smaller];
  v.erase(v.begin()+smaller);

  c = v[0];
  for (i=0;i<3;i++)
    if (i != ap && i != bp)
      cp = i;

  c.y ++;
  a.y --;

  double dz1=0,dz2=0,dz3=0;
  double drz1=0,drz2=0,drz3=0;
  double dx1,dx2,dx3;
  vec3 dc1,dc2,dc3;
  vec2 dt1,dt2,dt3;

  if (c.y-a.y <= 1 || max(max(abs(c.x-a.x),abs(b.x-a.x)),abs(c.x-b.x)) <= 1) {
    if (a.x < 0 || a.x >= pixelBuffer->width ||
	a.y < 0 || a.y >= pixelBuffer->height)
      return;

    FloatToUInt conversor;

    conversor.i = zBuffer->getPixel(a.x,a.y);

    if (tri.vertices[0].position.z >= conversor.f) {
      conversor.f = tri.vertices[0].position.z;
      
      vec2 fcoord = tri.vertices[0].texCoord*tri.vertices[0].position.z;
      vec3 fcolor = fragmentShader(tri.vertices[0].color, fcoord, tri.texture);
      unsigned char r=fcolor.x*255,g=fcolor.y*255,b=fcolor.z*255;
      unsigned int fc = (0xFF<<24) | (r<<16) | (g<<8) | b; 
      
      pixelBuffer->setPixel ((int)a.x,(int)a.y,fc);
      zBuffer->setPixel ((int)a.x, (int)a.y, conversor.i);
    }
    return;
  }

  if (b.y-a.y > 0) {
    dx1=(b.x-a.x)/(b.y-a.y);
    dc1=(tri.vertices[bp].color-tri.vertices[ap].color)/(b.y-a.y);
    dt1=(tri.vertices[bp].texCoord/tri.vertices[bp].position.z-tri.vertices[ap].texCoord/tri.vertices[ap].position.z)/(b.y-a.y);
    dz1=(1/tri.vertices[bp].position.z-1/tri.vertices[ap].position.z)/(b.y-a.y);
    drz1=(tri.vertices[bp].position.z-tri.vertices[ap].position.z)/(b.y-a.y);
  } else
    dx1=0;
  if (c.y-a.y > 0) {
    dx2=(c.x-a.x)/(c.y-a.y);
    dc2=(tri.vertices[cp].color-tri.vertices[ap].color)/(c.y-a.y);
    dt2=(tri.vertices[cp].texCoord/tri.vertices[cp].position.z-tri.vertices[ap].texCoord/tri.vertices[ap].position.z)/(c.y-a.y);
    dz2=(1/tri.vertices[cp].position.z-1/tri.vertices[ap].position.z)/(c.y-a.y);
    drz2=(tri.vertices[cp].position.z-tri.vertices[ap].position.z)/(c.y-a.y);
  } else
    dx2=0;
  if (c.y-b.y > 0) {
    dx3=(c.x-b.x)/(c.y-b.y);
    dc3=(tri.vertices[cp].color-tri.vertices[bp].color)/(c.y-b.y);
    dt3=(tri.vertices[cp].texCoord/tri.vertices[cp].position.z-tri.vertices[bp].texCoord/tri.vertices[bp].position.z)/(c.y-b.y);
    dz3=(1/tri.vertices[cp].position.z-1/tri.vertices[bp].position.z)/(c.y-b.y);
    drz3=(tri.vertices[cp].position.z-tri.vertices[bp].position.z)/(c.y-b.y);
  } else
    dx3=0;

  vec2 S,E;

  S = a;
  E = a;

  startColor = endColor = tri.vertices[ap].color;
  startCoord = endCoord = tri.vertices[ap].texCoord/tri.vertices[ap].position.z;
  startZ = endZ = 1/tri.vertices[ap].position.z;

  if(dx1 > dx2) {
    for(;S.y<=b.y;S.y++,S.x+=dx2,E.x+=dx1,startColor+=dc2,endColor+=dc1,startCoord+=dt2,endCoord+=dt1,startZ+=dz2,endZ+=dz1)
      rasterHLine(S.x,E.x,S.y);
    E = b;
    endColor = tri.vertices[bp].color;
    endCoord = tri.vertices[bp].texCoord/tri.vertices[bp].position.z;
    endZ = 1/tri.vertices[bp].position.z;
    for(;S.y<=c.y;S.y++,S.x+=dx2,E.x+=dx3,startColor+=dc2,endColor+=dc3,startCoord+=dt2,endCoord+=dt3,startZ+=dz2,endZ+=dz3)
      rasterHLine(S.x,E.x,S.y);
  } else {
    for(;S.y<=b.y;S.y++,S.x+=dx1,E.x+=dx2,startColor+=dc1,endColor+=dc2,startCoord+=dt1,endCoord+=dt2,startZ+=dz1,endZ+=dz2)
      rasterHLine(S.x,E.x,S.y);
    S = b;
    startColor = tri.vertices[bp].color;
    startCoord = tri.vertices[bp].texCoord/tri.vertices[bp].position.z;
    startZ = 1/tri.vertices[bp].position.z;
    for(;S.y<=c.y;S.y++,S.x+=dx3,E.x+=dx2,startColor+=dc3,endColor+=dc2,startCoord+=dt3,endCoord+=dt2,startZ+=dz3,endZ+=dz2)
      rasterHLine(S.x,E.x,S.y);
  }
}

void tRas::clearColorBuffer (unsigned int color) {
  memset (pixelBuffer->data, color, pixelBuffer->size*4);
}

void tRas::clearZBuffer (unsigned int color) {
  unsigned int* ptr = zBuffer->data;
  unsigned int* end = ptr+zBuffer->size;
  
  while (ptr < end) {
    *ptr++ = color;
  }
}
