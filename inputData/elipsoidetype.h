#ifndef ELIPSOIDETYPE_H
#define ELIPSOIDETYPE_H

#include <string>

class ElipsoideType
{
    public:
        ElipsoideType();
        ElipsoideType(std::string code, std::string description, int year, double maxDiameter, double minDiameter, double equatorialRadius, double squareEccentricity);
        std::string code;
        std::string description;
        int year;
        double minDiameter;
        double maxDiameter;
        double equatorialRadius;
        double squareEccentricity;
};

#endif // ELIPSOIDETYPE_H
