// Canvas provides a window / canvas for all of the drawing operations in the graphicOMP library
//
// Last Modified: Patrick Crain, 6/5/2014

#ifndef CANVAS_H_
#define CANVAS_H_

#include <FL/Fl_Double_Window.H>	//For Fl_Double_Window, which draws our window
#include <FL/Fl_Box.H>				//For Fl_Box, from which our Canvas inherits
#include <FL/fl_draw.H>				//For FLTK's drawing function, which we implement to make our own thread-safe version.
#include "Point.h"					//Our own class for drawing single points.
#include <omp.h>					//For OpenMP support
#include "CQueue.h"					//Our own concurrent queue for buffering drawing operations
//#include "List.h"					//UNFINISHED * Our own doubly-linked list, providing the same support as the above queue, with iteration

const double FRAME = 1.0f/60.0f;	//Represents a single frame (@ 60Hz)

class Canvas : public Fl_Box {
	typedef void (*fcall)(void);	//Define a type for our callback function pointer
private:
	Queue<Shape*> myShapes;			//Our queue of shapes to draw
	int counter;					//Counter for the number of frames that have elapsed in the current session (for animations)
	int queueSize;					//The length of our current (concurrent) queue
	int x,y,w,h;  					//Positioning and sizing data for the Canvas
	int colorR, colorG, colorB; 	//Our current global RGB drawing color
	Fl_Double_Window *window;		//The FLTK window to which we draw
	bool started;					//Whether our canvas is running and the counter is counting
	fcall updateFunc;				//User-defined callback function for drawing
	inline void init(fcall c, int xx, int yy, int ww, int hh);	//Method for initializing the canvas
	inline void draw();											//Method for drawing the canvas and the shapes within
	inline static void Canvas_Callback(void *userdata);			//Callback so that the canvas redraws periodically
public:
	inline Canvas(fcall c);											//Default constructor for our Canvas
	inline Canvas(fcall c, int xx, int yy, int w, int h, char* t);	//Explicit constructor for our Canvas
	inline int start();													//Function to start rendering our Canvas
	inline int end();													//Function to end rendering our Canvas
	inline void setColor(int r, int g, int b);							//Sets the global drawing color
	inline Point drawPoint(int x, int y);								//Draws a point at the given coordinates
	inline Point drawPointColor(int x, int y, int r, int g, int b);		//Draws a point at the given coordinates with the given color
	inline int getColorR();												//Gets the red component of the global drawing color
	inline int getColorG();												//Gets the green component of the global drawing color
	inline int getColorB();												//Gets the blue component of the global drawing color
	inline int getFrameNumber();										//Accessor for the number of frames rendered so far
};

/*
 * init initialized the Canvas with the values specified in the constructor
 * Parameters:
 * 		xx, the x position of the Canvas window
 * 		yy, the y position of the Canvas window
 * 		width, the x dimension of the Canvas window
 * 		width, the y dimension of the Canvas window
 */
void Canvas::init(fcall c, int xx, int yy, int ww, int hh) {
	started = false;  	//We haven't started the window yet
	counter = 0;		//We haven't drawn any frames yet
	queueSize = 0;		//Our drawing queue is empty
	x = xx; y = yy; w = ww; h = hh;  				//Initialize translation
	box(FL_FLAT_BOX);  										//Sets the box we will draw to (the only one)
	setColor(0,0,0);										//Our default global drawing color is black
	updateFunc = c;
	Fl::add_timeout(FRAME, Canvas_Callback, (void*)this);  	//Adds a callback after 1/60 second to our callback function
}

/*
 * draw actually draws our Canvas and its contents to the display
 * Note: this function is called automatically by the callback and the FLTK redraw function, which is why it's private
 */
void Canvas::draw() {
	counter++;				//Increment the frame counter

	updateFunc();			//Call our callback

	int ql = queueSize;		//Temporary variable for our initial queue size
	//Temporary variables for the initial global drawing color
	int oldR = colorR;
	int oldG = colorG;
	int oldB = colorB;
	Shape *s;				//Pointer to the next Shape in the queue
	while (ql--  > 0) {		//Iterate through our queue until we've made a full cycle
		s = myShapes.pop();	//Pop each item
		if (s->getUsesDefaultColor()) {
			s->draw();		//If our shape uses the default color, just draw it
		}
		else {				//Otherwise, the color must be set before and after drawing
			setColor(s->getColorR(),s->getColorG(),s->getColorB());
			s->draw();
			setColor(oldR, oldG, oldB);
		}
		myShapes.push(s);	//Add the Shape back to the end of the queue
	}
}

/*
 * Canvas_Callback is a callback function called (by default) every 1/60 of a second (update cycle) to redraw the canvas
 * Parameters:
 * 		userdata, a pointer to the Canvas class that we're calling back
 */
void Canvas::Canvas_Callback(void *userdata) {
	Canvas *o = (Canvas*)userdata;  						//Casts the userdata pointer as a Canvas pointer
    o->redraw();  											//Redraw the canvas
    Fl::repeat_timeout(FRAME, Canvas_Callback, userdata);  	//Restart the fcall
}

/*
 * Default constructor for the canvas class
 * Returns: a new 800x600 Canvas on the topleft of the screen with no title
 */
Canvas::Canvas(fcall c) : Fl_Box (0,0,800,600) {
	init(c,0,0,800,600);
}

/*
 * Explicit constructor for the canvas class
 * Parameters:
 * 		xx, the x position of the Canvas window
 * 		yy, the y position of the Canvas window
 * 		w, the x dimension of the Canvas window
 * 		h, the y dimension of the Canvas window
 * 		t, the title of the window
 * Returns: a new Canvas with the specified positional data and title
 */
Canvas::Canvas(fcall c, int xx, int yy, int w, int h, char* t = 0) : Fl_Box(xx,yy,w,h,t) {
	init(c,xx,yy,w,h);
}

/*
 * start starts rendering the Canvas
 * Returns: the exit code of the FLTK render method
 */
int Canvas::start() {
	if (started)						//If we're already started, return error code -1
		return -1;
	started = true;						//We've now started
    window = new Fl_Double_Window(w,h);	//Instantiate our drawing window
    window->add(this);					//Add ourself (Canvas) to the drawing window
    window->show();						//Show the window
    return(Fl::run());
}

/*
 * start starts rendering the Canvas
 * Returns: 0 if exit is successful, -1 otherwise
 */
int Canvas::end() {
	if (!started)	//If we haven't even started yet, return error code -1
		return -1;
	window->hide();	//Hide our window
	delete window;	//Delete our window from the heap
	return 0;
}

/*
 * setColor sets the global drawing color
 * Parameters:
 * 		r, the red component
 * 		g, the red component
 * 		b, the red component
 */
void Canvas::setColor(int r, int g, int b) {
	colorR = r;
	colorG = g;
	colorB = b;
	fl_color(r,g,b);	//Updates the underlying FLTK color
}

/*
 * drawPoint draws a point at the given coordinates
 * Parameters:
 * 		x, the x position of the point
 * 		y, the y position of the point
 * 	Returns: a new point at the given position
 */
Point Canvas::drawPoint(int x, int y) {
	Point *p = new Point(x,y);	//Creates the Point with the specified coordinates
	myShapes.push(p);			//Push it onto our drawing queue
	#pragma omp atomic
	queueSize++;				//Atomically increment the size of our queue
	return *p;					//Return a pointer to our new Point
}

/*
 * drawPointColor draws a point at the given coordinates with the given color
 * Parameters:
 * 		x, the x position of the point
 * 		y, the y position of the point
 * 		r, the red component
 * 		g, the red component
 * 		b, the red component
 * 	Returns: a new point at the given position with the specificed color
 */
Point Canvas::drawPointColor(int x, int y, int r, int g, int b) {
	Point *p = new Point(x,y,r,g,b);	//Creates the Point with the specified coordinates and color
	myShapes.push(p);					//Push it onto our drawing queue
	#pragma omp atomic
	queueSize++;						//Atomically increment the size of our queue
	return *p;							//Return a pointer to our new Point
}

// Accessor functions for the individual components of the global drawing color
int Canvas::getColorR() { return colorR; }
int Canvas::getColorG() { return colorG; }
int Canvas::getColorB() { return colorB; }

// Accessor function for the current render frame number
int Canvas::getFrameNumber() { return counter; }

#endif /* CANVAS_H_ */