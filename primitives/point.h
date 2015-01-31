#ifndef POINT_H
#define POINT_H
#include "lib/glm/glm.hpp"
#include <vector>


namespace runnel {
    class Point
    {
        public:

            static const int PEUCKER = 0;


            glm::vec3 coord;
            int ident;
            int water_value;
            std::vector<runnel::Point*> water_parent;
            char flags; //0 representa el calculo del algoritmo de grilla 2x2 peucker and douglas
            Point(glm::vec3 p, int i);
            void setFlagsOn(int position);
            bool isFlagsOn(int position);
            void setFlagsOff(int position);


    };
}
#endif // POINT_H

