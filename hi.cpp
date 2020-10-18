/* 
	Remember to compile try:
		1) gcc hi.c -o hi -lX11
		2) gcc hi.c -I /usr/include/X11 -L /usr/X11/lib -lX11
		3) gcc hi.c -I /where/ever -L /who/knows/where -l X11

	Brian Hammond 2/9/96.    Feel free to do with this as you will!
*/


/* include the X library headers */
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>

/* include some silly stuff */
#include <stdio.h>
#include <stdlib.h>

/* here are our X variables */
Display *dis;
int screen;
Window win;
GC gc;

/* here are our X routines declared! */
void init_x();
void close_x();
void redraw();

int main (int argc, char** argv) {
	XEvent event;		/* the XEvent declaration !!! */
	KeySym key;		/* a dealie-bob to handle KeyPress Events */	
	char text[255];		/* a char buffer for KeyPress Events */

	init_x();

	/* look for events forever... */
	while(1) {		
		/* get the next event and stuff it into our event variable.
		   Note:  only events we set the mask for are detected!
		*/
		XNextEvent(dis, &event);
	
		if (event.type==Expose && event.xexpose.count==0) {
		/* the window was exposed redraw it! */
			redraw();
            printf("redraw\n");
		}
		if (event.type==KeyPress&&
		    XLookupString(&event.xkey,text,255,&key,0)==1) {
		/* use the XLookupString routine to convert the invent
		   KeyPress data into regular text.  Weird but necessary...
		*/
			if (text[0] == 'q' || (unsigned int)text[0] == 0x1b) {
				close_x();
			}
			printf("You pressed the %c key (%x) !\n",text[0],(unsigned int)text[0]);
		}
		if (event.type==ButtonPress) {
		/* tell where the mouse Button was Pressed */
			int x=event.xbutton.x,
			    y=event.xbutton.y;

			strcpy(text,"X is FUN!");
            XSetForeground(dis,gc,0xff0000); // red
            XSetBackground(dis,gc,0x000000);
			XDrawString(dis,win,gc,x,y, text, strlen(text));
            XRectangle rect = {.x=x, .y=y, .width=100, .height=100};
            //XDrawRectangles(dis,win,gc,&rect, 1);
            XFillRectangles(dis,win,gc,&rect, 1);

		}
        if (event.type==FocusOut) {
            close_x();
		}
        if (event.type==LeaveNotify) {
            close_x();
		}
        if (event.type==VisibilityNotify && (event.xvisibility.type == VisibilityPartiallyObscured || event.xvisibility.type == VisibilityFullyObscured)) {
            close_x();
		}
	}
    return 0;
}

void init_x() {
/* get the colors black and white (see section for details) */        
	unsigned long black,white;

	dis=XOpenDisplay((char *)0);
   	screen=DefaultScreen(dis);
	black=BlackPixel(dis,screen),
	white=WhitePixel(dis, screen);
   	win=XCreateSimpleWindow(dis,DefaultRootWindow(dis),0,0,	
		300, 300, 5,black, white);
	XSetStandardProperties(dis,win,"menu","menu",None,NULL,0,NULL);
	XSelectInput(dis, win, ExposureMask|ButtonPressMask|KeyPressMask|LeaveWindowMask|VisibilityChangeMask|FocusChangeMask);
        gc=XCreateGC(dis, win, 0,0);        
	XSetBackground(dis,gc,white);
	XSetForeground(dis,gc,black);
	XClearWindow(dis, win);
	XMapRaised(dis, win);
    printf("White pixel: %x\n", white);
    printf("Black pixel: %x\n", black);
};

void close_x() {
	XFreeGC(dis, gc);
	XDestroyWindow(dis,win);
	XCloseDisplay(dis);	
	exit(1);				
};

void redraw() {
    //XClearArea(dis, win, 0, 0, 100, 100, false);
	XClearWindow(dis, win);
};