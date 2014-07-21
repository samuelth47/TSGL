#include "CartesianCanvas.h"

CartesianCanvas::CartesianCanvas(unsigned int b)
    : Canvas(b) {
    recomputeDimensions(-400, -300, 400, 300);
}

CartesianCanvas::CartesianCanvas(int xx, int yy, int w, int h, Decimal xMin, Decimal yMin, Decimal xMax,
                                 Decimal yMax, unsigned int b, std::string t)
    : Canvas(xx, yy, w, h, b, t) {
    recomputeDimensions(xMin, yMin, xMax, yMax);
}

void CartesianCanvas::drawAxes(Decimal x, Decimal y, Decimal dx = 0, Decimal dy = 0) {
    drawLine(maxX, y, minX, y);  // Make the two axes
    drawLine(x, maxY, x, minY);
    if (dx != 0 && dy != 0) {
        for (int x = dx; x < maxX; x += dx) {
            drawLine(x, y + 4 * pixelHeight, x, y - 4 * pixelHeight);
        }
        for (int x = -dx; x > minX; x -= dx) {
            drawLine(x, y + 4 * pixelHeight, x, y - 4 * pixelHeight);
        }
        for (int y = dy; y < maxY; y += dy) {
            drawLine(x + 4 * pixelWidth, y, x - 4 * pixelWidth, y);
        }
        for (int y = -dy; y > minY; y -= dy) {
            drawLine(x + 4 * pixelWidth, y, x - 4 * pixelWidth, y);
        }
    }
}

void CartesianCanvas::drawCircle(Decimal x, Decimal y, Decimal radius, int res, RGBfloatType color, bool filled) {
    int actualX, actualY, actualR;
    getScreenCoordinates(x, y, actualX, actualY);
    getScreenCoordinates(x+radius,y,actualR,actualY);
    actualR -= actualX;

    Canvas::drawCircle(actualX, actualY, actualR, res, color, filled);
}

void CartesianCanvas::drawFunction(const Function* f) {
    int screenX = 0, screenY = 0;
    Polyline *p = new Polyline(1 + (maxX - minX) / pixelWidth);
    for (Decimal x = minX; x <= maxX; x += pixelWidth) {
        getScreenCoordinates(x, f->valueAt(x), screenX, screenY);
        p->addNextVertex(screenX, screenY);
    }

    drawShape(p);
}

void CartesianCanvas::drawImage(std::string fname, Decimal x, Decimal y, Decimal w, Decimal h, float a) {
    int actualX1, actualY1, actualX2, actualY2;
    getScreenCoordinates(x, y, actualX1, actualY1);
    getScreenCoordinates(x + w, y - h, actualX2, actualY2);

    Canvas::drawImage(fname, actualX1, actualY1, actualX2 - actualX1, actualY2 - actualY1, a);
}

void CartesianCanvas::drawLine(Decimal x1, Decimal y1, Decimal x2, Decimal y2, RGBfloatType color) {
    int actualX1, actualY1, actualX2, actualY2;
    getScreenCoordinates(x1, y1, actualX1, actualY1);
    getScreenCoordinates(x2, y2, actualX2, actualY2);

    Canvas::drawLine(actualX1, actualY1, actualX2, actualY2, color);
}

void CartesianCanvas::drawPoint(Decimal x, Decimal y, RGBfloatType color) {
    int actualX, actualY;
    getScreenCoordinates(x, y, actualX, actualY);

    Canvas::drawPoint(actualX, actualY, color);
}

void CartesianCanvas::drawRectangle(Decimal x, Decimal y, Decimal w, Decimal h, RGBfloatType color, bool filled) {
    int actualX1, actualY1, actualX2, actualY2;
    getScreenCoordinates(x, y, actualX1, actualY1);
    getScreenCoordinates(x + w, y + h, actualX2, actualY2);

    Canvas::drawRectangle(actualX1, actualY1, actualX2 - actualX1, actualY2 - actualY1, color, filled);
}

void CartesianCanvas::drawShinyPolygon(int size, int x[], int y[], RGBfloatType color[], bool filled) {
    for (int i = 0; i < size; i++) {
        getScreenCoordinates(x[i], y[i], x[i], y[i]);
    }

    Canvas::drawShinyPolygon(size, x, y, color, filled);
}

void CartesianCanvas::drawText(std::string s, Decimal x, Decimal y, RGBfloatType color) {
    int actualX, actualY;
    getScreenCoordinates(x, y, actualX, actualY);

    Canvas::drawText(s, actualX, actualY, color);
}

void CartesianCanvas::drawTriangle(int x1, int y1, int x2, int y2, int x3, int y3, RGBfloatType color, bool filled) {
    int actualX1, actualY1, actualX2, actualY2, actualX3, actualY3;
    getScreenCoordinates(x1, y1, actualX1, actualY1);
    getScreenCoordinates(x2, y2, actualX2, actualY2);
    getScreenCoordinates(x3, y3, actualX3, actualY3);

    Canvas::drawTriangle(actualX1, actualY1, actualX2, actualY2, actualX3, actualY3, color, filled);
}

void CartesianCanvas::getCartesianCoordinates(int screenX, int screenY, Decimal &cartX, Decimal &cartY) {
    cartX = (screenX * cartWidth) / getWindowWidth() + minX;
    cartY = minY - (screenY - getWindowHeight()) * cartHeight / getWindowHeight();
}

void CartesianCanvas::getScreenCoordinates(Decimal cartX, Decimal cartY, int &screenX, int &screenY) {
    screenX = ceil((cartX - minX) / cartWidth * getWindowWidth());
    screenY = ceil(getWindowHeight() - (cartY - minY) / cartHeight * getWindowHeight());
}

void CartesianCanvas::recomputeDimensions(Decimal xMin, Decimal yMin, Decimal xMax, Decimal yMax) {
    minX = xMin;
    minY = yMin;
    maxX = xMax;
    maxY = yMax;
    cartWidth = maxX - minX;
    cartHeight = maxY - minY;
    Decimal xError = cartWidth / getWindowWidth();
    Decimal yError = cartHeight / getWindowHeight();
    pixelWidth = (cartWidth - xError) / (getWindowWidth() + xError);
    pixelHeight = (cartHeight - yError) / (getWindowHeight() + yError);
}

void CartesianCanvas::zoom(Decimal x, Decimal y, Decimal scale) {
    Decimal newWidth = cartWidth * scale;
    Decimal newHeight = cartHeight * scale;
    recomputeDimensions(x - .5 * newWidth, y - .5 * newHeight, x + .5 * newWidth, y + .5 * newHeight);
}

void CartesianCanvas::zoom(Decimal x1, Decimal y1, Decimal x2, Decimal y2) {
    Decimal scale = (std::abs(x2 - x1) / cartWidth + std::abs(y2 - y1) / cartHeight) / 2.0;
    zoom((x2 + x1) / 2, (y2 + y1) / 2, scale);
}
