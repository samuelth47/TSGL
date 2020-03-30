#include "Cylinder.h"

namespace tsgl {

 /*!
  * \brief Explicitly constructs a new Cylinder.
  * \details Explicit constructor for a Cylinder object.
  *   \param x The x coordinate of the center of the Cylinder.
  *   \param y The y coordinate of the center of the Cylinder.
  *   \param z The z coordinate of the center of the Cylinder.
  *   \param height The height of the Cylinder.
  *   \param radius The radius of the Cylinder's circular base.
  *   \param yaw The Cylinder's yaw.
  *   \param pitch The Cylinder's pitch.
  *   \param roll The Cylinder's roll.
  *   \param c A ColorFloat for the Cylinder's vertex colors.
  * \warning An invariant is held where if length, width, or height isn't positive then an error message is given.
  * \return A new Cylinder with a buffer for storing the specified numbered of vertices.
  */
Cylinder::Cylinder(float x, float y, float z, GLfloat height, GLfloat radius, float yaw, float pitch, float roll, ColorGLfloat c)  
: Prism(x, y, z, 2 * ( (radius > 1) ? radius * 6 : 6), yaw, pitch, roll)  {
    if (radius <= 0 ||  height <= 0) {
        TsglDebug("Cannot have a Cylinder with non-positive height or radius.");
    }
    myRadius = radius;
    myHeight = height;
    geometryType = GL_TRIANGLES;
}

 /*!
  * \brief Explicitly constructs a new Cylinder.
  * \details Explicit constructor for a Cylinder object.
  *   \param x The x coordinate of the center of the Cylinder.
  *   \param y The y coordinate of the center of the Cylinder.
  *   \param z The z coordinate of the center of the Cylinder.
  *   \param height The height of the Cylinder.
  *   \param radius The radius of the Cylinder's circular base.
  *   \param yaw The Cylinder's yaw.
  *   \param pitch The Cylinder's pitch.
  *   \param roll The Cylinder's roll.
  *   \param c An array of ColorFloats for the Cylinder's vertex colors.
  * \warning An invariant is held where if length, width, or height isn't positive then an error message is given.
  * \return A new Cylinder with a buffer for storing the specified numbered of vertices.
  */
Cylinder::Cylinder(float x, float y, float z, float height, float radius, float yaw, float pitch, float roll, ColorGLfloat c[])  
: Prism(x, y, z, 2 * ( (radius > 1) ? radius * 6 : 6), yaw, pitch, roll)  { // FIXME vertices
    if (radius <= 0 ||  height <= 0) {
        TsglDebug("Cannot have a Cylinder with non-positive height or radius.");
    }
    myRadius = radius;
    myHeight = height;
    geometryType = GL_TRIANGLES;

}

/**
 * \brief Mutates the radius of the Cylinder's base.
 * \param height The Cylinder's new base radius.
 */
void Cylinder::setRadius(float radius) {
    attribMutex.lock();
    if (radius <= 0) {
        TsglDebug("Cannot have a Cylinder with radius less than or equal to 0.");
        attribMutex.unlock();
        return;
    }
    myRadius = radius;
    attribMutex.unlock();
    // fix vertices
}

/**
 * \brief Mutates the radius of the Cylinder's base by the parameter amount.
 * \param delta The amount by which to change the radius of the Cylinder's base.
 */
void Cylinder::changeRadiusBy(float delta) {
    attribMutex.lock();
    if (myRadius + delta <= 0) {
        TsglDebug("Cannot have a Cylinder with radius less than or equal to 0.");
        attribMutex.unlock();
        return;
    }
    myRadius += delta;
    attribMutex.unlock();
    // fix vertices
}

/**
 * \brief Mutates the distance from the center of the Cylinder's base to the top.
 * \param height The Cylinder's new height.
 */
void Cylinder::setHeight(float height) {
    attribMutex.lock();
    if (height <= 0) {
        TsglDebug("Cannot have a Cylinder with height less than or equal to 0.");
        attribMutex.unlock();
        return;
    }
    myHeight = height;
    attribMutex.unlock();
    // fix vertices
}

/**
 * \brief Mutates the distance from the center of the Cylinder's base to the top by the parameter amount.
 * \param delta The amount by which to change the height of the Cylinder.
 */
void Cylinder::changeHeightBy(float delta) {
    attribMutex.lock();
    if (myHeight + delta <= 0) {
        TsglDebug("Cannot have a Cylinder with height less than or equal to 0.");
        attribMutex.unlock();
        return;
    }
    myHeight += delta;
    attribMutex.unlock();
    // fix vertices
}

/*!
 * \brief Destructor for the Cylinder.
 */
Cylinder::~Cylinder() { }

}