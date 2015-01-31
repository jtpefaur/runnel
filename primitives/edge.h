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
        Edge(runnel::Point* value1, runnel::Point* value2);
        float angulo_diedro;
        int id1;
        int id2;
        glm::vec3 vector_sort;
        void addTriangle(runnel::Triangle* t);
        glm::vec3 edge_vector;
        int calculate_angulo_diendro;
        std::vector< runnel::Triangle*> neighbour_triangle;
        void calculateAngleDiedro();
        glm::vec3 getEdgeVector(runnel::Triangle* t1);

};
}
#endif // EDGE_H
