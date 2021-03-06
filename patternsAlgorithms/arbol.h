#ifndef ARBOL_H
#define ARBOL_H
#include <vector>
#include "primitives/point.h"
#include <unordered_map>
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
        void getArbolEdges(std::vector<glm::vec3>& edges);
        void getArbolEdges(std::vector<glm::vec3>& edges, int orderThreshold);
        void getNumberStrahlerHorton();

        void getColorEdgesType(std::vector<glm::vec3>& color_edges, std::string type_color);
        void getColorEdges(std::vector<glm::vec3>& color_edges);
        void getColorEdges(std::vector<glm::vec3>& color_edges, int orderThreshold);
};

#endif // ARBOL_H
