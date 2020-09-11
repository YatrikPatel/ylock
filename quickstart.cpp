#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>

Display *dis;
int screen;
Window win;
GC gc;



void init_x() {
	unsigned long black, white;

	dis = XOpenDisplay((char *) 0);
	screen = DefaultScreen(dis);
	black = BlackPixel(dis, screen);
	white = WhitePixel(dis, screen);

	Win = XCreateSimpleWindow(dis, DefaultRootWindow(dis), 0, 0, 200, 300, 5, white, black);
	
	XSetStandardProperties(dis,win,"My Window","HI!",None,NULL,0,NULL);

	XSelectInput(dis, win, ExposureMask|ButtonPressMask|KeyPressMask);

	        gc=XCreateGC(dis, win, 0,0);        

	XSetBackground(dis, gc, white);
	XSetForeground(dis, gc, black);
	
	XClearWindow(dis, win);
	XMapRaised(dis, win);
}

void close_x() {}
	XFreeGC(dis, gc);
	XDestroyWindow(dis, win);
	XCloseDisplay(dis);
	exit(1);
}

int main() {
	init_x();
	sleep(10);
	close_x();D
}
