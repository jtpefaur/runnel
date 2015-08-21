#include "arbol.h"
#include <iostream>
#include <unordered_map>

arbol::arbol(runnel::Point* p)
{
    pto = p;
    number_strahler_horton = NO_NUMBER_STRAHLER_HORTON;
    colores.push_back(glm::vec3(0, 1, 43.0/255));
    colores.push_back(glm::vec3(0, 77.0/255, 1));
    colores.push_back(glm::vec3(1, 68.0/255, 0));
    colores.push_back(glm::vec3(247.0/255, 1, 0));
    colores.push_back(glm::vec3(239.0/255, 0, 1));
    colores.push_back(glm::vec3(1, 179.0/255, 0));
    colores.push_back(glm::vec3(0, 220.0/255, 1));
    type_colors["rectangular"] = glm::vec3(1,0,0);
    type_colors["trellis"] = glm::vec3(0,1,0);
    type_colors["parallel"] = glm::vec3(0,0,1);
    type_colors["dendritic"] = glm::vec3(1,1,0);
    type_colors[""] = glm::vec3(1,1,1);
}

std::map<int, std::vector<int>> arbol::makeInflowingEdgeMap(EdgeList &edgeList)
{
    std::map<int, std::vector<int>> inflowingEdgeMap;

    for (auto iter = edgeList.begin(); iter != edgeList.end(); std::advance(iter, 1)) {
        runnel::Point* p1 = (*iter).first;
        runnel::Point* p2 = (*iter).second;
        if (p1->coord.z < p2->coord.z) {
            // We save the edge's index in edgeList, not the point's ID!
            inflowingEdgeMap[p1->ident].push_back(iter - edgeList.begin());
        } else {
            inflowingEdgeMap[p2->ident].push_back(iter - edgeList.begin());
        }
    }

    return inflowingEdgeMap;
}

EdgeList arbol::makeEdgeList(std::vector<runnel::Point*> &edges)
{
    EdgeList edgeList;

    for (auto iter = edges.begin(); iter != edges.end(); std::advance(iter, 2)) {
        // Each even-indexed point in the vector is followed by a point with which it forms an edge.
        runnel::Point* p1 = *iter;
        runnel::Point* p2 = *(iter+1);
        std::pair<runnel::Point*, runnel::Point*> edgePair(p1, p2);
        edgeList.push_back(edgePair);
    }

    return edgeList;
}

std::map<int, int> arbol::makeUpstreamNodePerEdgeMap(EdgeList &edgeList)
{
    std::map<int, int> upstreamNodePerEdge;

    for (auto iter = edgeList.begin(); iter != edgeList.end(); std::advance(iter, 1)) {
        runnel::Point* p1 = (*iter).first;
        runnel::Point* p2 = (*iter).second;
        if (p1->coord.z < p2->coord.z) {
            upstreamNodePerEdge[iter - edgeList.begin()] = p2->ident;
        } else {
            upstreamNodePerEdge[iter - edgeList.begin()] = p1->ident;
        }
    }

    return upstreamNodePerEdge;
}

void arbol::getArbolEdges(std::vector<runnel::Point*>& edges){
    getArbolEdges(edges, 1);
}

void arbol::getArbolEdges(std::vector<runnel::Point*> &edges, int orderThreshold) {
    for (arbol* h: hijos){
        edges.push_back(pto);
        edges.push_back(h->pto);

        h->getArbolEdges(edges, orderThreshold);
    }
}

void arbol::getColorEdges(std::vector<glm::vec3>& color_edges) {
    getColorEdges(color_edges, 1);
}

void arbol::getColorEdges(std::vector<glm::vec3> &color_edges, int orderThreshold) {
    for (arbol* h: hijos){
        //if (h->number_strahler_horton >= orderThreshold) {
            if(h->number_strahler_horton > colores.size()){
                color_edges.push_back(colores[colores.size()-1]);
                color_edges.push_back(colores[colores.size()-1]);
            }else{
                color_edges.push_back(colores[h->number_strahler_horton - 1]);
                color_edges.push_back(colores[h->number_strahler_horton - 1]);
            }
        //}
      h->getColorEdges(color_edges, orderThreshold);
    }
}

void arbol::computeNetworkStrahlerOrdering(){
    std::vector<runnel::Point*> edges;
    this->getArbolEdges(edges);
    EdgeList edgeList = makeEdgeList(edges);
    std::map<int, bool> visitedEdges;
    std::map<int, std::vector<int>> inflowingEdgesPerNode = makeInflowingEdgeMap(edgeList);
    std::map<int, int> upstreamNodePerEdge = makeUpstreamNodePerEdgeMap(edgeList);
    std::map<int, int> streamOrders;
    std::map<int, int> originatingNode;

    streamOrdering(0, visitedEdges, inflowingEdgesPerNode, upstreamNodePerEdge, streamOrders, originatingNode);

    this->updateStrahlerOrder(edgeList, streamOrders);
}

int arbol::streamOrdering(int edgeIndex, std::map<int, bool> &visitedEdges,
                          std::map<int, std::vector<int>> &inflowingEdgesPerNode,
                          std::map<int, int> &upstreamNodePerEdge,
                          std::map<int, int> &streamOrders,
                          std::map<int, int> &originatingNode)
{
    visitedEdges[edgeIndex] = true;

    if (inflowingEdgesPerNode[upstreamNodePerEdge[edgeIndex]].size() == 0) {
        streamOrders[edgeIndex] = 1;
    } else {
        std::map<int, std::pair<int,int>> upstreamOrders;

        for (int inflowingEdgeIndex : inflowingEdgesPerNode[upstreamNodePerEdge[edgeIndex]]) {
            if (!visitedEdges[inflowingEdgeIndex]) {
                upstreamOrders[inflowingEdgeIndex] = std::make_pair(streamOrdering(inflowingEdgeIndex,
                                                                                   visitedEdges,
                                                                                   inflowingEdgesPerNode,
                                                                                   upstreamNodePerEdge,
                                                                                   streamOrders,
                                                                                   originatingNode),
                                                                    originatingNode[inflowingEdgeIndex]);
            } else {
                upstreamOrders[inflowingEdgeIndex] = std::make_pair(streamOrders[inflowingEdgeIndex],
                                                                    originatingNode[inflowingEdgeIndex]);
            }
        }

        int maxOrder = 0;
        int maxOrderCount = 0;
        int maxOrderOrigin = -1;

        for (auto iter = upstreamOrders.begin(); iter != upstreamOrders.end(); std::advance(iter, 1)) {
            std::pair<int,int> orderOriginPair = (*iter).second;
            int order = orderOriginPair.first;
            int origin = orderOriginPair.second;

            if (order > maxOrder) {
                maxOrder = order;
                maxOrderCount = 1;
                maxOrderOrigin = origin;
            } else if (order == maxOrder) {
                if (origin != maxOrderOrigin) {
                    maxOrderCount += 1;
                }
            }
        }

        if (maxOrderCount > 1) {
            streamOrders[edgeIndex] = maxOrder + 1;
            originatingNode[edgeIndex] = upstreamNodePerEdge[edgeIndex];
        } else {
            streamOrders[edgeIndex] = maxOrder;
            originatingNode[edgeIndex] = maxOrderOrigin;
        }
    }

    return streamOrders[edgeIndex];
}

void arbol::updateStrahlerOrder(EdgeList &edgeList, std::map<int, int> &streamOrders)
{
    if (this->number_strahler_horton != NO_NUMBER_STRAHLER_HORTON) {
        return;
    }

    int maxOrder = 0;

    for (auto iter = edgeList.begin(); iter != edgeList.end(); std::advance(iter, 1)) {
        auto edge = (*iter);
        runnel::Point* p = edge.first;
        if (this->pto->ident == p->ident && streamOrders[iter - edgeList.begin()] > maxOrder) {
            maxOrder = streamOrders[iter - edgeList.begin()];
        }
    }

    this->number_strahler_horton = maxOrder;

    for (arbol* child : this->hijos) {
        child->updateStrahlerOrder(edgeList, streamOrders);
    }
}


void arbol::getColorEdgesType(std::vector<glm::vec3>& color_edges, std::string type_color){
    for(arbol* child: hijos){
        color_edges.push_back(type_colors[type_color]);
        color_edges.push_back(type_colors[type_color]);
        child->getColorEdgesType(color_edges, type_color);
    }
}
