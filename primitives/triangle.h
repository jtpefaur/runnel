#ifndef TRIANGLE_H
#define TRIANGLE_H
#include "primitives/point.h"
#include "primitives/edge.h"
#include <vector>
#include "lib/glm/glm.hpp"


namespace runnel {
    class Triangle
    {
        public:
            Triangle(int id);
            int ident;
            std::vector< runnel::Point*> points;
            std::vector< runnel::Edge*> edges;
            std::vector<int> position;
            std::vector<float> minimun_value_angle_point; //uno para cada punto
            std::vector<glm::vec3> gradient;
            glm::vec3 normal;
            glm::vec3 incentro;
            glm::vec3 sides;
            glm::vec3 height;
            glm::vec3 gradient_vector;
            void addPoint(runnel::Point* p);
            void addGroupPoints(runnel::Point* p1, runnel::Point* p2, runnel::Point* p3);
            void addEdge(runnel::Edge* e);
            void addGroupEdges(runnel::Edge* e1, runnel::Edge* e2, runnel::Edge* e3);
            void addPositionPoint(int x);
            void calculateNormalVector();
            void calculateIncentroPoint();
            void calculateHeight();
            std::vector<Triangle*> getNeighbours();

            glm::vec3 calculateLineHorizontal();
            std::vector<glm::vec3> calculate_orden();
    };
}
#endif // TRIANGLE_H
