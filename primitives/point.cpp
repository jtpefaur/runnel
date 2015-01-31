#include "point.h"

runnel::Point::Point(glm::vec3 p, int i)
{
    coord = p;
    ident = i;
    flags = 0;
    water_value = 1;
}

void runnel::Point::setFlagsOn(int position){
     char bitmask = 0x01 << position;
     flags = flags | bitmask;
}

bool runnel::Point::isFlagsOn(int position){
    char bitmask = 0x01 << position;
    char value = flags & bitmask;
    if (value != 0){
        return true;
    }else{
        return false;
    }
}

void runnel::Point::setFlagsOff(int position){
    char bitmask = 0x01 << position;
    flags = flags & ~bitmask;
}
