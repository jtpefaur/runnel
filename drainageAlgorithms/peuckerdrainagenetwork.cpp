#include "peuckerdrainagenetwork.h"
#include "terrain.h"
#include "primitives/point.h"
#include <cmath>


void PeuckerDrainageNetwork::calculateGrid(Terrain *ter){
    int width = ter->width;
    int height = ter->height;

    for(unsigned int i = 0; i < ter->struct_point.size() ; ++i){
        int fila = i/width;

        if ( (i+1) % width == 0){
            continue;
        }
        if( fila == (height-1)){
            break;
        }
        runnel::Point *points[4];
        points[0] = ter->struct_point[i];
        points[1] = ter->struct_point[i + 1];
        points[2] = ter->struct_point[i + width];
        points[3] = ter->struct_point[i + width + 1];

        if(!points[0] || !points[1] || !points[2] || !points[3]){
            continue;
        }

        runnel::Point *max_point = points[0];
        for(runnel::Point* pto: points){
            if(pto->coord.z > max_point->coord.z){
                max_point = pto;
            }
        }

        for(runnel::Point* pto: points){
            if(pto->coord.z == max_point->coord.z){
                max_point->setFlagsOn(runnel::Point::PEUCKER);
            }
        }


    }
}
