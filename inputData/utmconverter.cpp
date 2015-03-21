#include "utmconverter.h"

#include <cmath>
#include "iostream"
UTMConverter::UTMConverter()
{
}

ElipsoideType UTMConverter::elipsoideType = ElipsoideType("WGS84", "WGS84", 1984, 6378137.0, 6356752.314, 6378137.0, 0.00669438);
double UTMConverter::PI = 3.141592653589793238463;
int UTMConverter::husoUnicoUTM;
int UTMConverter::hemisferioUnicoUTM;
char UTMConverter::letterDesignator;
double UTMConverter::a = elipsoideType.maxDiameter;
double UTMConverter::b = elipsoideType.minDiameter;
double UTMConverter::c = std::pow(a,2)/b;
double UTMConverter::ep = std::sqrt(std::pow(a,2) - std::pow(b,2))/b;
double UTMConverter::ep2 = std::pow(ep,2);
double UTMConverter::alfa = 3 * ep2 / 4;
double UTMConverter::beta = 5 * std::pow(alfa,2)/3;
double UTMConverter::gamma = 35 * std::pow(alfa,3) / 27;
char UTMConverter::UTMZone[4];

double UTMConverter::degreeToRadians(double degree) {
    return degree * UTMConverter::PI / 180.0;
}

double UTMConverter::radiansToDegree(double radian) {
    return radian * 180.0 / UTMConverter::PI;
}

glm::vec3 UTMConverter::convertToUTM(double latitud, double longitud, double altitud) {
    double lonRadianes, latRadianes, huso, deltaLambda, aa;
    double xi, eta, ni, zeta, a1, a2, j2, j4, j6;
    double bfi;
    int hemisferio;

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
    bfi = 0.9996 * c * (latRadianes - alfa * j2 + beta * j4 - gamma * j6);

    double coordX = xi * ni * (1 + zeta / 3) + 500000;
    double coordY = eta * ni * (1 + zeta) + bfi + (hemisferio == UTMConverter::SOUTH ? 10000000 : 0 );

    return glm::vec3(coordX, coordY, altitud);
}

glm::vec3 UTMConverter::convertToLatLong(glm::vec3 utm){
    // http://www.gpsy.com/gpsinfo/geotoutm/gantz/LatLong-UTMconversion.cpp
    double UTMNorthing = utm[1];
    double UTMEasting = utm[0];
    double k0 = 0.9996;
    double a = elipsoideType.equatorialRadius;
    double eccSquared = elipsoideType.squareEccentricity;
    double eccPrimeSquared;
    double e1 = (1-std::sqrt(1-eccSquared))/(1+std::sqrt(1-eccSquared));
    double N1, T1, C1, R1, D, M;
    double longOrigin;
    double mu, phi1Rad;
    double x, y;
    int zoneNumber;
    char* zoneLetter;

    x = UTMEasting - 500000.0; //remove 500,000 meter offset for longitude
    y = UTMNorthing;

    zoneNumber = strtoul(UTMZone, &zoneLetter, 10);

    if((*zoneLetter - 'N') < 0)
        y -= 10000000.0;//remove 10,000,000 meter offset used for southern hemisphere

    longOrigin = (zoneNumber - 1)*6 - 180 +3;  //+3 puts origin in middle of zone

    eccPrimeSquared = (eccSquared)/(1-eccSquared);

    M = y / k0;
    mu = M/(a*(1-eccSquared/4-3*eccSquared*eccSquared/64-5*eccSquared*eccSquared*eccSquared/256));

    phi1Rad = mu	+ (3*e1/2-27*e1*e1*e1/32)*std::sin(2*mu)
                + (21*e1*e1/16-55*e1*e1*e1*e1/32)*std::sin(4*mu)
                +(151*e1*e1*e1/96)*std::sin(6*mu);

    N1 = a/std::sqrt(1-eccSquared*std::sin(phi1Rad)*std::sin(phi1Rad));
    T1 = std::tan(phi1Rad)*std::tan(phi1Rad);
    C1 = eccPrimeSquared*std::cos(phi1Rad)*std::cos(phi1Rad);
    R1 = a*(1-eccSquared)/std::pow(1-eccSquared*std::sin(phi1Rad)*std::sin(phi1Rad), 1.5);
    D = x/(N1*k0);

    double lat = phi1Rad - (N1*tan(phi1Rad)/R1)*(D*D/2-(5+3*T1+10*C1-4*C1*C1-9*eccPrimeSquared)*D*D*D*D/24
                    +(61+90*T1+298*C1+45*T1*T1-252*eccPrimeSquared-3*C1*C1)*D*D*D*D*D*D/720);
    lat = radiansToDegree(lat);

    double longi = (D-(1+2*T1+C1)*D*D*D/6+(5-2*C1+28*T1-3*C1*C1+8*eccPrimeSquared+24*T1*T1)
                    *D*D*D*D*D/120)/std::cos(phi1Rad);
    longi = longOrigin + radiansToDegree(longi);

    return glm::vec3(lat, longi, utm[2]);
}

char UTMConverter::UTMLetterDesignator(double lat) {
//This routine determines the correct UTM letter designator for the given latitude
//returns 'Z' if latitude is outside the UTM limits of 84N to 80S
    //Written by Chuck Gantz- chuck.gantz@globalstar.com
    char letterDesignator;

    if((84 >= lat) && (lat >= 72)) letterDesignator = 'X';
    else if((72 > lat) && (lat >= 64)) letterDesignator = 'W';
    else if((64 > lat) && (lat >= 56)) letterDesignator = 'V';
    else if((56 > lat) && (lat >= 48)) letterDesignator = 'U';
    else if((48 > lat) && (lat >= 40)) letterDesignator = 'T';
    else if((40 > lat) && (lat >= 32)) letterDesignator = 'S';
    else if((32 > lat) && (lat >= 24)) letterDesignator = 'R';
    else if((24 > lat) && (lat >= 16)) letterDesignator = 'Q';
    else if((16 > lat) && (lat >= 8)) letterDesignator = 'P';
    else if(( 8 > lat) && (lat >= 0)) letterDesignator = 'N';
    else if(( 0 > lat) && (lat >= -8)) letterDesignator = 'M';
    else if((-8> lat) && (lat >= -16)) letterDesignator = 'L';
    else if((-16 > lat) && (lat >= -24)) letterDesignator = 'K';
    else if((-24 > lat) && (lat >= -32)) letterDesignator = 'J';
    else if((-32 > lat) && (lat >= -40)) letterDesignator = 'H';
    else if((-40 > lat) && (lat >= -48)) letterDesignator = 'G';
    else if((-48 > lat) && (lat >= -56)) letterDesignator = 'F';
    else if((-56 > lat) && (lat >= -64)) letterDesignator = 'E';
    else if((-64 > lat) && (lat >= -72)) letterDesignator = 'D';
    else if((-72 > lat) && (lat >= -80)) letterDesignator = 'C';
    else letterDesignator = 'Z'; //This is here as an error flag to show that the Latitude is outside the UTM limits

    return letterDesignator;
}

void UTMConverter::setZeroPosition(float lat0, float lng0){
    float value_lng = lng0/6 + 31;
    husoUnicoUTM = int(value_lng);
    hemisferioUnicoUTM = lat0 >= 0 ? NORTH : SOUTH;
    double lng0Temp = (lng0+180)-int((lng0+180)/360)*360-180;
    int zoneNumber = int((lng0Temp + 180)/6) + 1;
    sprintf(UTMZone, "%d%c", zoneNumber, UTMConverter::UTMLetterDesignator(lat0));

}
