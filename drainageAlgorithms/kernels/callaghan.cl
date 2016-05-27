int getTopLeftNeighbourId(int id, int width, int height);
int getTopNeighbourId(int id, int width, int height);
int getTopRightNeighbourId(int id, int width, int height);
int getLeftNeighbourId(int id, int width, int height);
int getRightNeighbourId(int id, int width, int height);
int getBottomLeftNeighbourId(int id, int width, int height);
int getBottomRightNeighbourId(int id, int width, int height);
int getBottomNeighbourId(int id, int width, int height);

int getTopLeftNeighbourId(int id, int width, int height){
        if (id >= width && id%width > 0)
        {
                return id-width-1;
        }
        else {
                return -1;
        }
}
int getTopNeighbourId(int id, int width, int height){
        if (id >= width)
        {
                return id-width;
        }
        else{
                return -1;
        }
}
int getTopRightNeighbourId(int id, int width, int height){
        if (id >= width && id%width < (width-1))
        {
                return id-width+1;
        }
        else {
                return -1;
        }
}
int getLeftNeighbourId(int id, int width, int height){
        if (id%width > 0)
        {
                return id-1;
        }
        else {
                return -1;
        }
}
int getRightNeighbourId(int id, int width, int height){
        if (id%width < (width-1))
        {
                return id+1;
        }
        else {
                return -1;
        }
}
int getBottomLeftNeighbourId(int id, int width, int height){
        if (id < width*(height - 1) && id%width > 0)
        {
                return id+width-1;
        }
        else {
                return -1;
        }
}
int getBottomRightNeighbourId(int id, int width, int height){
         if (id < width*(height - 1) && id%width < (width-1))
        {
                return id+width+1;
        }
        else {
                return -1;
        }
}
int getBottomNeighbourId(int id, int width, int height) {
        if (id < width*(height - 1))
        {
                return id+width;
        }
        else{
                return -1;
        }
}

__kernel void setWaterPath(__global int* maxNeighbors, __global int* waterValues, __global const int*  pwidth, __global const int*  pheight){

        int id = get_global_id(0);
        int width = *pwidth;
        int height = *pheight;

        if(id < width*height) {
                while(id != -1) {
                        atomic_add(&(waterValues[id]), 1);
                        id = maxNeighbors[id];
                }
        }
}

__kernel void setMaxNeighbour(__global float* coordsz, __global int* maxNeighbours, __global const int* pwidth, __global const int* pheight, __global const float* pdeltaWater) {

        int id = get_global_id(0);
        int width = *pwidth;
        int height = *pheight;
        float deltaWater = *pdeltaWater;

        if(id < width*height) {
                int neighbourPosition[8];

                neighbourPosition[0] = getTopNeighbourId(id, width, height);
                neighbourPosition[1] = getBottomNeighbourId(id, width, height);
                neighbourPosition[2] = getBottomLeftNeighbourId(id, width, height);
                neighbourPosition[3] = getTopLeftNeighbourId(id, width, height);
                neighbourPosition[4] = getLeftNeighbourId(id, width, height);
                neighbourPosition[5] = getRightNeighbourId(id, width, height);
                neighbourPosition[6] = getTopRightNeighbourId(id, width, height);
                neighbourPosition[7] = getBottomRightNeighbourId(id, width, height);

                float maxHeightDifference = -1;
                int idMaxNeighbour = -1;

                for (int i = 0; i < 8; ++i)
                {
                        if (neighbourPosition[i]>=0)
                        {
                                float currentPointHeight = coordsz[id];
                                float neighbourPointHeight = coordsz[neighbourPosition[i]];
                                if (neighbourPointHeight < currentPointHeight + deltaWater) {
                                        float heightDifference = currentPointHeight - neighbourPointHeight;
                                        if (maxHeightDifference < heightDifference)
                                        {
                                                maxHeightDifference=heightDifference;
                                                idMaxNeighbour = neighbourPosition[i];
                                        }
                                }
                        }
                }
                maxNeighbours[id] = idMaxNeighbour;
        }
}
