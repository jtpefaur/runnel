#include "rwfloodalgorithm.h"
#include <memory>
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
    this->calculateWaterAccumulation();
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
        bool isBoundaryPoint = initializeDirection(point);
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
                    setDirectionTowardsAdjacentPoint(neighbor, point);
                    if(neighbor->coord.z < z){
                        /* Warning: this modifies the data! Might want to make a
                         * local copy instead.*/
                        neighbor->coord.z = z;
                    }
                    queueArray[neighbor->coord.z - minElev].push(neighbor);
                }
            }
        }
    }
}

void RWFloodAlgorithm::calculateWaterAccumulation()
{
    std::vector<int> inboundDegree(ter->struct_point.size());
    std::vector<bool> pointIsVisited(ter->struct_point.size());

    for (runnel::Point* point : ter->struct_point) {
        point->water_value = 1;
        inboundDegree[point->ident] = 0;
        pointIsVisited[point->ident] = false;
    }

    for (runnel::Point* point : ter->struct_point) {
        if (!isDirectedOutsideTerrainBoundary(point)) {
            inboundDegree[getNextPointId(point)]++;
        }
    }

    for (runnel::Point* point : ter->struct_point) {
        if (!pointIsVisited[point->ident]) {
            runnel::Point* currentPoint = point;
            while (inboundDegree[currentPoint->ident] == 0) {
                pointIsVisited[currentPoint->ident] = true;
                if (isDirectedOutsideTerrainBoundary(currentPoint)) break;
                runnel::Point* nextPoint =
                        ter->struct_point[getNextPointId(currentPoint)];
                nextPoint->water_value += currentPoint->water_value;
                inboundDegree[nextPoint->ident]--;
                currentPoint = nextPoint;
            }
        }
    }
}

bool RWFloodAlgorithm::initializeDirection(runnel::Point* point){
    int id = point->ident;
    int width = ter->width;
    int height = ter->height;
    if (id == 0) {
        /*Top-left corner of raster*/
        point->setFlagsOn(TOP_LEFT);
    } else if (id < width - 1) {
        point->setFlagsOn(TOP);
    } else if (id == width - 1) {
        point->setFlagsOn(TOP_RIGHT);
    } else if (id == width*(height - 1)) {
        point->setFlagsOn(BOTTOM_LEFT);
    } else if (id > width*(height - 1) &&
               id < width*height - 1) {
        point->setFlagsOn(BOTTOM);
    } else if (id == width*height - 1) {
        point->setFlagsOn(BOTTOM_RIGHT);
    } else if (id%width == 0) {
        point->setFlagsOn(LEFT);
    } else if (id%width == width - 1) {
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
    int id = point->ident;
    int width = ter->width;
    int height = ter->height;

    if (id == 0) {
        neighborhood.push_back(ter->struct_point[id + 1]);
        neighborhood.push_back(ter->struct_point[id + width]);
        neighborhood.push_back(ter->struct_point[id + width + 1]);
    } else if (id < width - 1) {
        neighborhood.push_back(ter->struct_point[id - 1]);
        neighborhood.push_back(ter->struct_point[id + 1]);
        neighborhood.push_back(ter->struct_point[id + width - 1]);
        neighborhood.push_back(ter->struct_point[id + width]);
        neighborhood.push_back(ter->struct_point[id + width + 1]);
    } else if (id == width - 1) {
        neighborhood.push_back(ter->struct_point[id - 1]);
        neighborhood.push_back(ter->struct_point[id + width - 1]);
        neighborhood.push_back(ter->struct_point[id + width]);
    } else if (id == width*(height - 1)) {
        neighborhood.push_back(ter->struct_point[id - width]);
        neighborhood.push_back(ter->struct_point[id - width + 1]);
        neighborhood.push_back(ter->struct_point[id + 1]);
    } else if (id > width*(height - 1) && id < width*height - 1) {
        neighborhood.push_back(ter->struct_point[id - width - 1]);
        neighborhood.push_back(ter->struct_point[id - width]);
        neighborhood.push_back(ter->struct_point[id - width + 1]);
        neighborhood.push_back(ter->struct_point[id - 1]);
        neighborhood.push_back(ter->struct_point[id + 1]);
    } else if (id == width*height - 1) {
        neighborhood.push_back(ter->struct_point[id - width - 1]);
        neighborhood.push_back(ter->struct_point[id - width]);
        neighborhood.push_back(ter->struct_point[id - 1]);
    } else if (id%width == 0) {
        neighborhood.push_back(ter->struct_point[id - width]);
        neighborhood.push_back(ter->struct_point[id - width + 1]);
        neighborhood.push_back(ter->struct_point[id + 1]);
        neighborhood.push_back(ter->struct_point[id + width]);
        neighborhood.push_back(ter->struct_point[id + width + 1]);
    } else if (id%width == width - 1) {
        neighborhood.push_back(ter->struct_point[id - width - 1]);
        neighborhood.push_back(ter->struct_point[id - width]);
        neighborhood.push_back(ter->struct_point[id - 1]);
        neighborhood.push_back(ter->struct_point[id + width - 1]);
        neighborhood.push_back(ter->struct_point[id + width]);
    } else {
        neighborhood.push_back(ter->struct_point[id - width - 1]);
        neighborhood.push_back(ter->struct_point[id - width]);
        neighborhood.push_back(ter->struct_point[id - width + 1]);
        neighborhood.push_back(ter->struct_point[id - 1]);
        neighborhood.push_back(ter->struct_point[id + 1]);
        neighborhood.push_back(ter->struct_point[id + width - 1]);
        neighborhood.push_back(ter->struct_point[id + width]);
        neighborhood.push_back(ter->struct_point[id + width + 1]);
    }
    neighborhood.shrink_to_fit();
    return neighborhood;
}

void RWFloodAlgorithm::setDirectionTowardsAdjacentPoint(runnel::Point* source, runnel::Point* destination){
    int diff = destination->ident - source->ident;
    int width = ter->width;

    if (diff == -width - 1) {
        source->setFlagsOn(TOP_LEFT);
    } else if (diff == -width) {
        source->setFlagsOn(TOP);
    } else if (diff == -width + 1) {
        source->setFlagsOn(TOP_RIGHT);
    } else if (diff == -1) {
        source->setFlagsOn(LEFT);
    } else if (diff == 1) {
        source->setFlagsOn(RIGHT);
    } else if (diff == width -1) {
        source->setFlagsOn(BOTTOM_LEFT);
    } else if (diff == width) {
        source->setFlagsOn(BOTTOM);
    } else if (diff == width + 1) {
        source->setFlagsOn(BOTTOM_RIGHT);
    }
}

bool RWFloodAlgorithm::isDirectedOutsideTerrainBoundary(runnel::Point* point) {
    // Pre-condition: point->flags != 0 (a direction must be set.)
    int id = point->ident;
    int height = ter->height;
    int width = ter->width;

    if (id < width &&
        (point->isFlagsOn(TOP) ||
         point->isFlagsOn(TOP_LEFT) ||
         point->isFlagsOn(TOP_RIGHT))) {
        return true;
    }

    if (id%width == 0 &&
        (point->isFlagsOn(LEFT) ||
         point->isFlagsOn(TOP_LEFT) ||
         point->isFlagsOn(BOTTOM_LEFT))) {
        return true;
    }

    if (id%width == width-1 &&
        (point->isFlagsOn(RIGHT) ||
         point->isFlagsOn(TOP_RIGHT) ||
         point->isFlagsOn(BOTTOM_RIGHT))) {
        return true;
    }

    if (id >= width*(height - 1) &&
        (point->isFlagsOn(BOTTOM) ||
         point->isFlagsOn(BOTTOM_LEFT) ||
         point->isFlagsOn(BOTTOM_RIGHT))){
        return true;
    }

    return false;
}

int RWFloodAlgorithm::getNextPointId(runnel::Point* point) {
    // Pre-condition: !isDirectedOutsideTerrainBoundary(point) == true
    int id = point->ident;
    int width = ter->width;

    if (point->isFlagsOn(TOP_LEFT)) return (id - width - 1);
    if (point->isFlagsOn(TOP)) return (id - width);
    if (point->isFlagsOn(TOP_RIGHT)) return (id - width + 1);
    if (point->isFlagsOn(LEFT)) return (id - 1);
    if (point->isFlagsOn(RIGHT)) return (id + 1);
    if (point->isFlagsOn(BOTTOM_LEFT)) return (id + width - 1);
    if (point->isFlagsOn(BOTTOM)) return (id + width);
    if (point->isFlagsOn(BOTTOM_RIGHT)) return (id + width + 1);

    return -1;
}
