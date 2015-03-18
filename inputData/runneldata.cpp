#include "runneldata.h"
#include <fstream>
#include <QFile>
#include <QTextStream>
#include <iostream>

RunnelData::RunnelData()
{
}

bool RunnelData::openFile(QString name){

}

void RunnelData::getDataTerrain(Terrain* ter){

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
    outStream << ter->struct_point.size() << endl;
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

        if ( neighbour.size() == 2 )
            outStream << neighbour[0]->ident << " " << -1 << " " << -1 << endl;
    }

    outputFile.close();
    return true;
}

RunnelData::~RunnelData(){

}
