#include "utmconverter.h"
#include <cmath>

UTMConverter::UTMConverter()
{
}

ElipsoideType UTMConverter::elipsoideType = ElipsoideType("WGS84", "WGS84", 1984, 6378137.0, 6356752.314);
double UTMConverter::PI = 3.141592653589793238463;
int UTMConverter::husoUnicoUTM;
int UTMConverter::hemisferioUnicoUTM;

double UTMConverter::degreeToRadians(double degree) {
    return degree * UTMConverter::PI / 180.0;
}

glm::vec3 UTMConverter::convertToUTM(double latitud, double longitud, double altitud) {
    double a = elipsoideType.maxDiameter;
    double b = elipsoideType.minDiameter;
    double ep, ep2, c;
    double lonRadianes, latRadianes, huso, deltaLambda, aa;
    double xi, eta, ni, zeta, a1, a2, j2, j4, j6;
    double alfa, beta, gamma, bfi;
    int hemisferio;

   //ex = std::sqrt(std::pow(a,2) - std::pow(b,2))/a;  //Excentricidad
    ep = std::sqrt(std::pow(a,2) - std::pow(b,2))/b;
    ep2 = std::pow(ep,2);
    c = std::pow(a,2)/b; //Radio polar de curvatura

    lonRadianes = UTMConverter::degreeToRadians(longitud);
    latRadianes = UTMConverter::degreeToRadians(latitud);

    huso = UTMConverter::husoUnicoUTM > 0 ? UTMConverter::husoUnicoUTM : (int)(longitud/6 + 31);
    hemisferio = UTMConverter::hemisferioUnicoUTM == -1 ? (latitud >= 0 ? UTMConverter::NORTH : UTMConverter::SOUTH) : UTMConverter::hemisferioUnicoUTM;

    huso = UTMConverter::degreeToRadians(6 * huso - 183);

    deltaLambda = lonRadianes - huso;
    aa = std::cos(latRadianes) * std::sin(deltaLambda);
    xi = std::log((1 + aa) / (1 - aa)) / 2;
    eta = std::atan(std::tan(latRadianes) / std::cos(deltaLambda)) - latRadianes;
    ni = 0.9996 * c / std::pow(1 + ep2 * std::pow(std::cos(latRadianes), 2), 0.5);
    zeta = (ep2 / 2) * std::pow(xi,2) * std::pow(std::cos(latRadianes), 2);
    a1 = std::sin(2*latRadianes);
    a2 = a1 * std::pow(std::cos(latRadianes),2);
    j2 = latRadianes + a1/2;
    j4 = (3 * j2 + a2) / 4;
    j6 = (5 * j4 + a2 * std::pow(std::cos(latRadianes), 2) / 3);
    alfa = 3 * ep2 / 4;
    beta = 5 * std::pow(alfa,2)/3;
    gamma = 35 * std::pow(alfa,3) / 27;
    bfi = 0.9996 * c * (latRadianes - alfa * j2 + beta * j4 - gamma * j6);

    double coordX = xi * ni * (1 + zeta / 3) + 500000;
    double coordY = eta * ni * (1 + zeta) + bfi + (hemisferio == UTMConverter::SOUTH ? 10000000 : 0 );

    return glm::vec3(coordX, coordY, altitud);
}

glm::vec3 UTMConverter::convertToLatLong(){

}


void UTMConverter::setZeroPosition(float lat0, float lng0){
    float value_lng = lng0/6 + 31;
    UTMConverter::husoUnicoUTM = int(value_lng);
    UTMConverter::hemisferioUnicoUTM = lat0 >= 0 ? NORTH : SOUTH;
}
