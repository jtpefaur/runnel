#ifndef EDGE_H
#define EDGE_H
#include <vector>
#include "primitives/point.h"

#include "lib/glm/glm.hpp"

namespace runnel {
class Triangle;
class Edge
{
    public:
        std::vector< runnel::Triangle*> neighbour_triangle;
        glm::vec3 edge_vector;
        runnel::Point* point1;
        runnel::Point* point2;
        Edge(runnel::Point* value1, runnel::Point* value2);
        void addTriangle(runnel::Triangle* t);
        void calculateEdgeVector();
        float calculateAngleDiedro();

};
}
#endif // EDGE_H
