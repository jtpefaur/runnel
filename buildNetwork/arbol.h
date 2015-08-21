#ifndef ARBOL_H
#define ARBOL_H
#include <vector>
#include "primitives/point.h"
#include <unordered_map>
#include <map>

using EdgeList = std::vector<std::pair<runnel::Point*, runnel::Point*>>;

class Terrain;

class arbol
{
    public:
        arbol(runnel::Point* p);
        static const int NO_NUMBER_STRAHLER_HORTON = -2;
        std::vector<glm::vec3> colores;
        std::vector<glm::vec3> color_arbolitos;
        runnel::Point* pto;
        std::unordered_map <std::string, glm::vec3> type_colors;
        std::vector<int> numbers_strahler_horton;
        int number_strahler_horton;
        std::vector<arbol *> hijos;

        std::map<int, std::vector<int> > makeInflowingEdgeMap(EdgeList &edgeList);
        EdgeList makeEdgeList(std::vector<runnel::Point*> &edges);
        std::map<int, int> makeUpstreamNodePerEdgeMap(EdgeList &edgeList);

        void getArbolEdges(std::vector<runnel::Point*> &edges);
        void getArbolEdges(std::vector<runnel::Point*> &edges, int orderThreshold);
        void computeNetworkStrahlerOrdering();

        void getColorEdgesType(std::vector<glm::vec3>& color_edges, std::string type_color);
        void getColorEdges(std::vector<glm::vec3>& color_edges);
        void getColorEdges(std::vector<glm::vec3>& color_edges, int orderThreshold);

    private:
        int streamOrdering(int edgeIndex, std::map<int,bool> &visitedEdges, std::map<int, std::vector<int> > &inflowingEdgesPerNode, std::map<int, int> &upstreamNodePerEdge, std::map<int,int> &streamOrders, std::map<int,int> &originatingNode);
        void updateStrahlerOrder(EdgeList &edgeList, std::map<int,int> &streamOrders);
};

#endif // ARBOL_H
