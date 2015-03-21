#include "elipsoidetype.h"

ElipsoideType::ElipsoideType(){}

ElipsoideType::ElipsoideType(std::string code, std::string description, int year, double maxDiameter, double minDiameter, double equatorialRadius, double squareEccentricity)
{
    this->code = code;
    this->description = description;
    this->year = year;
    this->minDiameter = minDiameter;
    this->maxDiameter = maxDiameter;
    this->equatorialRadius = equatorialRadius;
    this->squareEccentricity = squareEccentricity;
}
