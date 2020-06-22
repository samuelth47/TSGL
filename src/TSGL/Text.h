/*
 * Text.h extends Drawable and provides a class for drawing a string of text to the Canvas.
 */

#ifndef TEXT_H_
#define TEXT_H_

#include "Drawable.h"          // For extending our Shape object
#include <iostream>
#include <ft2build.h>
#include FT_FREETYPE_H
// #include "TextureHandler.h"

namespace tsgl {

/*! \class Text
 *  \brief Draw a string of text.
 *  \details Text is a class for holding the data necessary for rendering a string of text.
 *  \note Text is aligned by the upper-left corner.
 *  \note Fonts supported by FreeType are also supported.
 */
class Text : public Drawable {
 private:
    std::string myString;
    unsigned int myFontSize;
    std::string myFont;
    ColorFloat myColor;

    GLfloat myWidth;
    GLfloat myHeight;

    FT_Face face;
    FT_Library ft;

    void calculateDimensions();
 public:
    Text(float x, float y, float z, std::string text, std::string fontFilename, unsigned int fontsize, float yaw, float pitch, float roll, const ColorFloat &color);

    virtual void draw(Shader * shader);

    virtual void setText(std::string text);

    virtual void setFontSize(unsigned int fontsize);

    virtual void setFont(std::string filename);

    virtual void setColor(const ColorFloat& color);

    std::string getText() { return myString; }

    unsigned int getFontSize() { return myFontSize; }

    ~Text();
};

}

#endif /* TEXT_H_ */