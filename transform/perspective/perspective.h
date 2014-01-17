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

#ifndef RI_PERSPECTIVE_H
#define RI_PERSPECTIVE_H

/*
PERSPECTIVE PROJECTION

                             ^ z-
     x2                      |
|--------/| <- p             |
|       / |                  |
|      /  |                  |
|     /   |                  |
| x1 /    |f                 |
|---/     |                  |
|  /   |  | <- near plane    |
| /    |n |                  |
|/ <-e |  |                  |
                             |
                             |
-----------------------------> x

e  = eye
p  = point being projected
n  = distance to near plane (projection screen)
f  = distance to point
x1 = projected point x
x2 = projected point y

By this version, Render TRI Projection have no far plane, it renders to
-inf

x1/x2 = n/f;
x1 = x2*(n/f), the same thing applies to y axis.s

Bigger near, less angle to view.
Smaller near, more angle to view.

The eye position is fixed at origin.
 */

#include "../../math/vec2/vec2.h"
#include "../../math/vec3/vec3.h"
#include "../../math/vec4/vec4.h"
#include "../../math/sm4/sm4.h"

class pCamera {
public:
  //double far;
  sm4 transform;
  double near;

  pCamera ();
  pCamera (const double);

  vec2 project (const vec3&);  
};

#endif /* RI_PERSPECTIVE_H */
