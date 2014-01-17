#  Copyright (C) 2013 by Felipe Tavares
#
#  This file is part of Render TRI.
#
#  Render TRI is free software: you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation, either version 3 of the License, or
#  (at your option) any later version.
#
#  Render TRI is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with Render TRI.  If not, see <http://www.gnu.org/licenses/>. 

FAST=-lX11 -O3
DEBG=-lX11 -g

F=$(FAST)

all: ri1.0

ri1.0: vec2.o vec3.o vec4.o sm4.o persp.o raster.o pixelbuffer.o screen.o render.o main.cpp
	g++ -std=c++0x main.cpp vec2.o vec3.o vec4.o sm4.o persp.o raster.o pixelbuffer.o screen.o render.o -o ri1.0 $(F)
vec2.o: math/vec2/vec2.cpp math/vec2/vec2.h
	g++ -std=c++0x math/vec2/vec2.cpp -o vec2.o -c $(F)
vec3.o: math/vec3/vec3.cpp math/vec3/vec3.h
	g++ -std=c++0x math/vec3/vec3.cpp -o vec3.o -c $(F)
vec4.o: math/vec4/vec4.cpp math/vec4/vec4.h
	g++ -std=c++0x math/vec4/vec4.cpp -o vec4.o -c $(F)
sm4.o: math/sm4/sm4.cpp math/sm4/sm4.h
	g++ -std=c++0x math/sm4/sm4.cpp -o sm4.o -c $(F)
persp.o: transform/perspective/perspective.cpp transform/perspective/perspective.h
	g++ -std=c++0x transform/perspective/perspective.cpp -o persp.o -c $(F)
raster.o: rasterizer/triangle.cpp rasterizer/triangle.h
	g++ -std=c++0x rasterizer/triangle.cpp -o raster.o -c $(F)
pixelbuffer.o: rasterizer/pb.cpp rasterizer/pb.h
	g++ -std=c++0x rasterizer/pb.cpp -o pixelbuffer.o -c $(F)
screen.o: app/screen.cpp app/screen.h
	g++ -std=c++0x app/screen.cpp -o screen.o -c $(F)
render.o: render/render.cpp render/render.h
	g++ -std=c++0x render/render.cpp -o render.o -c $(F)
clear:
	rm *.o
