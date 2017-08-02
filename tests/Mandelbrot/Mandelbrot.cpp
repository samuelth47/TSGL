/*
 * Mandelbrot.cpp
 */

#include "Mandelbrot.h"

Mandelbrot::Mandelbrot(unsigned threads, unsigned depth) {
    myThreads = threads;
    myDepth = depth;
    myFirstX = myFirstY = mySecondX = mySecondY = 0.0;
    myRedraw = true;
}

void Mandelbrot::manhattanShading(CartesianRasterCanvas& can) {
  int** canPoints = new int*[can.getWindowHeight()];
  for (int i = 0; i < can.getWindowHeight(); ++i) {
    canPoints[i] = new int[can.getWindowWidth()];
    for (int j = 0; j < can.getWindowWidth(); ++j) {
      ColorInt c = can.getPoint(j,i);
      canPoints[i][j] = ((c.R == c.G) && (c.G == c.B) && (c.B == 0)) ? 0 : -1;
    }
  }
  bool done = false;
  int loop, cwh = can.getWindowHeight(), cww = can.getWindowWidth();
  for (loop = 0; !done; ++loop) {
    done = true;
    #pragma omp parallel for
    for (int i = 0; i < cwh; ++i) {
      for (int j = 0; j < cww; ++j) {
        if (canPoints[i][j] != loop)
          continue;
        if (i > 0 && canPoints[i-1][j] == -1)
          canPoints[i-1][j] = loop+1, done = false;
        if (j > 0 && canPoints[i][j-1] == -1)
          canPoints[i][j-1] = loop+1, done = false;
        if (i < cwh-1 && canPoints[i+1][j] == -1)
          canPoints[i+1][j] = loop+1, done = false;
        if (j < cww-1 && canPoints[i][j+1] == -1)
          canPoints[i][j+1] = loop+1, done = false;
      }
    }
  }

  int sum = 0;
  for (int i = 0; i < cwh; ++i)
    for (int j = 0; j < cww; ++j)
      sum += canPoints[i][j];
  float avg = (((float)sum)/cww)/cwh;

  #pragma omp parallel for
  for (int i = 0; i < cwh; ++i) {
    for (int j = 0; j < cww; ++j) {
      float mult = sqrt(avg*((float)canPoints[i][j])/loop);
      ColorFloat c = can.getPoint(j,i);
      Decimal x, y;
      can.getCartesianCoordinates(j, i, x, y);
      can.drawPoint(x,y,c*mult);
    }
  }

  for (int i = 0; i < can.getWindowHeight(); ++i)
    delete [] canPoints[i];
  delete [] canPoints;
  canPoints = NULL;
}

void Mandelbrot::bindings(CartesianRasterCanvas& can) {
  can.bindToButton(TSGL_SPACE, TSGL_PRESS, [&can, this]() {
    can.clear();
    this->myRedraw = true;
  });
  can.bindToButton(TSGL_MOUSE_LEFT, TSGL_PRESS, [&can, this]() {
    can.getCartesianCoordinates(can.getMouseX(), can.getMouseY(), this->myFirstX, this->myFirstY);
  });
  can.bindToButton(TSGL_MOUSE_LEFT, TSGL_RELEASE, [&can, this]() {
    can.getCartesianCoordinates(can.getMouseX(), can.getMouseY(), this->mySecondX, this->mySecondY);
    if (!(this->myFirstX == this->mySecondX || this->myFirstY == this->mySecondY)) {
      can.zoom(this->myFirstX, this->myFirstY, this->mySecondX, this->mySecondY);
      this->myRedraw = true;
    }
  });
  can.bindToButton(TSGL_MOUSE_RIGHT, TSGL_PRESS, [&can, this]() {
    Decimal x, y;
    can.getCartesianCoordinates(can.getMouseX(), can.getMouseY(), x, y);
    can.zoom(x, y, 1.5);
    this->myRedraw = true;
  });
  can.bindToScroll([&can, this](double dx, double dy) {
    Decimal x, y;
    can.getCartesianCoordinates(can.getMouseX(), can.getMouseY(), x, y);
    Decimal scale;
    if (dy == 1) scale = 0.5;
    else scale = 2;
    if( scale > 1 || !(can.getCartHeight() < 0.0003f) ) {
      can.zoom(x, y, scale);
      this->myRedraw = true;
    }
  });
}

void Mandelbrot::draw(CartesianRasterCanvas& can) {
  const int CH = can.getWindowHeight();   //Height of our Mandelbrot canvas
  const int CW = can.getWindowWidth();    //Width
  while(myRedraw) {
    myRedraw = false;
    can.reset();
    #pragma omp parallel num_threads(myThreads)
    {
      unsigned tid = omp_get_thread_num();
      unsigned nthreads = omp_get_num_threads();
      long double blocksize = can.getCartHeight() / nthreads;
      long double blockheight = CH / nthreads;
      long double startrow = blocksize * tid + can.getMinY();
      long double startcol = can.getMinX();
      for(long double row = startrow; row < startrow + blocksize; row += can.getPixelHeight()) {
        for(long double col = startcol; col <= can.getMaxX(); col += can.getPixelWidth()) {
          complex originalComplex(col, row);
          complex z(col, row);
          unsigned iterations = 0;
          while (std::abs(z) < 2.0 && iterations <= myDepth) {  // Compute it until it escapes or we give up
            iterations++;
            z = z * z + originalComplex;
          }
          if(iterations >= myDepth) { // If the point never escaped, draw it black
            can.drawPoint(col, row, BLACK);
          } else { // Otherwise, draw it with color based on how long it took
            can.drawPoint(col, row, ColorHSV(iterations*6.0f/myDepth, 1.0f, 0.6f, 1.0f));
          }
          if (myRedraw) break;
        }
        can.handleIO();
        if (myRedraw) break;
      }
    }
    printf("%f seconds to draw\n", can.getTime());
    printf("%Lfx scale\n", 1/(can.getCartHeight()/2));
    while (can.isOpen() && !myRedraw) {
      can.sleep(); //Removed the timer and replaced it with an internal timer in the Canvas class
    }
  }
}
