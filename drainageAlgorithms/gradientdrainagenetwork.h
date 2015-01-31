#ifndef GRADIENTDRAINAGENETWORK_H
#define GRADIENTDRAINAGENETWORK_H

#include "primitives/triangle.h"
#include "terrain.h"

class GradientDrainageNetwork
{
    public:
         static void calculateFinalPointTriangle(std::vector<glm::vec3>& points_gradient, std::vector<runnel::Point *> points ,runnel::Triangle* t, glm::vec3 pto);
         static bool isOnEdge(runnel::Point* p1, runnel::Point* p2, glm::vec3 pto);
         static std::vector<runnel::Edge*> getEdgeOpposite(std::vector<runnel::Point *> points, runnel::Triangle*trian, glm::vec3 pto);
         static glm::vec3 calculatePointInterseption(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec3 p4);
         static runnel::Triangle* getTriangleForEdge(runnel::Edge* ed, runnel::Triangle* t);
         static void calculateDrainage(std::vector<glm::vec3>& points_gradient, Terrain *ter, glm::vec3 pto);
         static bool isSamePoint(glm::vec3 pto_result, glm::vec3 pto_initial);
         static void getTriangleNeigthbour(runnel::Triangle* t_initial, std::vector<runnel::Triangle *>& triangle_neighbour, std::vector<runnel::Point *> points, runnel::Triangle* trian, glm::vec3 pto, glm::vec3 pto_edge, runnel::Triangle* ant_trian);
};

#endif // GRADIENTDRAINAGENETWORK_H
