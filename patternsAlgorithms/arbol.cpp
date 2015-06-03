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


void arbol::getArbolEdges(std::vector<glm::vec3>& edges){
    getArbolEdges(edges, 1);
}

void arbol::getArbolEdges(std::vector<glm::vec3> &edges, int orderThreshold) {
    for (arbol* h: hijos){
        if(h->number_strahler_horton >= orderThreshold) {
            edges.push_back(pto->coord);
            edges.push_back(h->pto->coord);
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
