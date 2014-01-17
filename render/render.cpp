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

#include "render.h"

RI_Texture::RI_Texture (unsigned int w,unsigned int h):
  pixelBuffer(w,h) {
}

#include <iostream>
#include <fstream>

RI_ObjLoader::RI_ObjLoader () {
}

void RI_ObjLoader::load (string fileName) {
  fstream file (fileName.c_str(), ios::in);
  RI_Triangle t;
  RI_Vertex v;
  vec3 norm;
  string function;
  char c;

  vector <RI_Vertex> vertices;
  vector <vec3> normals;

  int v1,v2,v3;
  int n1,n2,n3;

  double n=0;

  while (file.good()) {
    n+=0.01;
    file >> function; 
    
    if (function == "v") {
      file >> v.position.x >> v.position.y >> v.position.z;
      
      v.color = vec3 (sin(n),tan(n),cos(n)/2);
      
      if (v.color.x > 0.5)
	v.color.x = 1;
      else
	v.color.x = 0;
      if (v.color.y > 0.5)
	v.color.y = 1;
      else
	v.color.y = 0;
      if (v.color.z > 0.5)
	v.color.z = 1;
      else
	v.color.z = 0.5;
      vertices.push_back (v);
    } if (function == "vn") {
      file >> norm.x >> norm.y >> norm.z;
      normals.push_back (norm);
    } else if (function == "f") {
      //file >> v1 >> c >> c >> n1 >> v2 >> c >> c >> n2 >> v3 >> c >> c >> n3;
      file >> v1 >> v2 >> v3;
      t.vertices[0] = vertices[v1-1];
      t.vertices[1] = vertices[v2-1];
      t.vertices[2] = vertices[v3-1];
      //t.vertices[0].normal = normals[n1-1];
      //t.vertices[1].normal = normals[n2-1];
      //t.vertices[2].normal = normals[n3-1];
      t.vertices[2].normal = t.vertices[1].normal = t.vertices[0].normal = (t.vertices[0].position-t.vertices[1].position)*(t.vertices[0].position-t.vertices[2].position);
    
      t.vertices[0].normal.normalize();
      t.vertices[1].normal.normalize();
      t.vertices[2].normal.normalize();
      
      t.texture = this->defaultTex;
      triangles.push_back (t);
    } else {
      do {
	file.readsome(&c,1);
      } while (c != '\n' && c != '\r');
    }
  }

  file.close();
}

vec3 RI_Texture::getTexel (vec2 pos) {
  pos.x = fmod(fabs(pos.x),1);
  pos.y = fmod(fabs(pos.y),1);

  unsigned int ucolor = pixelBuffer.getPixel (pos.x*(double)pixelBuffer.width,
					      pos.y*(double)pixelBuffer.height);
  return vec3 (double((ucolor>>16)&0xFF)/255,double((ucolor>>8)&0xFF)/255,double(ucolor&0xFF)/255);
}

RI_Renderer::RI_Renderer () {
  vertexShader = NULL;
  fragmentShader = NULL;

  world = new RI_World();

  colorBuffer = NULL;
  zBuffer = NULL;
  a = 0;
}

bool RI_Renderer::render () {
  // Verify if we have everything we need
  if (vertexShader != NULL &&
      fragmentShader != NULL &&
      colorBuffer != NULL &&
      zBuffer != NULL) {
    // Maybe you noticed that I didn't put a check for 'world'
    // this is because world is aways initialized (take a look up)
    camera.transform.identity();
    rasterizer.fragmentShader = fragmentShader;
    rasterizer.pixelBuffer = colorBuffer;
    rasterizer.zBuffer = zBuffer;

    rasterizer.clearColorBuffer (0);

    FloatToUInt conversor;
    conversor.f = -9e10;
    rasterizer.clearZBuffer(conversor.i);

    sm4 rZ = sm4::rotateX(a+=0.5);
    sm4 rY = sm4::rotateZ(a/2);
    sm4 rX = sm4::rotateY(a/4);

    camera.transform *= rY;
    camera.transform *= rZ;
    camera.transform *= rX;
    camera.transform.d.z = 10;

    for (auto& t :world->triangles) {
      renderTriangle (t);
    }
    return true;
  } else {
    return false;
  }
}

vec3 lightPosition;

void RI_Renderer::renderTriangle (RI_Triangle& tri) {
  vec2 d (colorBuffer->width/2,colorBuffer->height/2);
  vec2 a,b,c;
  
  a = camera.project(tri.vertices[0].position)+d;
  b = camera.project(tri.vertices[1].position)+d;
  c = camera.project(tri.vertices[2].position)+d;

  RI_Triangle t;
  t.texture = tri.texture;
  t.vertices[0] = tri.vertices[0];
  t.vertices[1] = tri.vertices[1];
  t.vertices[2] = tri.vertices[2];

  t.vertices[0].position = (camera.transform* vec4(t.vertices[0].position,1)).toVec3();
  t.vertices[1].position = (camera.transform* vec4(t.vertices[1].position,1)).toVec3();
  t.vertices[2].position = (camera.transform* vec4(t.vertices[2].position,1)).toVec3();

  sm4 m = camera.transform;

  t.vertices[0].normal = (m*vec4(t.vertices[0].normal,1)).toVec3();
  t.vertices[1].normal = (m*vec4(t.vertices[1].normal,1)).toVec3();
  t.vertices[2].normal = (m*vec4(t.vertices[2].normal,1)).toVec3();

  lightPosition = (vec4(vec3 (100,50,15),1)).toVec3();

  t.vertices[0] = vertexShader(t.vertices[0]);
  t.vertices[1] = vertexShader(t.vertices[1]);
  t.vertices[2] = vertexShader(t.vertices[2]);
  rasterizer.tri = t;

  rasterizer.rasterTriangle (a,b,c);
}

RI_Vertex SimpleVertexShader (RI_Vertex& v) {
  RI_Vertex r;
  vec3 lightDir = v.position-lightPosition;
  lightDir.normalize();

  double diffuseFactor = max(0.0,v.normal.dot(lightDir));

  diffuseFactor>1?diffuseFactor=1:0;

  r.position = v.position;
  r.normal   = v.normal;
  r.color    = vec3(diffuseFactor,diffuseFactor,diffuseFactor);
  r.texCoord = v.texCoord;

  return r;
}

vec3 SimpleFragmentShader (vec3& color, vec2& texCoord, RI_Texture* tex) {
  vec3 texel = tex->getTexel(texCoord);
  return vec3(color);

  // vec3 lightDir = color-lightPosition;
  //lightDir.normalize();
  //double diffuseFactor = max(0.0,color.dot(lightDir));

  //return vec3(color.x*0.5+texel.x*0.5,color.y*0.5+texel.y*0.5,color.z*0.5+texel.z*0.5);
  //return texel;
  //return vec3 {1,1,1};
}
