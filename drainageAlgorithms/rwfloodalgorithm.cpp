#include "rwfloodalgorithm.h"
#include <queue>

RWFloodAlgorithm::RWFloodAlgorithm():
    DrainageAlgorithms()
{
    this->ter = 0;
    this->shader = 0;
    this->maxWaterCount = 0;
    this->waterThreshold = 10;

    QObject::connect(&conf, SIGNAL(changeAttr()), this, SLOT(changeAttr()));
}

RWFloodAlgorithm::~RWFloodAlgorithm()
{
    if (shader) {
        delete shader;
    }
}

void RWFloodAlgorithm::run(Terrain *ter)
{
    this->ter = ter;
    this->flood(this->ter->struct_point);
    this->calculateWaterAccumulation(this->ter->struct_point);
    drainageColors.clear();
    drainagePoints.clear();
    this->getDrainagePoints();
    this->shader->fillBuffers(this->drainagePoints, this->drainageColors);
}

void RWFloodAlgorithm::glewReady()
{
    this->shader = new ShaderRWFlood();
}

void RWFloodAlgorithm::render(glm::mat4 matrix, float exagValue, glm::vec3)
{
    if (shader) {
        shader->render(matrix, exagValue);
    }
}

QString RWFloodAlgorithm::getName()
{
    return QString("RWFlood");
}

QWidget* RWFloodAlgorithm::getConf()
{
    return &conf;
}

std::vector<glm::vec3> RWFloodAlgorithm::getPathTree()
{
    std::vector<glm::vec3> path;
    return path;
}

void RWFloodAlgorithm::flood(std::vector<runnel::Point*>& points)
{
    const int maxElev = (int)(ter->max_bounding.z);
    const int minElev = (int)(ter->min_bounding.z);
    const std::size_t arraySize = maxElev - minElev + 1;

    std::vector<std::queue<runnel::Point*>> queueArray(arraySize,
                                                       std::queue<runnel::Point*>());
    std::map<int, float> zValueStore;

    for (runnel::Point* point : points) {
        point->flags = 0;
        bool isBoundaryPoint = initializeDirection(points, point);
        if (isBoundaryPoint) {
            queueArray[point->coord.z - minElev].push(point);
        }
    }

    for (int z = minElev; z <= maxElev; ++z) {
        while (!queueArray[z-minElev].empty()) {
            runnel::Point* point = queueArray[z-minElev].front();
            queueArray[z-minElev].pop();

            std::vector<runnel::Point*> neighborhood = computeNeighborhood(point);
            for (runnel::Point* neighbor : neighborhood) {
                if (neighbor->flags==0) {
                    setDirectionTowardsAdjacentPoint(points, neighbor, point);
                    if (neighbor->coord.z < z) {
                        /* Warning: this modifies the data!
                         * The original values are saved for later restoration. */
                        zValueStore[neighbor->ident] = neighbor->coord.z;
                        neighbor->coord.z = z;
                    }
                    queueArray[neighbor->coord.z - minElev].push(neighbor);
                }
            }
        }
    }

    // Restore modified z-coordinate values to their original values.
    for (auto &entry : zValueStore) {
        points[entry.first]->coord.z = entry.second;
    }
}

void RWFloodAlgorithm::calculateWaterAccumulation(std::vector<runnel::Point*>& points)
{
    int maxWaterCount = 1;
    std::vector<int> inboundDegree(points.size());
    std::vector<bool> pointIsVisited(points.size());

    for (runnel::Point* point : points) {
        points[point->ident]->water_value = 1;
        inboundDegree[point->ident] = 0;
        pointIsVisited[point->ident] = false;
        point->water_parent.clear();
    }

    for (runnel::Point* point : points) {
        if (!isDirectedOutsideTerrainBoundary(point)) {
            inboundDegree[getNextPointId(point)]++;
        }
    }

    for (runnel::Point* point : points) {
        if (!pointIsVisited[point->ident]) {
            runnel::Point* currentPoint = point;
            while (inboundDegree[currentPoint->ident] == 0) {
                pointIsVisited[currentPoint->ident] = true;
                if (isDirectedOutsideTerrainBoundary(currentPoint)) break;
                int nextPointId = getNextPointId(currentPoint);
                int currentPointId = currentPoint->ident;
                points[nextPointId]->water_value += points[currentPointId]->water_value;
                points[nextPointId]->water_parent.push_back(points[currentPointId]);
                if (points[nextPointId]->water_value > maxWaterCount) {
                    maxWaterCount = points[nextPointId]->water_value;
                }
                inboundDegree[nextPointId]--;
                currentPoint = points[nextPointId];
            }
        }
    }

    this->maxWaterCount = maxWaterCount;
}

void RWFloodAlgorithm::getDrainagePoints()
{
    for (runnel::Edge* edge : ter->struct_edge) {
        runnel::Point* p1 = ter->struct_point[edge->id1];
        runnel::Point* p2 = ter->struct_point[edge->id2];
        if (p1->water_value >= waterThreshold && p2->water_value >= waterThreshold) {
            float water1 = (float)p1->water_value / (float)maxWaterCount;
            float water2 = (float)p2->water_value / (float)maxWaterCount;
            this->drainagePoints.push_back(p1->coord);
            this->drainagePoints.push_back(p2->coord);
            this->drainageColors.push_back(glm::vec3(1.0f-water1, 1.0f-water1, 1.0f));
            this->drainageColors.push_back(glm::vec3(1.0f-water2, 1.0f-water2, 1.0f));
        }
    }
}

bool RWFloodAlgorithm::initializeDirection(std::vector<runnel::Point*>& points,
                                           runnel::Point* point)
{
    int id = point->ident;
    int width = ter->width;
    int height = ter->height;
    if (id == 0) {
        /*Top-left corner of raster*/
        points[id]->setFlagsOn(TOP_LEFT);
    } else if (id < width - 1) {
        points[id]->setFlagsOn(TOP);
    } else if (id == width - 1) {
        points[id]->setFlagsOn(TOP_RIGHT);
    } else if (id == width*(height - 1)) {
        points[id]->setFlagsOn(BOTTOM_LEFT);
    } else if (id > width*(height - 1) &&
               id < width*height - 1) {
        points[id]->setFlagsOn(BOTTOM);
    } else if (id == width*height - 1) {
        points[id]->setFlagsOn(BOTTOM_RIGHT);
    } else if (id%width == 0) {
        points[id]->setFlagsOn(LEFT);
    } else if (id%width == width - 1) {
        points[id]->setFlagsOn(RIGHT);
    } else {
        /*Not a boundary point; set a null direction and return*/
        points[id]->flags = 0;
        return false;
    }
    return true;
}

std::vector<runnel::Point*> RWFloodAlgorithm::computeNeighborhood(runnel::Point* point)
{
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

void RWFloodAlgorithm::setDirectionTowardsAdjacentPoint(std::vector<runnel::Point*>& points, runnel::Point* source, runnel::Point* destination)
{
    int sourceId = source->ident;
    int diff = destination->ident - source->ident;
    int width = ter->width;

    if (diff == -width - 1) {
        points[sourceId]->setFlagsOn(TOP_LEFT);
    } else if (diff == -width) {
        points[sourceId]->setFlagsOn(TOP);
    } else if (diff == -width + 1) {
        points[sourceId]->setFlagsOn(TOP_RIGHT);
    } else if (diff == -1) {
        points[sourceId]->setFlagsOn(LEFT);
    } else if (diff == 1) {
        points[sourceId]->setFlagsOn(RIGHT);
    } else if (diff == width -1) {
        points[sourceId]->setFlagsOn(BOTTOM_LEFT);
    } else if (diff == width) {
        points[sourceId]->setFlagsOn(BOTTOM);
    } else if (diff == width + 1) {
        points[sourceId]->setFlagsOn(BOTTOM_RIGHT);
    }
}

bool RWFloodAlgorithm::isDirectedOutsideTerrainBoundary(runnel::Point* point)
{
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

int RWFloodAlgorithm::getNextPointId(runnel::Point* point)
{
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

void RWFloodAlgorithm::changeAttr()
{
    this->waterThreshold = conf.getWaterThreshold();
    this->run(ter);
    emit reload();
}
