#ifndef TERRAIN_H
#define TERRAIN_H

#include <vector>
#include "primitives/point.h"
#include "primitives/triangle.h"
#include "lib/glm/glm.hpp"
#include "unordered_map"
#include <unordered_set>


class Terrain
{
    public:
        Terrain();

        int width;
        int height;
        bool signalPaintGoogle;
        float lat0;
        float lng0;
        float ratio;
        glm::mat4 matrix_esc;
        glm::vec3 max_bounding;
        glm::vec3 min_bounding;
        glm::vec3 sigma;
        glm::vec3 media;
        std::vector<runnel::Point*> struct_point;
        std::vector<runnel::Triangle*> struct_triangle;
        std::vector<runnel::Edge*> struct_edge;
        std::vector<glm::vec3> vector_gradient_color;
        std::unordered_map< runnel::Point*, std::unordered_set<runnel::Triangle*>> trianglesContainingPoint;


        void addPoint(runnel::Point* p);
        void addTriangle(runnel::Triangle* t);
        void normalize();
        void addEdge(runnel::Edge* ed);
        void setBoundingBox(glm::vec3 coords);
        float minumum(std::vector<runnel::Triangle *> list_triangle, runnel::Triangle* trian);


        glm::vec3 getMedia();
        glm::vec3 getSigma();
        std::vector<runnel::Point*> getPoints();
        std::vector<glm::vec3> getVectorPoints();
        std::vector<glm::vec3> getVectorNormals();
        std::vector<glm::vec3> calculateNeighbourByEdges();
        std::vector<glm::vec3> calculateHeightArray();
        std::vector<glm::vec3> getDrainageColor();
        std::vector<glm::vec3> getPointsEdgeDrainage();
        std::vector<glm::vec3> getCoordinateAxis();
        void getMoreWaterPoint();
        void setMapPixel(int ancho_pix, int largo_pix);
        void setRatio();
        runnel::Triangle* getClosestTriangle(glm::vec3 p);
        bool isBorderPoint(runnel::Point* point);
        bool isBorderEdge(runnel::Edge* edge);
};

#endif // TERRAIN_H
