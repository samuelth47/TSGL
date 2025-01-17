// #include "TextureHandler.h"

// namespace tsgl {

//   //The instructions for the stb library say to define it exactly once in a .c or .cpp file (NOT a .h file)
// //   #ifndef STB_DEFINE
// //     #define STB_IMAGE_WRITE_IMPLEMENTATION
// //     #include "stb/stb_image_write.h"
// //     #define STB_IMAGE_IMPLEMENTATION
// //     #include "stb/stb_image.h"
// //     #define STB_DEFINE
// // //It may look truly awful....but its an easy way to turn off warnings
// // //solely for stb.h. Sorry :'(
// // // #pragma GCC diagnostic push
// // // #pragma GCC diagnostic ignored "-fpermissive"
// // // #pragma GCC diagnostic push
// // // #pragma GCC diagnostic ignored "-Wmissing-field-initializers"
// // // #pragma GCC diagnostic push
// // // #pragma GCC diagnostic ignored "-Wsign-compare"
// // // #pragma GCC diagnostic push
// // // #pragma GCC diagnostic ignored "-Wstrict-aliasing"
// // // #pragma GCC diagnostic push
// // // #pragma GCC diagnostic ignored "-Wwrite-strings"
// // // #pragma GCC diagnostic push
// // // #pragma GCC diagnostic ignored "-Wint-to-pointer-cast"
// // // #pragma GCC diagnostic push
// // // #pragma GCC diagnostic ignored "-Wtype-limits"
// // // #pragma GCC diagnostic push
// // // #pragma GCC diagnostic ignored "-Wunused-but-set-variable"
// // // #pragma GCC diagnostic push
// // // #pragma GCC diagnostic ignored "-Wunused-result"
// // //   #include "stb/stb.h"
// // // #pragma GCC diagnostic pop
// // // #pragma GCC diagnostic pop
// // // #pragma GCC diagnostic pop
// // // #pragma GCC diagnostic pop
// // // #pragma GCC diagnostic pop
// // // #pragma GCC diagnostic pop
// // // #pragma GCC diagnostic pop
// // // #pragma GCC diagnostic pop
// // // #pragma GCC diagnostic pop
// //   #endif

//  //List of default fonts to check for
// #ifndef DEFAULTFONTS
// #define DEFAULTFONTS
// #ifdef _WIN32
//     const char* DEFAULTFONTPATHS[] = {
//         "../assets/freefont/FreeSerif.ttf",
//         "./assets/freefont/FreeSerif.ttf",
//         "./FreeSerif.ttf",
//         "C:\\Windows\\Fonts\\ARIALUNI.ttf",
//         "C:\\Windows\\Fonts\\ARIAL.ttf",
//         "C:\\Windows\\Fonts\\COUR.ttf",
//         "C:\\Windows\\Fonts\\COURI.ttf"
//     };
// #endif
// #ifdef __CYGWIN__
//     const char* DEFAULTFONTPATHS[] = {
//         "../assets/freefont/FreeSerif.ttf",
//         "./assets/freefont/FreeSerif.ttf",
//         "./FreeSerif.ttf",
//         "C:\\Windows\\Fonts\\ARIALUNI.ttf",
//         "C:\\Windows\\Fonts\\ARIAL.ttf",
//         "C:\\Windows\\Fonts\\COUR.ttf",
//         "C:\\Windows\\Fonts\\COURI.ttf"
//     };
// #endif
// #ifdef __APPLE__
//     const char* DEFAULTFONTPATHS[] = {
//         "../assets/freefont/FreeSerif.ttf",
//         "./assets/freefont/FreeSerif.ttf",
//         "./FreeSerif.ttf",
//         "/Library/Fonts/Arial.ttf",
//         "/Library/Fonts/Courier New.ttf",
//         "/Library/Fonts/Georgia.ttf",
//         "/opt/X11/share/fonts/TTF/VeraSe.ttf",
//     };
// #endif
// #ifdef __linux__
//     const char* DEFAULTFONTPATHS[] = {
//         "../assets/freefont/FreeSerif.ttf",
//         "./assets/freefont/FreeSerif.ttf",
//         "./FreeSerif.ttf",
//         "/usr/share/fonts/dejavu/DejaVuSerif.ttf",
//         "/usr/share/fonts/truetype/dejavu/DejaVuSerif.ttf",
//         "/usr/share/fonts/TTF/DejaVuSerif.ttf",
//         "/usr/share/fonts/TTF/arial.ttf",
//         "/usr/share/fonts/TTF/cour.ttf",
//         "/usr/share/fonts/TTF/couri.ttf"
//     };
// #endif
// #endif

// #define GL_GLEXT_PROTOTYPES

// /*!
//  * \brief Default TextureHandler constructor method.
//  * \details This is the default constructor for theTextureHandler Canvas class.
//  * \return A new TextureHandler instance.
//  */
// TextureHandler::TextureHandler() {
//     fontLibrary = nullptr;
//     fontFace = nullptr;
// }

// /*!
//  * \brief TextureHandler destructor method.
//  * \details This is the destructor for the TextureHandler class.
//  * \details Frees up memory that was allocated to a TextureHandler instance.
//  */
// TextureHandler::~TextureHandler() {
//     for (TextureMap::iterator it = loadedTextures.begin(); it != loadedTextures.end(); ++it) {
//         glDeleteTextures(1, &(it->second));
//     }

//     for (FontMap::iterator it = loadedFonts.begin(); it != loadedFonts.end(); ++it) {
//         FT_Done_Face(it->second);
//     }
//     FT_Done_FreeType(fontLibrary);
// }

// void TextureHandler::createGLtextureFromBuffer(GLtexture &texture, unsigned char* buffer,
//                                                const unsigned int &width, const unsigned int &height,
//                                                int glMode) {
//     // Generate the OpenGL texture object
//     glGenTextures(1, &texture);
//     glBindTexture(GL_TEXTURE_2D, texture);

//     if (glMode == GL_ALPHA) {
//         unsigned char* newBuffer = new unsigned char[width * height * 4];
//         unsigned maxSize = width * height;
//         for (unsigned int i = 0, x = 0; i < maxSize; i++, x += 4) {
//             newBuffer[x] = newBuffer[x + 1] = newBuffer[x + 2] = newBuffer[x + 3] = buffer[i];
//         }

//         glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
//         glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, newBuffer);
//         delete[] newBuffer;
//     } else {
//         if (glMode == GL_RED) {
//             glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
//         } else {
//             glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
//         }
//         glTexImage2D(GL_TEXTURE_2D, 0, glMode, width, height, 0, glMode, GL_UNSIGNED_BYTE, buffer);
//     }

//     glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//     glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
// }

// void TextureHandler::drawGLtextureFromBuffer(GLubyte* buffer, int x, int y, unsigned int width, unsigned int height, int glMode) {
//     // Generate the OpenGL texture object
//     GLtexture texture;

//     glGenTextures(1, &texture);
//     glBindTexture(GL_TEXTURE_2D, texture);

//     if (glMode == GL_ALPHA) {
//         unsigned char* newBuffer = new unsigned char[width * height * 4];
//         unsigned maxSize = width * height;
//         for (unsigned int i = 0, x = 0; i < maxSize; i++, x += 4) {
//             newBuffer[x] = newBuffer[x + 1] = newBuffer[x + 2] = newBuffer[x + 3] = buffer[i];
//         }

//         glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
//         glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, newBuffer);
//         delete[] newBuffer;
//     } else {
//         if (glMode == GL_RED) {
//             glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
//         } else {
//             glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
//         }
//         glTexImage2D(GL_TEXTURE_2D, 0, glMode, width, height, 0, glMode, GL_UNSIGNED_BYTE, buffer);
//     }

//     glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//     glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

//     float* vertices = new float[32];
//     vertices[0] = x;
//     vertices[1] = y + height;
//     vertices[8] = x + width;
//     vertices[9] = y + height;
//     vertices[16] = x + width;
//     vertices[17] = y;
//     vertices[24] = x;
//     vertices[25] = y;
//     vertices[2] = vertices[10] = vertices[18] = vertices[26] = 1.0f;  // Texture color of the coords
//     vertices[3] = vertices[11] = vertices[19] = vertices[27] = 1.0f;
//     vertices[4] = vertices[12] = vertices[20] = vertices[28] = 1.0f;
//     vertices[5] = vertices[13] = vertices[21] = vertices[29] = 1.0f;
//     vertices[6] = vertices[7] = 0.0f;           // Texture coords of top left
//     vertices[14] = 1.0f, vertices[15] = 0.0f;   // Texture coords of top right
//     vertices[30] = 0.0f, vertices[31] = 1.0f;   // Texture coords of bottom left
//     vertices[22] = vertices[23] = 1.0f;         // Texture coords of bottom right

//     // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//     // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//     // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//     // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
//     glBufferData(GL_ARRAY_BUFFER, 32 * sizeof(float), vertices, GL_DYNAMIC_DRAW);
//     glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

//     glDeleteTextures(1, &texture);

//     delete[] vertices;
// }

// /*!
//  * \brief Draws text.
//  * \details Draws the text specified by its parameters onto a Canvas.
//  *   \param text The UTF-8 encoded string of text to be drawn.
//  *   \param font_size The size of the text in pixels.
//  *   \param vertices An array of vertex data for the bonding box of the text.
//  * \note <code>vertices</code> will be partially automatically set by drawText()
//  *   itself in order to draw / kern the text properly, but the color, starting
//  *   position, and texture coordinates will be left unchanged.
//  * \note If no font is loaded before calling this function, TSGL will attempt to locate a
//  *   default font at <i>../assets/freefont/FreeMono.ttf.</i>
//  * \return True if successful, false otherwise.
//  * \bug If the default font cannot be located, TSGL will crash.
//  */
// bool TextureHandler::drawText(std::string text, unsigned int font_size, float* vertices) {
//     // const wchar_t* string = text.c_str();
//     if(fontFace == nullptr) {   //If no font is set, load up a default one
//       bool found = false;
//       for (unsigned int i = 0; i < sizeof(DEFAULTFONTPATHS)/sizeof(*DEFAULTFONTPATHS); ++i) {
//           if (fileExists(DEFAULTFONTPATHS[i])) {
//               TsglDebug("No Font set! Now loading from " + std::string(DEFAULTFONTPATHS[i]));    //NEW
//               loadFont(DEFAULTFONTPATHS[i]);
//               found = true;
//               break;
//           }
//       }
//       if (!found) {
//           TsglErr("No suitable fonts found...exiting");    //NEW
//           exit(44);
//       }
//     }

//     FT_GlyphSlot glyph = fontFace->glyph;
//     FT_UInt current_glyph_index, previous_glyph_index = 0;
//     int penX = vertices[0],
//         penY = vertices[1];

//     bool error = FT_Set_Pixel_Sizes(fontFace, 0, font_size);
//     if (error) {
//         fprintf(stderr, "FT_Set_Pixel_Sizes failed\n");
//         return false;
//     }

//     bool use_kerning = FT_HAS_KERNING(fontFace);

//     for (unsigned int i = 0; i < text.size(); i++) {
//         current_glyph_index = FT_Get_Char_Index(fontFace, text[i]);

//         if (use_kerning && previous_glyph_index && current_glyph_index) {
//             FT_Vector delta;
//             FT_Get_Kerning(fontFace, previous_glyph_index, current_glyph_index, FT_KERNING_DEFAULT, &delta);
//             penX += delta.x >> 6;
//             penY += delta.y >> 6;
//         }

//         error = FT_Load_Glyph(fontFace, current_glyph_index, FT_LOAD_RENDER);
//         if (error) {
//             fprintf(stderr, "FT_Load_Char failed\n");
//             return false;
//         }

//         previous_glyph_index = current_glyph_index;

//         int glMode = GL_ALPHA;

//         char fontMode = glyph->bitmap.pixel_mode;
//         if (fontMode == FT_PIXEL_MODE_MONO)
//            glMode = GL_RED;
//         else if (fontMode == FT_PIXEL_MODE_GRAY)
//             glMode = GL_ALPHA;
//         else if (fontMode == FT_PIXEL_MODE_LCD)
//             glMode = GL_RGB;
//         else if (fontMode == FT_PIXEL_MODE_LCD_V)
//             glMode = GL_RGB;
// #ifndef _WIN32
//         else if (fontMode == FT_PIXEL_MODE_BGRA)
//             glMode = GL_RGBA;
// #endif

//         GLtexture texture;
//         createGLtextureFromBuffer(texture, glyph->bitmap.buffer, glyph->bitmap.width, glyph->bitmap.rows, glMode);
//         glBindTexture(GL_TEXTURE_2D, texture);                            // Set the current texture
//         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

//         /* triangles:
//             bottom left-bottom right-top left
//             top right-top left-bottom right
//          */

//         vertices[0]  = vertices[10] = vertices[20] = penX + glyph->bitmap_left; // left
//         vertices[5]  = vertices[16] = vertices[25] = penX + glyph->bitmap_left + glyph->bitmap.width; //right
//         vertices[1]  = vertices[6]  = vertices[26] = penY - glyph->bitmap_top; // bottom
//         vertices[15] = vertices[11] = vertices[21] = penY - glyph->bitmap_top + glyph->bitmap.rows; // top

//         penX += glyph->advance.x >> 6;
//         penY += glyph->advance.y >> 6;

//         glBufferData(GL_ARRAY_BUFFER, 30 * sizeof(float), vertices, GL_DYNAMIC_DRAW);  // Fill the buffer
//         glDrawArrays(GL_TRIANGLES, 0, 6);                                         // Draw the character

//         glDeleteTextures(1, &texture);
//     }
//     return true;
// }

// /*!
//  * \brief Loads a font.
//  * \details Loads a font from the library given by <code>filename</code>.
//  *   \param filename The file name of the font to be loaded.
//  * \warning If the font cannot be found then an error message is printed out.
//  * \warning If the font library is not correctly installed then an error message is printed out.
//  * \warning If the font is not supported then an error message is printed out.
//  * \return True if successful, false otherwise.
//  */
// bool TextureHandler::loadFont(const std::string& filename) {
//     if (fontLibrary == nullptr) {
//         if (FT_Init_FreeType(&fontLibrary)) {
//             fprintf(stderr, "An error occurred during freetype font library initialization\n");
//             return false;
//         }
//     }

//     if(filename == "") {
//         fontFace = nullptr;
//         return true;
//     }

//     if (loadedFonts.find(filename) == loadedFonts.end()) {  // Load the image if we haven't already
//         FT_Face tmp_face;
//         int error = FT_New_Face(fontLibrary, filename.c_str(), 0, &tmp_face);
//         if (error == FT_Err_Unknown_File_Format) {
//             fprintf(stderr, "%s: the font file could be opened and read, but it appears that its"
//                             "font format is unsupported\n", filename.c_str());
//             return false;
//         } else if (error) {
//             fprintf(stderr, "%s: the font file could not be opened and read\n", filename.c_str());
//             return false;
//         }

//         loadedFonts[filename] = tmp_face;
//         fontFace = tmp_face;
//         FT_Select_Charmap(fontFace , ft_encoding_unicode);
//     } else {
//         fontFace = loadedFonts[filename];
//     }

//     return true;
// }

// void TextureHandler::calculateTextDimensions(std::string text, unsigned int font_size, float& width, float& height) {
//     // const wchar_t* string = text.c_str();
//     if(fontFace == nullptr) {   //If no font is set, load up a default one
//       bool found = false;
//       for (unsigned int i = 0; i < sizeof(DEFAULTFONTPATHS)/sizeof(*DEFAULTFONTPATHS); ++i) {
//           if (fileExists(DEFAULTFONTPATHS[i])) {
//               TsglDebug("No Font set! Now loading from " + std::string(DEFAULTFONTPATHS[i]));    //NEW
//               loadFont(DEFAULTFONTPATHS[i]);
//               found = true;
//               break;
//           }
//       }
//       if (!found) {
//           TsglErr("No suitable fonts found...exiting");    //NEW
//           exit(44);
//       }
//     }
//     FT_GlyphSlot glyph = fontFace->glyph;
//     FT_UInt current_glyph_index, previous_glyph_index = 0;
//     int penX = 0;
//     int penY = 0;

//     int minX = 0;
//     int minY = 0;
//     int maxX = 0;
//     int maxY = 0;

//     int currentRightX, currentTopY, currentBottomY;

//     bool error = FT_Set_Pixel_Sizes(fontFace, 0, font_size);
//     if (error) {
//         fprintf(stderr, "FT_Set_Pixel_Sizes failed\n");
//         return;
//     }

//     bool use_kerning = FT_HAS_KERNING(fontFace);

//     for (unsigned int i = 0; i < text.size(); i++) {
//         current_glyph_index = FT_Get_Char_Index(fontFace, text[i]);

//         if (use_kerning && previous_glyph_index && current_glyph_index) {
//             FT_Vector delta;
//             FT_Get_Kerning(fontFace, previous_glyph_index, current_glyph_index, FT_KERNING_DEFAULT, &delta);
//             penX += delta.x >> 6;
//             penY += delta.y >> 6;
//         }

//         error = FT_Load_Glyph(fontFace, current_glyph_index, FT_LOAD_RENDER);
//         if (error) {
//             fprintf(stderr, "FT_Load_Char falied\n");
//             return;
//         }

//         previous_glyph_index = current_glyph_index;

//         currentRightX = penX + glyph->bitmap_left + glyph->bitmap.width;
//         currentTopY  = penY - glyph->bitmap_top;
//         currentBottomY = penY - glyph->bitmap_top + glyph->bitmap.rows;

//         maxX = currentRightX;
//         if(currentBottomY > maxY) {
//             maxY = currentBottomY;
//         }
//         if(currentTopY < minY) {
//             minY = currentTopY;
//         }

//         penX += glyph->advance.x >> 6;
//         penY += glyph->advance.y >> 6;
//     }

//     width = maxX - minX;
//     height = maxY - minY;
// }

// /*!
//  * \brief Loads an image.
//  * \details Loads a .png, .jpeg, or .bmp image from a file.
//  *   \param filename The file name of the picture.
//  *   \param width A reference variable for holding the width of the picture.
//  *   \param height A reference variable for holding the height of the picture.
//  *   \param texture A reference variable for holding the texture of the picture.
//  *     (same as return value)
//  * \return The texture that created from the loaded image.
//  */
// GLtexture TextureHandler::loadPicture(std::string filename, unsigned int &width, unsigned int &height, GLtexture &texture) {
//     if (loadedTextures.find(filename) == loadedTextures.end()) {  // Load the image if we haven't already
//         texture = 0;
//         // stbi_set_flip_vertically_on_load(true);
//         // unsigned char * data = stbi_load(filename.c_str(), width, height, 0, 4);
//         // tsglAssert(data, "stbi_load(filename) failed.");
//         // // create the Image's texture id
//         // glGenTextures(1, &texture);
//         // glBindTexture(GL_TEXTURE_2D, texture);

//         // // Set texture parameters for wrapping.
//         // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//         // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

//         // // Set texture parameters for filtering.
//         // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
//         // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

//         // // actually generate the texture
//         // glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
//         // glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0,
//         //            	 GL_RGBA, GL_UNSIGNED_BYTE, data);
//         // glGenerateMipmap(GL_TEXTURE_2D);

//         std::string extension = filename.substr(filename.find_last_of('.'));
//         if (extension == ".png")
//             loadedTextures[filename] = loadTextureFromPNG(filename.c_str(), width, height, texture);
//         else if (extension == ".jpg" || extension == ".jpeg")
//             loadedTextures[filename] = loadTextureFromJPG(filename.c_str(), width, height, texture);
//         else if (extension == ".bmp")
//             loadedTextures[filename] = loadTextureFromBMP(filename.c_str(), width, height, texture);
//         else {
//             fprintf(stderr, "File extension not found\n");
//             return 0;
//         }
//     } else {
//         texture = loadedTextures[filename];
//     }

//     return texture;
// }

// GLtexture TextureHandler::loadTextureFromBMP(const char* filename, unsigned int &width, unsigned int &height,
//                                        GLtexture &texture) const {
//     // Adapted from http://www.opengl-tutorial.org/beginners-tutorials/tutorial-5-a-textured-cube/#Loading__BMP_images_yourself

//     // Data read from the header of the BMP file
//     unsigned char header[54];  // Each BMP file begins by a 54-bytes header
//     unsigned int imageSize;    // = width*height*3
//     // Actual RGB data
//     unsigned char * data;

//     // Open the file
// #ifdef _WIN32
//     FILE* file;
//     fopen_s(&file, filename, "rb");
// #else
//     FILE* file = fopen(filename, "rb");
// #endif

//     if (!file) {
//         fprintf(stderr, "Can't open %s: no such file\n", filename);
//         return 0;
//     }

//     if (fread(header, 1, 54, file) != 54) {  // If not 54 bytes read : problem
//         fprintf(stderr, "%s: not a correct BMP file: header incorrect size\n", filename);
//         fclose(file);
//         return 0;
//     }

//     if (header[0] != 'B' || header[1] != 'M') {
//         fprintf(stderr, "%s: not a correct BMP file: header did not specify BMP type\n", filename);
//         fclose(file);
//         return 0;
//     }

//     imageSize = width = height = 0;
//     // Get info out of header as 4 byte unsigned ints
//     for (int i = 3; i >= 0; i--)
//         imageSize = (imageSize << 8) | header[0x22 + i];
//     for (int i = 3; i >= 0; i--)
//         width = (width << 8) | header[0x12 + i];
//     for (int i = 3; i >= 0; i--)
//         height = (height << 8) | header[0x16 + i];

//     int components = imageSize / width / height;

//     // Some BMP files are misformatted, guess missing information
//     if (imageSize == 0) imageSize = width * height * 4;  // 4 : one byte for each Red, Green, Blue, and Alpha component

//     // Create a buffer
//     data = new unsigned char[imageSize];

//     // Read the actual data from the file into the buffer
//     if (fread(data, 1, imageSize, file) != imageSize) {  // If not imageSize bytes read : problem
//         fprintf(stderr, "%s: file ended unexpectedly\n", filename);
//         fclose(file);
//         return 0;
//     }

//     //Everything is in memory now, the file can be closed
//     fclose(file);

//     char tmp;
//     //Reverse the endian-ness of the colors
//     if (components == 4) {
//         for (unsigned int i = 0; i < imageSize; i += 4) {
//             tmp = data[i];
//             data[i] = data[i + 3];
//             data[i + 3] = tmp;
//             tmp = data[i + 1];
//             data[i + 1] = data[i + 2];
//             data[i + 2] = tmp;
//         }
//     } else if (components == 3) {
//         for (unsigned int i = 0; i < imageSize; i += 3) {
//             tmp = data[i];
//             data[i] = data[i + 1];
//             data[i + 1] = tmp;
//         }
//     }

//     // Flip the image vertically, since BMPs are loaded bottom to top
//     for (unsigned int j = 0; j < height - (height / 2); j++) {
//         for (unsigned int i = 0; i < components * width; i++) {
//             int s1 = components * width * j + i;
//             int s2 = components * width * (height - 1 - j) + i;  // This needs to be height *MINUS ONE* minus j
//             tmp = data[s1];
//             data[s1] = data[s2];
//             data[s2] = tmp;
//         }
//     }

//     if (components == 3)
//         components = GL_RGB;
//     else if (components == 4)
//         components = GL_RGBA;

//     createGLtextureFromBuffer(texture, data, width, height, components);

//     delete[] data;

//     return texture;
// }

// /*!
//  * \brief Gets the dimensions of an image
//  * \details Loads the header of a .png, .jpeg, or .bmp image to read their dimensions.
//  *   \param filename The file name of the picture.
//  *   \param width A reference variable for holding the width of the picture.
//  *   \param height A reference variable for holding the height of the picture.
//  * \return The texture that created from the loaded image.
//  */
// void TextureHandler::getDimensions(std::string filename, int &width, int &height) {
//     int w = 0, h = 0;
//     stbi_info(filename.c_str(), &w, &h, 0);
//     width = w; height = h;
// }

// GLtexture TextureHandler::loadTextureFromJPG(const char* filename, unsigned int &width, unsigned int &height,
//                                              GLtexture &texture) const {
//     unsigned char *data;
//     int w = 0, h = 0;
//     TsglDebug(std::string("Loading ") + filename);
//     data = stbi_load(filename, &w, &h, 0, 4);
//     assert(data);
//     if (!data) {
//       TsglErr(std::string("Loading ") + filename + " failed");
//       return texture;
//     }
//     TsglDebug(std::string("Loading ") + filename + " succeeded");
//     TsglDebug(to_string(w) + "," + to_string(h));
//     createGLtextureFromBuffer(texture, data, w, h, GL_RGBA);
//     width = w, height = h;
//     free(data);
//     return texture;
// }

// GLtexture TextureHandler::loadTextureFromPNG(const char* filename, unsigned int &width, unsigned int &height,
//                                        GLtexture &texture) const {
//   unsigned char *data;
//   int w = 0, h = 0;
//   TsglDebug(std::string("Loading ") + filename);
//   data = stbi_load(filename, &w, &h, 0, 4);
//   assert(data);
//   if (!data) {
//     TsglErr(std::string("Loading ") + filename + " failed");
//     return texture;
//   }
//   TsglDebug(std::string("Loading ") + filename + " succeeded");
//   TsglDebug(to_string(w) + "," + to_string(h));
//   createGLtextureFromBuffer(texture, data, w, h, GL_RGBA);
//   width = w, height = h;
//   free(data);
//   return texture;
// }

// /*!
//  * \brief Saves an Image.
//  * \details Saves an Image to file that was captured from a Canvas object.
//  *   \param filename The name of the file to save the Image to.
//  *   \param pixels The pixel data for the Image.
//  *   \param width The width of the Image.
//  *   \param height The height of the Image.
//  * \return True if successful, false otherwise.
//  */
// bool TextureHandler::saveImageToFile(std::string filename, GLubyte *pixels,
//                                      unsigned int width, unsigned int height) const {
//     std::string extension = filename.substr(filename.find_last_of('.'));
//     bool success = false;
//     if (extension == ".png")
//         success = saveToPNG(filename.c_str(), pixels, width, height);
//     else if (extension == ".jpg" || extension == ".jpeg")
//         fprintf(stderr, "JPG saving not implemented yet\n");
//     else if (extension == ".bmp")
//         success = saveToBMP(filename.c_str(), pixels, width, height);
//     else
//         fprintf(stderr, "File extension not found\n");
//     return success;
// }

// bool TextureHandler::saveToBMP(const char* filename, GLubyte *pixels, unsigned int w, unsigned int h) const {
//     unsigned char header[54];           // Each BMP file begins by a 54-bytes header
//     unsigned int imageSize = w * h * 3; // Byte-size of the data for the image
//     unsigned int totalSize = imageSize + 54;

//     // Open the file
// #ifdef _WIN32
//     FILE* file;
//     fopen_s(&file, filename, "wb");
// #else
//     FILE* file = fopen(filename, "wb");
// #endif

//     if (!file) {
//         fprintf(stderr, "Can't open %s: no such file\n", filename);
//         return false;
//     }

//     unsigned char padding = 4 - (w * 3) % 4;
//     if (padding == 4) padding = 0;
//     int rawdatasize = (w * 3 + padding) * h;

//     header[0] = 'B';
//     header[1] = 'M';
//     header[2] = (unsigned char) totalSize;
//     header[3] = (unsigned char)(totalSize >> 8);
//     header[4] = (unsigned char)(totalSize >> 16);
//     header[5] = (unsigned char)(totalSize >> 24);
//     for (unsigned i = 6; i <= 9; ++i)
//         header[i] = 0;
//     header[10] = 54;
//     for (unsigned i = 11; i <= 13; ++i)
//         header[i] = 0;
//     header[14] = 40;
//     for (unsigned i = 15; i <= 17; ++i)
//         header[i] = 0;
//     header[18] = (unsigned char) w;
//     header[19] = (unsigned char)(w >> 8);
//     header[20] = (unsigned char)(w >> 16);
//     header[21] = (unsigned char)(w >> 24);
//     header[22] = (unsigned char) h;
//     header[23] = (unsigned char)(h >> 8);
//     header[24] = (unsigned char)(h >> 16);
//     header[25] = (unsigned char)(h >> 24);
//     header[26] = 1;
//     header[27] = 0;
//     header[28] = 24;
//     header[29] = 0;
//     for (unsigned i = 30; i <= 33; ++i)
//         header[i] = 0;
//     header[34] = (unsigned char) rawdatasize;
//     header[35] = (unsigned char)(rawdatasize >> 8);
//     header[36] = (unsigned char)(rawdatasize >> 16);
//     header[37] = (unsigned char)(rawdatasize >> 24);
//     header[38] = 19;
//     header[39] = 11;
//     header[40] = 0;
//     header[41] = 0;
//     header[42] = 19;
//     header[43] = 11;
//     header[44] = 0;
//     header[45] = 0;
//     for (unsigned i = 46; i <= 53; ++i)
//         header[i] = 0;

//     unsigned char *rawdata = new unsigned char[rawdatasize];

//     unsigned rawpos = 0, datapos = 0;
//     for (unsigned j = 0; j < h; ++j) {
//         for (unsigned i = 0; i < w; ++i) {
//             rawdata[rawpos] = pixels[datapos+2];
//             rawdata[rawpos+1] = pixels[datapos+1];
//             rawdata[rawpos+2] = pixels[datapos];
//             rawpos += 3;
//             datapos += 3;
//         }
//         for (unsigned i = 0; i < padding; ++i) {
//             rawdata[rawpos++] = 0;
//         }
//     }

//     fwrite(header, 54, 1, file);
//     fwrite(rawdata, 1, rawdatasize, file);
//     fclose(file);

//     delete[] rawdata;

//     return true;
// }

// bool TextureHandler::saveToPNG(const char* filename, GLubyte *pixels, unsigned int w, unsigned int h) const {
//   // Flip the image, since for some reason the library call
//   // flips the image upside down when it saves
//   for (unsigned int j = 0; j < h - (h / 2); j++) {
//       for (unsigned int i = 0; i <  3 * w; i++) {
//           int s1 =  3 * w * j + i;
//           int s2 =  3 * w * (h - 1 - j) + i;  // This needs to be height *MINUS ONE* minus j
//           char tmp = pixels[s1];
//           pixels[s1] = pixels[s2];
//           pixels[s2] = tmp;
//       }
//   }
//     stbi_write_png(filename, w, h, 3, pixels, 0);
//     return true;
// }

// //-------------------------Unit testing---------------------------------------------
// /*!
//  * \brief Runs the Unit tests for TextureHandler.
//  */
// void TextureHandler::runTests() {
//     TsglDebug("Testing TextureHandler class...");
//     TextureHandler tester;
//     tsglAssert(testLoadFont(tester), "Unit test for loading in fonts failed!");
//     TsglDebug("Unit tests for TextureHandler complete.");
//     std::cout << std::endl;
// }

// bool TextureHandler::testLoadFont(TextureHandler& test) {
//   int passed = 0;   //Passed tests
//   int failed = 0;   //Failed tests
//   //Test 1: Loading in the font at the start
//   if(test.fontFace == nullptr) {
//      test.loadFont("../assets/freefont/FreeMono.ttf");
//      if(test.fontFace != nullptr) {
//        passed++;
//      } else {
//        failed++;
//        TsglErr("Test 1, Loading font for testLoadFont() failed!");
//      }
//   }

//   if(passed == 1 && failed == 0) {
//     TsglDebug("Unit test for loading in fonts passed!");
//     return true;
//   } else {
//     TsglErr("This many tests passed for testLoadFont: ");
//     std::cout << " " << passed << std::endl;
//     TsglErr("This many tests failed for testLoadFont: ");
//     std::cout << " " << failed << std::endl;
//     return false;
//   }
// }
// //----------------------------End Unit testing---------------------------------------
// }
