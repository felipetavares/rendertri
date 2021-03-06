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

#include "pb.h"
#include <fstream>
using namespace std;

pb::pb (unsigned int w,
	unsigned int h) {
  width = w;
  height = h;
  size = w*h;

  data = new unsigned int[size];

  int i;
  for (i=0;i<size;i++)
    data[i] = 0;
}

pb::~pb () {
  delete data;
}

unsigned int pb::getPixel (unsigned int x,unsigned int y) {
  return data[y*width+x];
}

unsigned int pb::setPixel (unsigned int x,unsigned int y,
			   unsigned int color) {
  data[y*width+x] = color;
  return color;
}

unsigned int pb::fastGetPixel (unsigned int p) {
  return data[p];
}

unsigned int pb::fastSetPixel (unsigned int p,
			       unsigned int color) {
  data[p] = color;
  return color;
}

unsigned int pb::compilePosition (unsigned int x,unsigned int y) {
  return y*width+x;
}

void pb::saveAsBitmap (string fileName) {
  fstream file (fileName.c_str(),ios::out|ios::binary);
  char buffer[2];

  unsigned int fileSize = size*4 + 14 + 40;
  unsigned int startPosition = 14 + 40;

  buffer[0] = 0x42;
  buffer[1] = 0x4D;
  file.write (buffer,2);
  file.write ((char*)&fileSize,4);

  buffer[0] = 0;
  buffer[1] = 0;
  file.write (buffer,2);
  file.write (buffer,2);

  file.write ((char*)&startPosition,4);  

  unsigned int dibSize = 40;
  unsigned int dibType = 0;
  unsigned short cPlanes = 1;
  unsigned short dibBpp = 32;

  unsigned int rH = 1;
  unsigned int rW = 1;
  unsigned int col = 0;
  unsigned int iCol = 0;

  file.write ((char*)&dibSize,4);
  file.write ((char*)&width,4);
  file.write ((char*)&height,4);
  file.write ((char*)&cPlanes,2);
  file.write ((char*)&dibBpp,2);
  file.write ((char*)&dibType,4);
  file.write ((char*)&size,4);
  file.write ((char*)&rW,4);
  file.write ((char*)&rH,4);
  file.write ((char*)&col,4);
  file.write ((char*)&iCol,4);

  file.write ((char*)data,size*4);

  file.close();
}

void pb::loadFromBitmap (string fileName) {
  fstream file (fileName.c_str(),ios::in|ios::binary);
  char buffer[2];

  unsigned int fileSize = size*4 + 14 + 40;
  unsigned int startPosition = 14 + 40;

  buffer[0] = 0x42;
  buffer[1] = 0x4D;

  //file.write (buffer,2);
  file.readsome (buffer,2);
  //file.write ((char*)&fileSize,4);
  file.readsome ((char*)&fileSize,4);

  buffer[0] = 0;
  buffer[1] = 0;
  //file.write (buffer,2);
  //file.write (buffer,2);
  file.readsome (buffer,2);
  file.readsome (buffer,2);

  //file.write ((char*)&startPosition,4);  

  file.readsome ((char*)&startPosition,4);

  unsigned int dibSize = 40;
  unsigned int dibType = 0;
  unsigned short cPlanes = 1;
  unsigned short dibBpp = 32;

  unsigned int rH = 1;
  unsigned int rW = 1;
  unsigned int col = 0;
  unsigned int iCol = 0;

  //file.write ((char*)&dibSize,4);
  //file.write ((char*)&width,4);
  //file.write ((char*)&height,4);
  //file.write ((char*)&cPlanes,2);
  //file.write ((char*)&dibBpp,2);
  //file.write ((char*)&dibType,4);
  //file.write ((char*)&size,4);
  //file.write ((char*)&rW,4);
  //file.write ((char*)&rH,4);
  //file.write ((char*)&col,4);
  //file.write ((char*)&iCol,4);

  file.readsome ((char*)&dibSize,4);
  file.readsome ((char*)&width,4);
  file.readsome ((char*)&height,4);
  file.readsome ((char*)&cPlanes,2);
  file.readsome ((char*)&dibBpp,2);
  file.readsome ((char*)&dibType,4);
  file.readsome ((char*)&size,4);
  file.readsome ((char*)&rW,4);
  file.readsome ((char*)&rH,4);
  file.readsome ((char*)&col,4);
  file.readsome ((char*)&iCol,4);

  //file.write ((char*)data,size*4);
  file.seekg (startPosition);
  file.readsome ((char*)data,size*4);

  file.close();
}
