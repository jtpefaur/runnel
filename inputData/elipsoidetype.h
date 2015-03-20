#ifndef ELIPSOIDETYPE_H
#define ELIPSOIDETYPE_H

#include <string>

class ElipsoideType
{
    public:
        ElipsoideType();
        ElipsoideType(std::string code, std::string description, int year, double maxDiameter, double minDiameter);
        std::string code;
        std::string description;
        int year;
        double minDiameter;
        double maxDiameter;
};

#endif // ELIPSOIDETYPE_H
