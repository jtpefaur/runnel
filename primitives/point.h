#ifndef POINT_H
#define POINT_H
#include "lib/glm/glm.hpp"
#include <vector>


namespace runnel {
    class Point
    {
        public:

            static const int PEUCKER = 1;

            std::vector<runnel::Point*> water_parent;
            glm::vec3 coord;
            int ident;
            int water_value;
            char flags; //0 representa el calculo del algoritmo de grilla 2x2 peucker and douglas
            Point(glm::vec3 p, int i);


    };
}
#endif // POINT_H

