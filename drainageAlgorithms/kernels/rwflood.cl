int isDirectedOutsideTerrainBoundary(int id, char flag, int width, int height)
{

    if (id < width &&
        (flag == TOP ||
         flag == TOP_LEFT ||
         flag == TOP_RIGHT)) {
        return 1;
    }

    if (id%width == 0 &&
        (flag == LEFT ||
         flag == TOP_LEFT ||
         flag == BOTTOM_LEFT)) {
        return 1;
    }

    if (id%width == width-1 &&
        (flag == RIGHT ||
         flag == TOP_RIGHT ||
         flag == BOTTOM_RIGHT)) {
        return 1;
    }

    if (id >= width*(height - 1) &&
        (flag == BOTTOM ||
         flag == BOTTOM_LEFT ||
         flag == BOTTOM_RIGHT)) {
        return 1;
    }

    return 0;
}

void setDirectionTowardsAdjacentPoint(__global char* flags, int sourceId, int destinationId, int width)
{
    int diff = destinationId - sourceId;

    if (diff == -width - 1) {
        flags[sourceId] = TOP_LEFT;
    } else if (diff == -width) {
        flags[sourceId] = TOP;
    } else if (diff == -width + 1) {
        flags[sourceId] = TOP_RIGHT;
    } else if (diff == -1) {
        flags[sourceId] = LEFT;
    } else if (diff == 1) {
        flags[sourceId] = RIGHT;
    } else if (diff == width -1) {
        flags[sourceId] = BOTTOM_LEFT;
    } else if (diff == width) {
        flags[sourceId] = BOTTOM;
    } else if (diff == width + 1) {
        flags[sourceId] = BOTTOM_RIGHT;
    }
}

int getNextPointId(char flag, int id, int width)
{
    // Pre-condition: !isDirectedOutsideTerrainBoundary(point) == true

    if (flag == TOP_LEFT) return (id - width - 1);
    if (flag == TOP) return (id - width);
    if (flag == TOP_RIGHT) return (id - width + 1);
    if (flag == LEFT) return (id - 1);
    if (flag == RIGHT) return (id + 1);
    if (flag == BOTTOM_LEFT) return (id + width - 1);
    if (flag == BOTTOM) return (id + width);
    if (flag == BOTTOM_RIGHT) return (id + width + 1);

    return -1;
}

__kernel void iterateQueues(__global char* flags, __global float* coordsz, __global int* pwidth, __global int* pheight, __global float* queueArray, __global int* pcurrentHeight, __global char* pshouldRepeat) {

    int id = get_global_id(0);
    int width = *pwidth;
    int height = *pheight;
    int currentHeight = *pcurrentHeight;

    if(id < width*height) {
        if((int)queueArray[id] == currentHeight) {

            queueArray[id] = -1.0;
            int neighborId;

            for(int i = 0; i<3; i++){

                neighborId = id - width -1 + i;

                if(neighborId > -1 && neighborId < width*height) {
                    if(flags[neighborId] == 0) {
                        setDirectionTowardsAdjacentPoint(flags, neighborId, id, width);
                        if(coordsz[neighborId] <= currentHeight)
                        {
                            queueArray[neighborId] = currentHeight*1.0;
                            *pshouldRepeat = 1;
                        }
                        else{
                            queueArray[neighborId] = coordsz[neighborId];
                        }
                    }
                }
            }

            neighborId = id - 1;
            if(neighborId > -1 && neighborId < width*height) {
                if(flags[neighborId] == 0) {
                    setDirectionTowardsAdjacentPoint(flags, neighborId, id, width);
                    if(coordsz[neighborId] <= currentHeight)
                    {
                        queueArray[neighborId] = currentHeight*1.0;
                        *pshouldRepeat = 1;
                    }
                    else{
                        queueArray[neighborId] = coordsz[neighborId];
                    }
                }
            }

            neighborId = id + 1;
            if(neighborId > -1 && neighborId < width*height) {
                if(flags[neighborId] == 0) {
                    setDirectionTowardsAdjacentPoint(flags, neighborId, id, width);
                    if(coordsz[neighborId] <= currentHeight)
                    {
                        queueArray[neighborId] = currentHeight*1.0;
                        *pshouldRepeat = 1;
                    }
                    else{
                        queueArray[neighborId] = coordsz[neighborId];
                    }
                }
            }

            for(int i = 0; i<3; i++){

                neighborId = id + width -1 + i;
                if(neighborId > -1 && neighborId < width*height) {
                    if(flags[neighborId] == 0) {
                        setDirectionTowardsAdjacentPoint(flags, neighborId, id, width);
                        if(coordsz[neighborId] <= currentHeight)
                        {
                            queueArray[neighborId] = currentHeight*1.0;
                            *pshouldRepeat = 1;
                        }
                        else{
                            queueArray[neighborId] = coordsz[neighborId];
                        }
                    }
                }
            }

        }
    }
}

__kernel void setInboundDegree(__global char* flags, __global const int* pwidth, __global const int* pheight, __global int* inboundDegree) {

    int id = get_global_id(0);
    int width = *pwidth;
    int height = *pheight;

    if(id < width*height) {
        if(!isDirectedOutsideTerrainBoundary(id, flags[id], width, height)) {
            atomic_inc(inboundDegree + getNextPointId(flags[id], id, width));
        }
    }
}

__kernel void initializePoints(__global  char* flags, __global int* pwidth, __global int* pheight, __global char* pointIsVisited, __global int* inboundDegree, __global int* waterValues){

    int id = get_global_id(0);
    int width = *pwidth;
    int height = *pheight;

    if(id < width*height) {
        flags[id] = 0;
        waterValues[id] = 1;
        inboundDegree[id] = 0;
        pointIsVisited[id] = 0;
        if (id == 0) {
            flags[id] = TOP_LEFT;
        } else if (id < width - 1) {
            flags[id] = TOP;
        } else if (id == width- 1) {
            flags[id] = TOP_RIGHT;
        } else if (id == width*(height - 1)) {
            flags[id] = BOTTOM_LEFT;
        } else if (id > width*(height - 1) &&
                    id <width*height - 1) {
            flags[id] = BOTTOM;
        } else if (id == width*height - 1) {
            flags[id] = BOTTOM_RIGHT;
        } else if (id%width== 0) {
            flags[id] = LEFT;
        } else if (id%width == width - 1) {
            flags[id] = RIGHT;
        } else {
            flags[id] = 0;
        }
    }
}
