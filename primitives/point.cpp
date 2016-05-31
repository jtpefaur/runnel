#include "point.h"

runnel::Point::Point(glm::vec3 p, int i)
{
    coord = p;
    ident = i;
    flags = 0;
    water_value = 1;
}
