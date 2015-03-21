#ifndef UTMCONVERTER_H
#define UTMCONVERTER_H

#include "lib/glm/glm.hpp"
#include "inputData/elipsoidetype.h"

class UTMConverter
{
    public:
        UTMConverter();
        const static int SOUTH = 0;
        const static int NORTH = 1;
        static double PI;
        static glm::vec3 convertToUTM(double latitud, double longitud, double altitud);
        static void setZeroPosition(float lat0, float lng0);
        static inline double degreeToRadians(double degree);
        static inline double radiansToDegree(double radian);
        static glm::vec3 convertToLatLong(glm::vec3);
        static char UTMLetterDesignator(double lat);

    private:
        static char letterDesignator;
        static int husoUnicoUTM;
        static int hemisferioUnicoUTM;
        static ElipsoideType elipsoideType;
        static double alfa, beta, gamma, ep, ep2, a, b, c;
        static char UTMZone[4];
};

#endif // UTMCONVERTER_H
