#include "datacollector.h"
#include <iostream>
#include "lib/glm/gtc/constants.hpp"
#include "utmconverter.h"

DataCollector::DataCollector():
    ter_data(0)
{
      number_points = 0;
      counter = 0;
      lat0 = 0;
      lng0 = 0;
}

void DataCollector::getCoord(QByteArray list_coord)
{
    char* puntero = list_coord.data();
    float* punteroDOuble = (float*) puntero;
    for(unsigned int i = 0;i<list_coord.size()/sizeof(float);){
        float x = *(punteroDOuble++);
        float y = *(punteroDOuble++);
        float z = *(punteroDOuble++);
        glm::vec3 coords = UTMConverter::convertToUTM(x, y, z);

        ter_data->setBoundingBox(coords);
        runnel::Point *point_new = new runnel::Point(coords, counter);
        ter_data->addPoint(point_new);
        ++counter;
        i+=3;
    }

}


void DataCollector::setCantPoints(int num, int anch, int alt, int ancho_pixel, int height_pixel, float lat0, float lng0)
{
    std::cout << "Seteo de cant de ptos " << std::endl;
    this->anch = anch;
    this->alt = alt;
    counter = 0;
    ancho_pix = ancho_pixel;
    largo_pix = height_pixel;
    number_points = num;
    this->lat0 = lat0;
    this->lng0 = lng0;
    UTMConverter::setZeroPosition(lat0, lng0);
    emit changeTerrain();
}

void DataCollector::finishInformation()
{
    std::cout << "Finish Information " << std::endl;
    ter_data->width = anch;
    ter_data->height = alt;
    ter_data->setMapPixel(ancho_pix, largo_pix);
    ter_data->signalPaintGoogle = true;
    ter_data->lat0 = this->lat0;
    ter_data->lng0 = this->lng0;
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
