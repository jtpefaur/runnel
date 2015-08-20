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

std::unordered_map<int, std::vector<int>> arbol::makeInflowingEdgeMap(EdgeList &edgeList)
{
    std::unordered_map<int, std::vector<int>> inflowingEdgeMap;

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
        if(h->number_strahler_horton >= orderThreshold) {
            edges.push_back(pto);
            edges.push_back(h->pto);
        }

        h->getArbolEdges(edges, orderThreshold);
    }
}

void arbol::getColorEdges(std::vector<glm::vec3>& color_edges) {
    getColorEdges(color_edges, 1);
}

void arbol::getColorEdges(std::vector<glm::vec3> &color_edges, int orderThreshold) {
    for (arbol* h: hijos){
        if (h->number_strahler_horton >= orderThreshold) {
            if(h->number_strahler_horton > colores.size()){
                color_edges.push_back(colores[colores.size()-1]);
                color_edges.push_back(colores[colores.size()-1]);
            }else{
                color_edges.push_back(colores[h->number_strahler_horton - 1]);
                color_edges.push_back(colores[h->number_strahler_horton - 1]);
            }
        }
      h->getColorEdges(color_edges, orderThreshold);
    }
}

void arbol::getNumberStrahlerHorton(){
    if(hijos.size() == 0){
        number_strahler_horton = 1;
        return;
    }

    std::unordered_map<int, int> ordenes;
    for(arbol*h : hijos){
        if(h->number_strahler_horton == NO_NUMBER_STRAHLER_HORTON){
            h->getNumberStrahlerHorton();
        }
        ordenes[h->number_strahler_horton] +=1;
    }
    int mayor_ord = 0;
    for( auto ord : ordenes){
        if(mayor_ord < ord.first){
            mayor_ord = ord.first;
        }
    }
    if(ordenes[mayor_ord] > 1){
        number_strahler_horton = mayor_ord + 1;
    }else{
        number_strahler_horton = mayor_ord;
    }
}

void arbol::getColorEdgesType(std::vector<glm::vec3>& color_edges, std::string type_color){
    for(arbol* child: hijos){
        color_edges.push_back(type_colors[type_color]);
        color_edges.push_back(type_colors[type_color]);
        child->getColorEdgesType(color_edges, type_color);
    }
}
