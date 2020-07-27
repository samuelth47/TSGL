#ifndef PERSON_H_
#define PERSON_H_

#include <random>
#include "Circle.h"
#include "statusEnums.h"

using namespace tsgl;

class Person {
protected:
    // int myID;
    float myX, myY;
    GLfloat myCircleRadius;
    Circle* myCircle;
    Square* myInfectionCircle;
    ColorFloat myColor;
    bool hasInfectedRadius;

    // Pandemic data
    char myStatus;
    int numInfectedNearby;
    int numDaysInfected;
public:
    Person();

    Person(float x, float y, GLfloat radius, char status, bool hasInfectedRadius);

    void draw(Canvas& can);


    // Accessors
    GLfloat getCircleRadius() { return myCircleRadius; }
        
    Circle * getCircle() { return myCircle; }

    float getX() { return myX; }

    float getY() { return myY; }

    char getStatus() { return myStatus; }

    int getNumDaysInfected() { return numDaysInfected; }
    

    // // Mutators
    void setStatus(char status);

    // void setCircleRadius(GLfloat radius);

    void moveBy(float x, float y, float max_x, float max_y);

    // void updateColor();

    bool checkIfInfectedNearby(std::vector<Person*> personVec, float infectionRadius);

    void increaseNumDaysInfected() { numDaysInfected += 1; }

    bool determineIfInfected(Canvas& can, int contagiousFactor, int randNum);
    
    bool determineIfDead(Canvas& can, int deadlinessFactor, int randNum);

    void recover(Canvas& can);

    virtual ~Person();
};

#endif /* PERSON_H_ */