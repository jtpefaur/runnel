#include "rwfloodalgorithm.h"
#include <queue>

RWFloodAlgorithm::RWFloodAlgorithm():
    DrainageAlgorithms()
{

}

RWFloodAlgorithm::~RWFloodAlgorithm()
{

}

void RWFloodAlgorithm::run(Terrain *ter)
{
    this->ter = ter;
    this->flood();
}

void RWFloodAlgorithm::glewReady()
{

}

void RWFloodAlgorithm::render(glm::mat4 matrix, float exag_z, glm::vec3 color)
{

}

QString RWFloodAlgorithm::getName()
{
    return QString("RWFlood");
}

QWidget* RWFloodAlgorithm::getConf()
{
    return 0;
}

std::vector<glm::vec3> RWFloodAlgorithm::getPathTree()
{
    std::vector<glm::vec3> path;
    return path;
}

void RWFloodAlgorithm::flood()
{
    const int maxElev = (int)(ter->max_bounding.z);
    const int minElev = (int)(ter->min_bounding.z);
    const std::size_t arraySize = maxElev - minElev + 1;

    std::vector<std::queue<runnel::Point*>> queueArray(arraySize, std::queue<runnel::Point*>());

    for(runnel::Point* point : this->ter->struct_point){
        bool isBoundaryPoint = setInitialDirection(point);
        if(isBoundaryPoint){
            queueArray[point->coord.z - minElev].push(point);
        }
    }

    for(int z = minElev; z <= maxElev; ++z){
        while(!queueArray[z-minElev].empty()){
            runnel::Point* point = queueArray[z-minElev].front();
            queueArray[z-minElev].pop();

            std::vector<runnel::Point*> neighborhood = computeNeighborhood(point);
            for(runnel::Point* neighbor : neighborhood){
                if(neighbor->flags==0){

                }
            }
        }
    }
}

bool RWFloodAlgorithm::setInitialDirection(runnel::Point* point){
    if(point->ident == 0){
        /*Top-left corner of raster*/
        point->setFlagsOn(TOP_LEFT);
    } else if (point->ident < ter->width - 1){
        point->setFlagsOn(TOP);
    } else if (point->ident == ter->width - 1){
        point->setFlagsOn(TOP_RIGHT);
    } else if (point->ident == ter->width*(ter->height-1)){
        point->setFlagsOn(BOTTOM_LEFT);
    } else if (point->ident > ter->width*(ter->height-1) &&
               point->ident < ter->width*ter->height - 1){
        point->setFlagsOn(BOTTOM);
    } else if (point->ident == ter->width*ter->height - 1){
        point->setFlagsOn(BOTTOM_RIGHT);
    } else if (point->ident%ter->width == 0){
        point->setFlagsOn(LEFT);
    } else if (point->ident%ter->width == ter->width - 1){
        point->setFlagsOn(RIGHT);
    } else {
        /*Not a boundary point; set a null direction and return*/
        point->flags = 0;
        return false;
    }
    return true;
}

std::vector<runnel::Point*> RWFloodAlgorithm::computeNeighborhood(runnel::Point* point){
    std::vector<runnel::Point*> neighborhood;
    if(point->ident == 0){
        neighborhood.push_back(ter->struct_point[point->ident + 1]);
        neighborhood.push_back(ter->struct_point[point->ident + ter->width]);
        neighborhood.push_back(ter->struct_point[point->ident + ter->width + 1]);
    } else if (point->ident < ter->width - 1){
        neighborhood.push_back(ter->struct_point[point->ident - 1]);
        neighborhood.push_back(ter->struct_point[point->ident + 1]);
        neighborhood.push_back(ter->struct_point[point->ident + ter->width - 1]);
        neighborhood.push_back(ter->struct_point[point->ident + ter->width]);
        neighborhood.push_back(ter->struct_point[point->ident + ter->width + 1]);
    } else if (point->ident == ter->width - 1){
        neighborhood.push_back(ter->struct_point[point->ident - 1]);
        neighborhood.push_back(ter->struct_point[point->ident + ter->width - 1]);
        neighborhood.push_back(ter->struct_point[point->ident + ter->width]);
    } else if (point->ident == ter->width*(ter->height-1)){
        neighborhood.push_back(ter->struct_point[point->ident - ter->width]);
        neighborhood.push_back(ter->struct_point[point->ident - ter->width + 1]);
        neighborhood.push_back(ter->struct_point[point->ident + 1]);
    } else if (point->ident > ter->width*(ter->height-1) &&
               point->ident < ter->width*ter->height - 1){
        neighborhood.push_back(ter->struct_point[point->ident - ter->width - 1]);
        neighborhood.push_back(ter->struct_point[point->ident - ter->width]);
        neighborhood.push_back(ter->struct_point[point->ident - ter->width + 1]);
        neighborhood.push_back(ter->struct_point[point->ident - 1]);
        neighborhood.push_back(ter->struct_point[point->ident + 1]);
    } else if (point->ident == ter->width*ter->height - 1){
        neighborhood.push_back(ter->struct_point[point->ident - ter->width - 1]);
        neighborhood.push_back(ter->struct_point[point->ident - ter->width]);
        neighborhood.push_back(ter->struct_point[point->ident - 1]);
    } else if (point->ident%ter->width == 0){
        neighborhood.push_back(ter->struct_point[point->ident - ter->width]);
        neighborhood.push_back(ter->struct_point[point->ident - ter->width + 1]);
        neighborhood.push_back(ter->struct_point[point->ident + 1]);
        neighborhood.push_back(ter->struct_point[point->ident + ter->width]);
        neighborhood.push_back(ter->struct_point[point->ident + ter->width + 1]);
    } else if (point->ident%ter->width == ter->width - 1){
        neighborhood.push_back(ter->struct_point[point->ident - ter->width - 1]);
        neighborhood.push_back(ter->struct_point[point->ident - ter->width]);
        neighborhood.push_back(ter->struct_point[point->ident - 1]);
        neighborhood.push_back(ter->struct_point[point->ident + ter->width - 1]);
        neighborhood.push_back(ter->struct_point[point->ident + ter->width]);
    } else {
        neighborhood.push_back(ter->struct_point[point->ident - ter->width - 1]);
        neighborhood.push_back(ter->struct_point[point->ident - ter->width]);
        neighborhood.push_back(ter->struct_point[point->ident - ter->width + 1]);
        neighborhood.push_back(ter->struct_point[point->ident - 1]);
        neighborhood.push_back(ter->struct_point[point->ident + 1]);
        neighborhood.push_back(ter->struct_point[point->ident + ter->width - 1]);
        neighborhood.push_back(ter->struct_point[point->ident + ter->width]);
        neighborhood.push_back(ter->struct_point[point->ident + ter->width + 1]);
    }
    neighborhood.shrink_to_fit();
    return neighborhood;
}
