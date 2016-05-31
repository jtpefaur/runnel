int isDirectedOutsideTerrainBoundary(Point point, int pointsSize)
{
    int id = point.id;

    if (id < pointsSize &&
        (point.flag == TOP ||
         point.flag == TOP_LEFT ||
         point.flag == TOP_RIGHT)) {
        return 1;
    }

    if (id%pointsSize == 0 &&
        (point.flag == LEFT ||
         point.flag == TOP_LEFT ||
         point.flag == BOTTOM_LEFT)) {
        return 1;
    }

    if (id%pointsSize == pointsSize-1 &&
        (point.flag == RIGHT ||
         point.flag == TOP_RIGHT ||
         point.flag == BOTTOM_RIGHT)) {
        return 1;
    }

    if (id >= pointsSize*(pointsSize - 1) &&
        (point.flag == BOTTOM ||
         point.flag == BOTTOM_LEFT ||
         point.flag == BOTTOM_RIGHT)) {
        return 1;
    }

    return 0;
}

void setDirectionTowardsAdjacentPoint(__global float* flags, int sourceId, int destinationId, int pointsSize)
{
    int diff = destinationId - sourceId;

    if (diff == -pointsSize - 1) {
        points[sourceId].flag=TOP_LEFT;
    } else if (diff == -pointsSize) {
        points[sourceId].flag=TOP;
    } else if (diff == -pointsSize + 1) {
        points[sourceId].flag=TOP_RIGHT;
    } else if (diff == -1) {
        points[sourceId].flag=LEFT;
    } else if (diff == 1) {
        points[sourceId].flag=RIGHT;
    } else if (diff == pointsSize -1) {
        points[sourceId].flag=BOTTOM_LEFT;
    } else if (diff == pointsSize) {
        points[sourceId].flag=BOTTOM;
    } else if (diff == pointsSize + 1) {
        points[sourceId].flag=BOTTOM_RIGHT;
    }
}

int getNextPointId(Point point, int pointsSize)
{
    // Pre-condition: !isDirectedOutsideTerrainBoundary(point) == true
    int id = point.id;

    if (point.flag == TOP_LEFT) return (id - pointsSize - 1);
    if (point.flag == TOP) return (id - pointsSize);
    if (point.flag == TOP_RIGHT) return (id - pointsSize + 1);
    if (point.flag == LEFT) return (id - 1);
    if (point.flag == RIGHT) return (id + 1);
    if (point.flag == BOTTOM_LEFT) return (id + pointsSize - 1);
    if (point.flag == BOTTOM) return (id + pointsSize);
    if (point.flag == BOTTOM_RIGHT) return (id + pointsSize + 1);

    return -1;
}

__kernel void iterateQueues(__global Point* points, __global const int* pointsSize, __global int* queueArray, __global int* currentHeight, __global int* shouldRepeat) {

    int x = get_global_id(0);
    int y = get_global_id(1);
    int width = *pointsSize;
    int height = *currentHeight;
    int id = x*width + y;

    if(queueArray[id] == height) {

        queueArray[id] = -1;
        int neighborId;

        for(int i = 0; i<3; i++){

            neighborId = id - width -1 + i;

            if(neighborId > -1 && neighborId < width*width) {
                if(points[neighborId].flag == 0) {
                    setDirectionTowardsAdjacentPoint(points, neighborId, id, width);
                    if(points[neighborId].height <= height)
                    {
                        queueArray[neighborId] = height;
                        *shouldRepeat = 1;
                    }
                    else{
                        queueArray[neighborId] = points[neighborId].height;
                    }
                }
            }
        }

        neighborId = id - 1;
        if(neighborId > -1 && neighborId < width*width) {
            if(points[neighborId].flag == 0) {
                setDirectionTowardsAdjacentPoint(points, neighborId, id, width);
                if(points[neighborId].height <= height)
                {
                    queueArray[neighborId] = height;
                    *shouldRepeat = 1;
                }
                else{
                    queueArray[neighborId] = points[neighborId].height;
                }
            }
        }

        neighborId = id + 1;
        if(neighborId > -1 && neighborId < width*width) {
            if(points[neighborId].flag == 0) {
                setDirectionTowardsAdjacentPoint(points, neighborId, id, width);
                if(points[neighborId].height <= height)
                {
                    queueArray[neighborId] = height;
                    *shouldRepeat = 1;
                }
                else{
                    queueArray[neighborId] = points[neighborId].height;
                }
            }
        }

        for(int i = 0; i<3; i++){

            neighborId = id + width -1 + i;
            if(neighborId > -1 && neighborId < width*width) {
                if(points[neighborId].flag == 0) {
                    setDirectionTowardsAdjacentPoint(points, neighborId, id, width);
                    if(points[neighborId].height <= height)
                    {
                        queueArray[neighborId] = height;
                        *shouldRepeat = 1;
                    }
                    else{
                        queueArray[neighborId] = points[neighborId].height;
                    }
                }
            }
        }
    }
}

__kernel void setInboundDegree(__global Point* points, __global const int* pointsSize, __global int* inboundDegree) {

    int x = get_global_id(0);
    int y = get_global_id(1);
    int width = *pointsSize;

    int id = x*width + y;
    if(!isDirectedOutsideTerrainBoundary(points[id], width)) {
        atomic_inc(inboundDegree + getNextPointId(points[id], width));
    }
}

__kernel void initializePoints(__global  float* flags, __global const int* pointsSize, __global int* inboundDegree, __global int* pointIsVisited)

    int x = get_global_id(0);
    int y = get_global_id(1);
    int width = *pointsSize;

    int id = x*width + y;
    flags[id] = 0;
    points[id].water_value = 1;
    inboundDegree[id] = 0;
    pointIsVisited[id] = 0;
    if (id == 0) {
        points[id].flag =TOP_LEFT;
    } else if (id < width - 1) {
        points[id].flag =TOP;
    } else if (id == width- 1) {
        points[id].flag =TOP_RIGHT;
    } else if (id == width*(width - 1)) {
        points[id].flag =BOTTOM_LEFT;
    } else if (id > width*(width - 1) &&
                id <width*width - 1) {
        points[id].flag =BOTTOM;
    } else if (id == width*width - 1) {
        points[id].flag =BOTTOM_RIGHT;
    } else if (id%width== 0) {
        points[id].flag =LEFT;
    } else if (id%width == width - 1) {
        points[id].flag =RIGHT;
    } else {
    /*Not a boundary point; set a null direction and return*/
    points[id].flag = 0;
    }
}
