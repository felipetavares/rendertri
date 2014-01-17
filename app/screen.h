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

#ifndef RI_SCREEN_H
#define RI_SCREEN_H

#include <X11/Xlib.h>

class wmScreen {
public:
	Display *x11display;
	Window  x11window;
	GC graph;
	XImage *screen;

	unsigned int cBlack;
	unsigned int cWhite;

	unsigned int width;
	unsigned int height;

	wmScreen(unsigned int, unsigned int, unsigned int*);
	~wmScreen();

	// Transfer data to screen
	void update ();
};

#endif /* RI_SCREEN_H */
