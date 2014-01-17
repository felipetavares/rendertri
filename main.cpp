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

#include <iostream>
using namespace std;

#include "math/vec2/vec2.h"
#include "math/vec3/vec3.h"
#include "math/vec4/vec4.h"
#include "math/sm4/sm4.h"
#include "transform/perspective/perspective.h"
#include "rasterizer/triangle.h"
#include "rasterizer/pb.h"
#include "app/screen.h"
#include "render/render.h"
#include "render/world.h"
#include <cstring>
#include <sys/time.h>

int main (int argc, char **argv) {
  if (argc < 2) {
    cout << "RenderTRI demo v1.0" << endl;
    
    cout << "ri1.0 FILENAME    : view the a file" << endl;
    cout << "ri1.0 FILENAME z  : view the z-buffer" << endl;
    return -1;
  }

  pb cbuffer (640,480); // Color buffer
  pb zbuffer (640,480); // Z-Index buffer

  // The 'REAL' screen, displaying the color buffer
  // (the good thing is that we can display any other buffer we want)

  unsigned int* buffer = cbuffer.data;

  if (argc >= 3 && argv[2][0] == 'z')
    buffer = zbuffer.data;

  wmScreen screen (640,480,buffer);

  RI_Renderer renderer;
  renderer.vertexShader = SimpleVertexShader;
  renderer.fragmentShader = SimpleFragmentShader;
  renderer.colorBuffer = &cbuffer;
  renderer.zBuffer = &zbuffer;
  
  RI_Texture tex (600,304);

  /*
  memset (tex.pixelBuffer.data, 0x00, 64*64*4); // Black texture

  int i,j;
  for (i=0;i<64;i++) {
    for (j=0;j<64;j++) {
      if (i%8 < 4 && j%8 < 4)
	tex.pixelBuffer.setPixel(i,j,0xFFFF00FF);
    }
  }
  */

  tex.pixelBuffer.loadFromBitmap ("hdri_01.bmp");

  string file = string (argv[1]);

  RI_ObjLoader loader;
  loader.defaultTex = &tex;

  loader.load (file);

  cout << "Loaded " << loader.triangles.size() << " triangles from '" << file << "'." << endl;

  RI_Triangle tri;

  tri.vertices[0].position = vec3 (-20,  -3,  1);
  tri.vertices[1].position = vec3 (0,    -3,  100);
  tri.vertices[2].position = vec3 (20,   -3,  1);

  tri.vertices[0].color = vec3 (1,0,0);
  tri.vertices[1].color = vec3 (0,1,0);
  tri.vertices[2].color = vec3 (0,0,1);

  tri.vertices[0].texCoord = vec2 (0,1);
  tri.vertices[1].texCoord = vec2 (0.5,0);
  tri.vertices[2].texCoord = vec2 (1,1);

  tri.texture = &tex;

  //renderer.world->triangles.push_back (tri);
  
  renderer.world->triangles = loader.triangles;

  // for fps calc
  double old_time = 0;
  int fpsc=0;
	
  while (1) {
    // + FPS
    struct timeval tv;
    struct timezone tz;
    struct tm *tm;
    gettimeofday(&tv, &tz);
    tm=localtime(&tv.tv_sec);

    if ((int)tm->tm_sec-old_time > 0) {
      old_time = (int)tm->tm_sec;
      cout << fpsc << endl;
      fpsc = 0;
    }
    // - FPS

    // Here we render the entire world
    renderer.render();
    
    screen.update();

    fpsc++;
  }	

  return 0;
}
