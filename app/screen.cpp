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

#include "screen.h"

#include <unistd.h>

wmScreen::wmScreen (unsigned int w,unsigned int h, unsigned int *data) {
  width = w;
  height = h;
  
  x11display = XOpenDisplay (NULL);
  
  cBlack = BlackPixel (x11display, DefaultScreen (x11display));
  cWhite = WhitePixel (x11display, DefaultScreen (x11display));
  
  x11window  = XCreateSimpleWindow (x11display, DefaultRootWindow(x11display),
				    0,0,
				    w,h,
				    0,
				    cBlack,cBlack);
  
  XSelectInput (x11display, x11window, StructureNotifyMask);
  
  XStoreName(x11display, x11window, "RenderTRI");
  
  XMapWindow (x11display, x11window);
  
  graph = XCreateGC (x11display, x11window, 0, NULL);
  
  for(;;) {
    XEvent e;
    XNextEvent(x11display, &e);
    if (e.type == MapNotify)
      break;
  }
  
  screen = XCreateImage (x11display, CopyFromParent, 24, ZPixmap, 0, (char*)data,
			 width, height,
			 8, 0);
   
  XFlush(x11display);
}

wmScreen::~wmScreen () {
  XCloseDisplay (x11display);
}

void wmScreen::update () {
  XPutImage (x11display, x11window, graph ,screen,0,0, 0,0 ,width,height);
  XFlush(x11display);
}
