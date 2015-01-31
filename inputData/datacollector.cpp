#include "datacollector.h"
#include <iostream>
#include <lib/glm/gtc/constants.hpp>

DataCollector::DataCollector():
    ter_data(0)
{
      number_points = 0;
      counter = 0;
}

void DataCollector::getCoord(QByteArray list_coord)
{
    char* puntero = list_coord.data();
    float* punteroDOuble = (float*) puntero;
//   std::cout << "Latitud" << " " << "Longitud" << " " << "Altura" << std::endl;
    for(unsigned int i = 0;i<list_coord.size()/sizeof(float);){
//        float x = *(punteroDOuble++);
//        float y = *(punteroDOuble++);
//        float z = *(punteroDOuble++);
//        glm::vec3 coords = calculateUTM( x, y, z);
//        std::cout << counter << " " << coords.x << " " << coords.y << " " << coords.z << std::endl;
        float x = this->conversionToUTM( *(punteroDOuble++))*31.1120;
        float y = -1*this->conversionToUTM( *(punteroDOuble++))*11.1320;
        float z = this->conversionToUTM( *(punteroDOuble++))/10000;
     //   std::cout << x << " " << y << " " << z << std::endl;
        glm::vec3 coords = glm::vec3(x,y,z);
        ter_data->setBoundingBox(coords);
     //   std::cout << "coordenadas " << coords.x << " " << coords.y << " " << coords.z << std::endl;
        runnel::Point *point_new = new runnel::Point(coords, counter);
        ter_data->addPoint(point_new);
        ++counter;
        i+=3;
    }

}


void DataCollector::setCantPoints(int num, int anch, int alt, int ancho_pixel, int height_pixel)
{
    std::cout << "Seteo de cant de ptos " << std::endl;
    this->anch = anch;
    this->alt = alt;
    counter = 0;
    std::cout << " lala" << ancho_pixel << " " << height_pixel << std::endl;
    ancho_pix = ancho_pixel;
    largo_pix = height_pixel;
    number_points = num;
    emit changeTerrain();
}

void DataCollector::finishInformation()
{
    std::cout << "Finish Information " << std::endl;
    ter_data->width = anch;
    ter_data->height = alt;
    ter_data->setMapPixel(ancho_pix, largo_pix);
    //this->calculateUTM();
    std::cout << "altura " << alt << " ancho "<< anch << std::endl;
    emit finishTerrain();
}

void DataCollector::getTerrainStruct(Terrain* ter)
{
    ter_data = ter;
}

DataCollector::~DataCollector()
{
    std::cout << "Finish Runnel Software" << std::endl;
}

float DataCollector::conversionToUTM(float number){
    return number;
}


glm::vec3 DataCollector::calculateUTM(float lati, float longit, float height){

    std::cout << "latitud " << lati << "longitud "<< longit << std::endl;
    double lat_decimal_grade = lati;
    double long_decimal_grade = longit;
    double long_rad_grade = long_decimal_grade*glm::pi<double>()/180.0;
    double lat_rad_grade = lat_decimal_grade*glm::pi<double>()/180.0;
    double a, b;
    a = 6378137;
    b = 6356752.3142;
    double cte_decimal = 0.9996;
    double excen = glm::sqrt(a*a -b*b)/a;
    double excen2 = glm::sqrt(a*a -b*b)/b;
    double c = a*a/b;
    double alpha_a = (a-b)/a;
    int HUSO = long_decimal_grade/6.0 + 31;
    double lambda0 = HUSO*6 -183;
    double lambda = long_rad_grade - lambda0*glm::pi<float>()/180.0;
    double cos_lat_rad_grade = glm::cos(lat_rad_grade);
    double A = cos_lat_rad_grade*glm::sin(lambda);
    double A_a = (A + 1)/(1 - A);
    double xi = 0.5*glm::log(A_a);
    double eta = glm::atan(glm::tan(lat_rad_grade)/glm::cos(lambda)) - lat_rad_grade;
    double ni_a = 1+ std::pow(excen2,2)*std::pow(cos_lat_rad_grade,2);
    double ni = cte_decimal*c/std::pow(ni_a,0.5);
    double dseta = std::pow(excen2,2)*std::pow(xi, 2)*cos_lat_rad_grade*cos_lat_rad_grade*0.5;
    double A_1 = glm::sin(2*lat_rad_grade);
    double A_2 = A_1*cos_lat_rad_grade*cos_lat_rad_grade;
    double J_2 = lat_rad_grade + A_1*0.5;
    double J_4 = (3.0*J_2 + A_2)/4.0;
    double J_6 = (5*J_4 + A_2*cos_lat_rad_grade*cos_lat_rad_grade)/3;
    double alpha = 3.0*excen2*excen2/4.0;
    double beta = 5.0*alpha*alpha/3.0;
    double gamma = 35.0*alpha*alpha*alpha/27.0;
    double B_theta = cte_decimal*c*(lat_rad_grade- alpha*J_2 + beta*J_4 - gamma*J_6);
    double lat_final = (xi*ni*(1 + dseta/3) + 10500000);
    double long_final = (eta*ni*(1+dseta) + B_theta);
    return glm::vec3(lat_final/10000,long_final/10000, height );

}
