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

#include "perspective.h"

pCamera::pCamera () {
  near = 1;
  transform.identity();
}

pCamera::pCamera (const double near) {
  this->near = near;
  transform.identity();
}

#include <iostream>
using namespace std;
vec2 pCamera::project (const vec3& vec) {
  vec4 p (vec,1);

  p = transform*p;

  double d = near/p.z;
  return vec2 {p.x/d,p.y/d};
}
