#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include <shadow.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>

Display *dis;
int screen;
Window win;
GC gc;

void init_x(); // start X server connection
void close_x(); // end X server connection
void redraw(); // redraw the window

uid_t euid;
uid_t ruid;  // effective and real uid

void do_setuid (void) {
	int status;

#ifdef _POSIX_SAVED_IDS
	status = seteuid (0);
#else
	printf("Hi");
	status = setreuid (ruid, euid);
#endif
	if (status < 0) {
		fprintf (stderr, "Couldn't set uid.\n");
		exit (status);
	}
}

static const char* getHash() {
	const char* hash;
	struct passwd* password;
	struct spwd* shadowPassword;
	password = getpwuid(getuid());

	hash = password->pw_passwd;
	shadowPassword = getspnam(password->pw_name);
	hash = shadowPassword->sp_pwdp;
	printf("%s", hash);
}

int main() {
	ruid = getuid();
	euid = geteuid();
	XEvent event;		/* the XEvent declaration !!! */
	KeySym key;		/* a dealie-bob to handle KeyPress Events */	
	char text[255];	
	char password[255];
	int length = 0;
	printf("%i, %i", ruid, euid);
	init_x();

	while(1) {		
		/* get the next event and stuff it into our event variable.
Note:  only events we set the mask for are detected!
		 */
		XNextEvent(dis, &event);

		if (event.type==Expose && event.xexpose.count==0) {
			/* the window was exposed redraw it! */
			redraw();
		}
		KeySym keysym =XLookupString(&event.xkey,text,255,&key,0); 
		if (event.type==KeyPress&& keysym==1) {
			if (IsFunctionKey(keysym) ||
			IsKeypadKey(keysym) ||
			IsMiscFunctionKey(keysym) ||
			IsPFKey(keysym) ||
			IsPrivateKeypadKey(keysym))
				continue;
			if (keysym == XK_BackSpace) {
				if (length != 0) password[--length] = '\0';
			}
			/* use the XLookupString routine to convert the invent
			   KeyPress data into regular text.  Weird but necessary...
			 */
			printf("You pressed the %c key!\n",text[0]);
			password[length++] = text[0];
			printf("%s", password);
		}
		if (event.type==ButtonPress) {
			/* tell where the mouse Button was Pressed */
			int x=event.xbutton.x,
			    y=event.xbutton.y;

			strcpy(text,"X is FUN!");
			XSetForeground(dis,gc,rand()%event.xbutton.x%255);
			XDrawString(dis,win,gc,x,y, text, strlen(text));
		}
	}

	do_setuid();
	printf("%i, %i", ruid, euid);
	getHash();
	return 0;
}

void init_x() {
	unsigned long black, white;
	dis = XOpenDisplay((char *) 0);
	screen = DefaultScreen(dis);
	black = BlackPixel(dis, screen);
	white = WhitePixel(dis, screen);
	win = XCreateSimpleWindow(dis, DefaultRootWindow(dis), 0, 0, 300, 300, 5, black, white);
	XSetStandardProperties(dis,win,"Howdy","Hi",None,NULL,0,NULL);
	XSelectInput(dis, win, ExposureMask|ButtonPressMask|KeyPressMask);
	gc=XCreateGC(dis, win, 0,0);        
	XSetBackground(dis,gc,white);
	XSetForeground(dis,gc,black);
	XClearWindow(dis, win);
	XMapRaised(dis, win);
}

void close_x() {
	XFreeGC(dis, gc);
	XDestroyWindow(dis, win);
	XCloseDisplay(dis);
	exit(1);
};

void redraw() {
	XClearWindow(dis, win);
};
