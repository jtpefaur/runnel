#include "runneldata.h"
#include <fstream>
#include <QFile>
#include <QTextStream>
#include <iostream>
#include "utmconverter.h"

RunnelData::RunnelData()
{
}

bool RunnelData::openFile(QString name){
    std::cout << "data archivo " << name.toStdString() << std::endl;

    file.setFileName(name);
    if(!file.open(QIODevice::ReadOnly)) {
        std::cout << "error with open file" << std::endl;
        return false;
    }
    return true;
}

void RunnelData::getDataTerrain(Terrain* ter){
    QTextStream stream(&file);
    QString line = stream.readLine();
    QStringList fields = line.split(" ");
    int number_points = fields[0].toInt();
    ter->width = fields[1].toInt();
    ter->height = fields[2].toInt();
    float lat0 = fields[3].toInt();
    float lng0 = fields[4].toInt();
    std::cout << number_points << " " << ter->width << " " << ter->height << std::endl;
    for (int i = 0; i < number_points; ++i){
        QString line = stream.readLine();
        QStringList fields = line.split(" ");
        glm::vec3 coords = glm::vec3(fields[1].toFloat(),fields[2].toFloat(),fields[3].toFloat());
        ter->setBoundingBox(coords);
        runnel::Point *point_new = new runnel::Point(coords, fields[0].toInt());
        ter->addPoint(point_new);
    }

    UTMConverter::setZeroPosition(lat0, lng0);

    file.close();
}


bool RunnelData::writeFile(QString fileName, Terrain* ter){
    QFile outputFile(fileName);
    outputFile.open(QIODevice::WriteOnly);

    if(!outputFile.isOpen()){
        std::cout << "Error, unable to open" << fileName.toStdString() << "for output" << std::endl;
        return false;
    }

    QTextStream outStream(&outputFile);

    /* Write the line to the file */
    //writes points
    float lat0 = ter->lat0;
    float lng0 = ter->lng0;
    outStream << ter->struct_point.size() << " " << ter->width << " " << ter->height << " " << lat0 << " " << lng0 << endl;
    for(runnel::Point *pto : ter->struct_point){
        outStream << pto->ident << " " << pto->coord.x << " " << pto->coord.y << " " << pto->coord.z << endl;
    }
    //writes triangles
    outStream << ter->struct_triangle.size() << endl;
    for(runnel::Triangle *trian : ter->struct_triangle){
        outStream << trian->ident << " " << trian->points[0]->ident << " " << trian->points[1]->ident << " " << trian->points[2]->ident << endl;
    }


    for(runnel::Triangle *trian : ter->struct_triangle){
        std::vector<runnel::Triangle*> neighbour = trian->getNeighbours();
        if ( neighbour.size() == 3 )
            outStream << neighbour[0]->ident << " " << neighbour[1]->ident << " " << neighbour[2]->ident << endl;

        if ( neighbour.size() == 2 )
            outStream << neighbour[0]->ident << " " << neighbour[1]->ident << " " << -1 << endl;

        if ( neighbour.size() == 1 )
            outStream << neighbour[0]->ident << " " << -1 << " " << -1 << endl;
    }

    outputFile.close();
    return true;
}

RunnelData::~RunnelData(){

}
