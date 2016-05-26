__kernel void markHighestPoint(__global  float* coordsz, __global char* flags, __global const int*  pwidth, __global const int*  pheight) {
    int x = get_global_id(0);

    int width = *pwidth;
    int height = *pheight;

    if(x < width*height) {

        if((x+1)%width == 0) return;
        if(x >= width*(height-1)) return;

        //window to process
        float square[4];
        square[0] = coordsz[x];
        square[1] = coordsz[x + 1];
        square[2] = coordsz[x + width];
        square[3] = coordsz[x + width + 1];

        //Obtain the highest point in the current window
        float maxHeight = square[0];

        if(square[1] > maxHeight) {
                maxHeight = square[1];
        }


        if(square[2] > maxHeight) {
                maxHeight = square[2];
        }


        if(square[3] > maxHeight) {
                maxHeight = square[3];
        }


        if(square[0] == maxHeight) {
                flags[x]=1;
        }

        if(square[1] == maxHeight) {
                flags[x+1]=1;
        }

        if(square[2] == maxHeight) {
                flags[x+width]=1;
        }

        if(square[3] == maxHeight) {
                flags[x+width+1]=1;
        }
    }

}

__kernel void setPeuckerFlag(__global char* flags, __global const int*  pwidth, __global const int*  pheight) {
    int x = get_global_id(0);

    int width = *pwidth;
    int height = *pheight;

    if(x < width*height) {
        flags[x] = 0;
    }
}
