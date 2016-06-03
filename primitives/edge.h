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
        int id1;
        int id2;
        float angulo_diedro;
        Edge(runnel::Point* value1, runnel::Point* value2);
        void addTriangle(runnel::Triangle* t);
        void calculateAngleDiedro();
        glm::vec3 getEdgeVector(runnel::Triangle* t1);

};
}
#endif // EDGE_H
