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
            std::vector< runnel::Point*> points;
            std::vector< runnel::Edge*> edges;
            std::vector<glm::vec3> gradient;
            glm::vec3 normal;
            glm::vec3 incentro;
            int ident;
            void addPoint(runnel::Point* p);
            void addGroupPoints(runnel::Point* p1, runnel::Point* p2, runnel::Point* p3);
            void addEdge(runnel::Edge* e);
            void addGroupEdges(runnel::Edge* e1, runnel::Edge* e2, runnel::Edge* e3);
            void calculateNormalVector();
            void calculateIncentroPoint();
            glm::vec3 calculateHeight();
            std::vector<Triangle*> getNeighbours();

            void calculateLineHorizontal();
    };
}
#endif // TRIANGLE_H
