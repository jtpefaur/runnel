#include "elipsoidetype.h"

ElipsoideType::ElipsoideType(){}

ElipsoideType::ElipsoideType(std::string code, std::string description, int year, double maxDiameter, double minDiameter)
{
    this->code = code;
    this->description = description;
    this->year = year;
    this->minDiameter = minDiameter;
    this->maxDiameter = maxDiameter;
}
