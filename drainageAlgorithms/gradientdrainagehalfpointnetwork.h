#ifndef GRADIENTDRAINAGEHALFPOINTNETWORK_H
#define GRADIENTDRAINAGEHALFPOINTNETWORK_H

#include "terrain.h"

class GradientDrainageHalfPointNetwork
{
    public:
        static void calculateDrainage(Terrain* ter, glm::vec3 pto, std::vector<glm::vec3>& points_gradient);
        static void calculateNextTriangle(runnel::Triangle* trian, std::vector<runnel::Point*> points, std::vector<glm::vec3>& points_gradient);
        static glm::vec3 calculatePointInterception(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec3 p4);
        static bool isSamePoint(glm::vec3 pto1, glm::vec3 pto2);
        static bool isOnEdge(runnel::Point* p1, runnel::Point* p2, glm::vec3 pto);
        static runnel::Triangle* calculateOtherTriangleInEdge(runnel::Triangle* trian_initial, runnel::Edge* ed);
};

#endif // GRADIENTDRAINAGEHALFPOINTNETWORK_H
