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
        static double degreeToRadians(double degree);
        static ElipsoideType elipsoideType;
        static glm::vec3 convertToLatLong();

    private:
        static int husoUnicoUTM;
        static int hemisferioUnicoUTM;
};

#endif // UTMCONVERTER_H
