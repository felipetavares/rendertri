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

#ifndef RI_PB_H
#define RI_PB_H

#include <string>

class pb {
public:
  unsigned int width;
  unsigned int height;
  unsigned int size;

  unsigned int *data;

  pb (unsigned int,unsigned int);
  ~pb();

  unsigned int getPixel (unsigned int,unsigned int);
  unsigned int setPixel (unsigned int,unsigned int,unsigned int);

  unsigned int fastGetPixel (unsigned int);
  unsigned int fastSetPixel (unsigned int,unsigned int);

  unsigned int compilePosition (unsigned int,unsigned int);

  void saveAsBitmap (std::string);
  void loadFromBitmap (std::string);
};

#endif /* RI_PB_H */
