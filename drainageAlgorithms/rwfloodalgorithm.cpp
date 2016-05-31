#include "rwfloodalgorithm.h"
#include <queue>
#include "CL/cl.h"
#include <chrono>
#include <iostream>

using namespace std;
using namespace std::chrono;

extern "C"
{
    #include "openclutils.h"
}

RWFloodAlgorithm::RWFloodAlgorithm():
    DrainageAlgorithms()
{
    this->ter = 0;
    this->shader = 0;
    this->maxWaterCount = 0;
    this->waterThreshold = 100;

    QObject::connect(&conf, SIGNAL(changeAttr()), this, SLOT(changeAttr()));
}

RWFloodAlgorithm::~RWFloodAlgorithm()
{
    if (shader) {
        delete shader;
    }
}

void RWFloodAlgorithm::run(Terrain *ter)
{
    this->ter = ter;

    high_resolution_clock::time_point t1 = high_resolution_clock::now();
    this->flood(this->ter->struct_point);
    this->calculateWaterAccumulation(this->ter->struct_point);
    high_resolution_clock::time_point t2 = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>( t2 - t1 ).count();
    cout << "Elapsed time on rwflood: " <<  duration/1000 << " miliseg" << endl;

    drainageColors.clear();
    drainagePoints.clear();
    this->getDrainagePoints();
    this->shader->fillBuffers(this->drainagePoints, this->drainageColors);
}

void RWFloodAlgorithm::runParallel(Terrain *ter)
{
    this->ter = ter;
    high_resolution_clock::time_point t1 = high_resolution_clock::now();

    cl_int error = CL_SUCCESS;

    cl_uint platformIdCount = 0;
    error = clGetPlatformIDs(0, NULL, &platformIdCount);
    checkError(error, "Finding amount of platforms");

    //TODO: Be able to choose among all platforms, only fail if there is no platform
    if(platformIdCount != 1) {
        printf("Found %d platforms (only one platform was expected)\n", platformIdCount);
        exit(EXIT_FAILURE);
    }
    cl_platform_id platformIds[platformIdCount];
    error = clGetPlatformIDs (platformIdCount, platformIds, NULL);
    checkError(error, "Getting platforms");

    cl_uint deviceIdCount = 0;
    //TODO: Should select all devices
    error = clGetDeviceIDs (platformIds[0], CL_DEVICE_TYPE_CPU, 0, NULL, &deviceIdCount);
    checkError(error, "Finding devices");
    //TODO: Be able to choose among all devices, only fail if there is no platform
    if(deviceIdCount != 1) {
        printf("Found %d devices (only one device was expected)\n", deviceIdCount);
        exit(EXIT_FAILURE);
    }
    cl_device_id deviceIds[deviceIdCount];
    //TODO: Should use all devices
    error = clGetDeviceIDs (platformIds [0], CL_DEVICE_TYPE_CPU, deviceIdCount, deviceIds, NULL);
    checkError(error, "Getting devices");

    //TODO: Should use the selected devices, not the first one on the array
    cl_context context = clCreateContext(NULL, 1, deviceIds, NULL, NULL, &error);
    checkError(error, "Creating context");

    cl_command_queue commandQueue = clCreateCommandQueue(context, deviceIds[0], 0, &error);
    checkError(error, "Creating command queue");

    const char* kernelSource = loadKernel("rwflood.cl");
    cl_program rwFloodProgram = clCreateProgramWithSource(context, 1, &kernelSource, NULL, &error);
    checkError(error, "Creating program");
    free((char*)kernelSource);

    error = clBuildProgram(rwFloodProgram, 0, NULL,
                           "-D TOP=1 -D TOP_LEFT=2 -D TOP_RIGHT=3 -D BOTTOM=4 -D BOTTOM_LEFT=5 -D BOTTOM_RIGHT=6 -D RIGHT=7 -D LEFT=8", NULL, NULL);
    checkError(error, "Building program\n");

    const int maxElev = (int)(ter->max_bounding.z);
    const int minElev = (int)(ter->min_bounding.z);
    int width = ter->width;
    int height = ter->height;

    int queueArrayMemorySize = width*height*sizeof(float);
    int flagsMemorySize = width*height*sizeof(char);
    int pointIsVisitedMemorySize = width*height*sizeof(char);
    int coordszMemorySize = width*height*sizeof(float);
    int inboundDegreeMemorySize = width*height*sizeof(int);
    int waterValueMemorySize = width*height*sizeof(int);

    //TODO: Should try to change this, too much memory alloction
    float* queueArray = (float*)malloc(queueArrayMemorySize);
    char* flags = (char*)malloc(flagsMemorySize);
    char* pointIsVisited = (char*)malloc(pointIsVisitedMemorySize);
    float* coordsz = (float*)malloc(coordszMemorySize);
    int* inboundDegree = (int*)malloc(inboundDegreeMemorySize);
    int* waterValues = (int*)malloc(waterValueMemorySize);

    std::vector<runnel::Point*>& points = ter->struct_point;
    for(int i = 0; i < width*height; i++){
        coordsz[i] = points[i]->coord.z;
        queueArray[i] = -1.0;
    }

    for (int i = 0; i < width; i++) {

        queueArray[i] = coordsz[i];
        queueArray[width*(height-1) + i ] = coordsz[width*(height-1) + i];
    }

    for(int i = 0; i < height; i++) {
        queueArray[i*width] = coordsz[i*width];
        queueArray[width*(1+i)-1] = coordsz[width*(1+i)-1];
    }

    cl_mem d_coordsz = clCreateBuffer(context, CL_MEM_READ_ONLY, coordszMemorySize, NULL, &error);
    checkError(error, "Allocating memory in the device\n");
    cl_mem d_currentHeight = clCreateBuffer(context, CL_MEM_READ_ONLY, sizeof(int), NULL, &error);
    checkError(error, "Allocating memory in the device\n");
    cl_mem d_shouldRepeat = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(char), NULL, &error);
    checkError(error, "Allocating memory in the device\n");
    cl_mem d_queueArray = clCreateBuffer(context, CL_MEM_READ_WRITE, queueArrayMemorySize, NULL, &error);
    checkError(error, "Allocating memory in the device\n");
    cl_mem d_width = clCreateBuffer(context, CL_MEM_READ_ONLY, sizeof(int), NULL, &error);
    checkError(error, "Allocating memory in the device\n");
    cl_mem d_height = clCreateBuffer(context, CL_MEM_READ_ONLY, sizeof(int), NULL, &error);
    checkError(error, "Allocating memory in the device\n");
    cl_mem d_flags = clCreateBuffer(context, CL_MEM_READ_WRITE, flagsMemorySize, NULL, &error);
    checkError(error, "Allocating memory in the device\n");
    cl_mem d_inboundDegree= clCreateBuffer(context, CL_MEM_READ_WRITE, inboundDegreeMemorySize, NULL, &error);
    checkError(error, "Allocating memory in the device\n");
    cl_mem d_waterValues = clCreateBuffer(context, CL_MEM_READ_WRITE, waterValueMemorySize, NULL, &error);
    checkError(error, "Allocating memory in the device\n");
    cl_mem d_pointIsVisited = clCreateBuffer(context, CL_MEM_READ_WRITE, pointIsVisitedMemorySize, NULL, &error);
    checkError(error, "Allocating memory in the device\n");

    error = clEnqueueWriteBuffer(commandQueue, d_coordsz, CL_TRUE, 0, coordszMemorySize, coordsz, 0, 0, NULL);
    checkError(error, "Writing from cpu to device\n");
    error = clEnqueueWriteBuffer(commandQueue, d_queueArray, CL_TRUE, 0, queueArrayMemorySize, queueArray, 0, 0, NULL);
    checkError(error, "Writing from cpu to device\n");
    error = clEnqueueWriteBuffer(commandQueue, d_width, CL_TRUE, 0, sizeof(int), &width, 0, 0, NULL);
    checkError(error, "Writing from cpu to device\n");
    error = clEnqueueWriteBuffer(commandQueue, d_height, CL_TRUE, 0, sizeof(int), &height, 0, 0, NULL);
    checkError(error, "Writing from cpu to device\n");

    cl_kernel initializePointsKernel = clCreateKernel(rwFloodProgram, "initializePoints", &error);
    checkError(error, "Creating initializePoints kernel");
    error = clSetKernelArg(initializePointsKernel, 0, sizeof(cl_mem), (void*)&d_flags);
    checkError(error, "Setting the first argument of the initializePoints Kernel");
    error = clSetKernelArg(initializePointsKernel, 1, sizeof(cl_mem), (void*)&d_width);
    checkError(error, "Setting the second argument of the initializePoints Kernel");
    error = clSetKernelArg(initializePointsKernel, 2, sizeof(cl_mem), (void*)&d_height);
    checkError(error, "Setting the third argument of the initializePoints Kernel");
    error = clSetKernelArg(initializePointsKernel, 3, sizeof(cl_mem), (void*)&d_pointIsVisited);
    checkError(error, "Setting the fourth argument of the initializePoints Kernel");
    error = clSetKernelArg(initializePointsKernel, 4, sizeof(cl_mem), (void*)&d_inboundDegree);
    checkError(error, "Setting the fourth argument of the initializePoints Kernel");
    error = clSetKernelArg(initializePointsKernel, 5, sizeof(cl_mem), (void*)&d_waterValues);
    checkError(error, "Setting the fourth argument of the initializePoints Kernel");

    size_t localWorkSizeX;
    clGetKernelWorkGroupInfo(initializePointsKernel, deviceIds[0], CL_KERNEL_PREFERRED_WORK_GROUP_SIZE_MULTIPLE, sizeof(size_t), &localWorkSizeX, NULL);
    int globalWorkSizeX = height*width;
    globalWorkSizeX = globalWorkSizeX - (globalWorkSizeX%localWorkSizeX) + localWorkSizeX;
    const size_t globalWorkSize [] = { globalWorkSizeX, 0, 0 };
    const size_t localWorkSize [] = { localWorkSizeX, 0, 0 };

    error = clEnqueueNDRangeKernel(commandQueue, initializePointsKernel, 1, NULL, globalWorkSize, localWorkSize, 0, NULL,  NULL);
    checkError(error, "Running initializePointsKernel");

    cl_kernel iterateQueuesKernel = clCreateKernel(rwFloodProgram, "iterateQueues", &error);
    checkError(error, "Creating iterateQueues kernel");

    for (int  z = minElev; z <= maxElev; ++z)
    {
        error = clEnqueueWriteBuffer(commandQueue, d_currentHeight, CL_TRUE, 0, sizeof(int), &z, 0, 0, NULL);
        checkError(error, "Writing from cpu to device\n");

        char shouldRepeat = 1;
        while(shouldRepeat == 1) {
            shouldRepeat = 0;
            error = clEnqueueWriteBuffer(commandQueue, d_shouldRepeat, CL_TRUE, 0, sizeof(char), &shouldRepeat, 0, 0, NULL);
            checkError(error, "Writing from cpu to device\n");

            error = clSetKernelArg(iterateQueuesKernel, 0, sizeof(cl_mem), (void*)&d_flags);
            checkError(error, "Setting the first argument of the iterateQueues Kernel");
            error = clSetKernelArg(iterateQueuesKernel, 1, sizeof(cl_mem), (void*)&d_coordsz);
            checkError(error, "Setting the second argument of the iterateQueues Kernel");
            error = clSetKernelArg(iterateQueuesKernel, 2, sizeof(cl_mem), (void*)&d_width);
            checkError(error, "Setting the third argument of the iterateQueues Kernel");
            error = clSetKernelArg(iterateQueuesKernel, 3, sizeof(cl_mem), (void*)&d_height);
            checkError(error, "Setting the fourth argument of the iterateQueues Kernel");
            error = clSetKernelArg(iterateQueuesKernel, 4, sizeof(cl_mem), (void*)&d_queueArray);
            checkError(error, "Setting the fifth argument of the iterateQueues Kernel");
            error = clSetKernelArg(iterateQueuesKernel, 5, sizeof(cl_mem), (void*)&d_currentHeight);
            checkError(error, "Setting the sixth argument of the iterateQueues Kernel");
            error = clSetKernelArg(iterateQueuesKernel, 6, sizeof(cl_mem), (void*)&d_shouldRepeat);
            checkError(error, "Setting the seventh argument of the iterateQueues Kernel");

            error = clEnqueueNDRangeKernel(commandQueue, iterateQueuesKernel, 1, NULL, globalWorkSize, localWorkSize, 0, NULL,  NULL);
            checkError(error, "Running initializePointsKernel");

            clEnqueueReadBuffer(commandQueue, d_shouldRepeat, CL_TRUE, 0, sizeof(char), &shouldRepeat, 0, NULL, NULL);
        }
    }

    clReleaseMemObject(d_shouldRepeat);
    clReleaseMemObject(d_currentHeight);
    clReleaseMemObject(d_queueArray);

    int maxWaterCount = 1;

    cl_kernel setInboundDegreeKernel = clCreateKernel(rwFloodProgram, "setInboundDegree", &error);
    checkError(error, "Creating setInboundDegree kernel");
    error = clSetKernelArg(setInboundDegreeKernel, 0, sizeof(cl_mem), (void*)&d_flags);
    checkError(error, "Setting the first argument of the initializePoints Kernel");
    error = clSetKernelArg(setInboundDegreeKernel, 1, sizeof(cl_mem), (void*)&d_width);
    checkError(error, "Setting the second argument of the initializePoints Kernel");
    error = clSetKernelArg(setInboundDegreeKernel, 2, sizeof(cl_mem), (void*)&d_height);
    checkError(error, "Setting the third argument of the initializePoints Kernel");
    error = clSetKernelArg(setInboundDegreeKernel, 3, sizeof(cl_mem), (void*)&d_inboundDegree);
    checkError(error, "Setting the fourth argument of the initializePoints Kernel");

    error = clEnqueueNDRangeKernel(commandQueue, setInboundDegreeKernel, 1, NULL, globalWorkSize, localWorkSize, 0, NULL, NULL);
    checkError(error, "Running setInboundDegreeKernel");

    error = clEnqueueReadBuffer(commandQueue, d_inboundDegree, CL_TRUE, 0, inboundDegreeMemorySize, inboundDegree, 0, NULL, NULL);
    checkError(error, "Reading inboundDegree from device");
    error = clEnqueueReadBuffer(commandQueue, d_pointIsVisited, CL_TRUE, 0, pointIsVisitedMemorySize, pointIsVisited, 0, NULL, NULL);
    checkError(error, "Reading pointIsVisited from device");
    error = clEnqueueReadBuffer(commandQueue, d_waterValues, CL_TRUE, 0, waterValueMemorySize, waterValues, 0, NULL, NULL);
    checkError(error, "Reading waterValues from device");
    error = clEnqueueReadBuffer(commandQueue, d_flags, CL_TRUE, 0, flagsMemorySize, flags, 0, NULL, NULL);
    checkError(error, "Reading flags from device");

    clReleaseMemObject(d_coordsz);
    clReleaseMemObject(d_width);
    clReleaseMemObject(d_height);
    clReleaseMemObject(d_flags);
    clReleaseMemObject(d_inboundDegree);
    clReleaseMemObject(d_waterValues);
    clReleaseMemObject(d_pointIsVisited);

    for (int id = 0; id < width*height; ++id)
    {
        if (!pointIsVisited[id]) {
            int currentId = id;
            while (inboundDegree[currentId] == 0) {
                pointIsVisited[currentId] = 1;
                if (isDirectedOutsideTerrainBoundary(currentId, flags[currentId])) break;
                int nextPointId = getNextPointId(currentId, flags[currentId]);
                waterValues[nextPointId] += waterValues[currentId];
                if (waterValues[nextPointId] > maxWaterCount) {
                    maxWaterCount = waterValues[nextPointId];
                }
                inboundDegree[nextPointId]--;
                currentId = nextPointId;
            }
        }
    }

    this->maxWaterCount = maxWaterCount;

    for(int i = 0; i < width*height; i++){
        points[i]->water_value = waterValues[i];
    }

    free(inboundDegree);
    free(pointIsVisited);
    free(queueArray);
    free(flags);
    free(waterValues);

    high_resolution_clock::time_point t2 = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>( t2 - t1 ).count();
    cout << "Elapsed time on rwflood: " <<  duration/1000 << " miliseg" << endl;

    drainageColors.clear();
    drainagePoints.clear();
    this->getDrainagePoints();
    this->shader->fillBuffers(this->drainagePoints, this->drainageColors);

    }

void RWFloodAlgorithm::glewReady()
{
    this->shader = new ShaderRWFlood();
}

void RWFloodAlgorithm::render(glm::mat4 matrix, float exagValue, glm::vec3)
{
    if (shader) {
        shader->render(matrix, exagValue);
    }
}

QString RWFloodAlgorithm::getName()
{
    return QString("RWFlood");
}

QWidget* RWFloodAlgorithm::getConf()
{
    return &conf;
}

std::vector<glm::vec3> RWFloodAlgorithm::getPathTree()
{
    std::vector<glm::vec3> path;
    return path;
}

void RWFloodAlgorithm::flood(std::vector<runnel::Point*>& points)
{
    const int maxElev = (int)(ter->max_bounding.z);
    const int minElev = (int)(ter->min_bounding.z);
    const std::size_t arraySize = maxElev - minElev + 1;

    std::vector<std::queue<runnel::Point*>> queueArray(arraySize,
                                                       std::queue<runnel::Point*>());
    std::map<int, float> zValueStore;

    for (runnel::Point* point : points) {
        point->flags = 0;
        bool isBoundaryPoint = initializeDirection(points, point);
        if (isBoundaryPoint) {
            queueArray[point->coord.z - minElev].push(point);
        }
    }

    for (int z = minElev; z <= maxElev; ++z) {
        while (!queueArray[z-minElev].empty()) {
            runnel::Point* point = queueArray[z-minElev].front();
            queueArray[z-minElev].pop();

            std::vector<runnel::Point*> neighborhood = computeNeighborhood(point);
            for (runnel::Point* neighbor : neighborhood) {
                if (neighbor->flags==0) {
                    setDirectionTowardsAdjacentPoint(points, neighbor, point);
                    if (neighbor->coord.z < z) {
                        /* Warning: this modifies the data!
                         * The original values are saved for later restoration. */
                        zValueStore[neighbor->ident] = neighbor->coord.z;
                        neighbor->coord.z = z;
                    }
                    queueArray[neighbor->coord.z - minElev].push(neighbor);
                }
            }
        }
    }


    // Restore modified z-coordinate values to their original values.
    for (auto &entry : zValueStore) {
        points[entry.first]->coord.z = entry.second;
    }
}

void RWFloodAlgorithm::calculateWaterAccumulation(std::vector<runnel::Point*>& points)
{
    int maxWaterCount = 1;
    std::vector<int> inboundDegree(points.size());
    std::vector<bool> pointIsVisited(points.size());

    for (runnel::Point* point : points) {
        points[point->ident]->water_value = 1;
        inboundDegree[point->ident] = 0;
        pointIsVisited[point->ident] = false;
    }

    for (runnel::Point* point : points) {
        if (!isDirectedOutsideTerrainBoundary(point)) {
            inboundDegree[getNextPointId(point)]++;
        }
    }

    for (runnel::Point* point : points) {
        if (!pointIsVisited[point->ident]) {
            runnel::Point* currentPoint = point;
            while (inboundDegree[currentPoint->ident] == 0) {
                pointIsVisited[currentPoint->ident] = true;
                if (isDirectedOutsideTerrainBoundary(currentPoint)) break;
                int nextPointId = getNextPointId(currentPoint);
                int currentPointId = currentPoint->ident;
                points[nextPointId]->water_value += points[currentPointId]->water_value;
                if (points[nextPointId]->water_value > maxWaterCount) {
                    maxWaterCount = points[nextPointId]->water_value;
                }
                inboundDegree[nextPointId]--;
                currentPoint = points[nextPointId];
            }
        }
    }

    this->maxWaterCount = maxWaterCount;
}

void RWFloodAlgorithm::getDrainagePoints()
{
    for (runnel::Edge* edge : ter->struct_edge) {
        runnel::Point* p1 = ter->struct_point[edge->id1];
        runnel::Point* p2 = ter->struct_point[edge->id2];
        if (p1->water_value >= waterThreshold && p2->water_value >= waterThreshold) {
            float water1 = (float)p1->water_value / (float)maxWaterCount;
            float water2 = (float)p2->water_value / (float)maxWaterCount;
            this->drainagePoints.push_back(p1->coord);
            this->drainagePoints.push_back(p2->coord);
            this->drainageColors.push_back(glm::vec3(1.0f-water1, 1.0f-water1, 1.0f));
            this->drainageColors.push_back(glm::vec3(1.0f-water2, 1.0f-water2, 1.0f));
        }
    }
}

bool RWFloodAlgorithm::initializeDirection(std::vector<runnel::Point*>& points,
                                           runnel::Point* point)
{
    int id = point->ident;
    int width = ter->width;
    int height = ter->height;
    if (id == 0) {
        /*Top-left corner of raster*/
        points[id]->flags = TOP_LEFT;
    } else if (id < width - 1) {
        points[id]->flags = TOP;
    } else if (id == width - 1) {
        points[id]->flags = TOP_RIGHT;
    } else if (id == width*(height - 1)) {
        points[id]->flags = BOTTOM_LEFT;
    } else if (id > width*(height - 1) &&
               id < width*height - 1) {
        points[id]->flags = BOTTOM;
    } else if (id == width*height - 1) {
        points[id]->flags = BOTTOM_RIGHT;
    } else if (id%width == 0) {
        points[id]->flags = LEFT;
    } else if (id%width == width - 1) {
        points[id]->flags = RIGHT;
    } else {
        /*Not a boundary point; set a null direction and return*/
        points[id]->flags = 0;
        return false;
    }
    return true;
}

std::vector<runnel::Point*> RWFloodAlgorithm::computeNeighborhood(runnel::Point* point)
{
    std::vector<runnel::Point*> neighborhood;
    int id = point->ident;
    int width = ter->width;
    int height = ter->height;

    if (id == 0) {
        neighborhood.push_back(ter->struct_point[id + 1]);
        neighborhood.push_back(ter->struct_point[id + width]);
        neighborhood.push_back(ter->struct_point[id + width + 1]);
    } else if (id < width - 1) {
        neighborhood.push_back(ter->struct_point[id - 1]);
        neighborhood.push_back(ter->struct_point[id + 1]);
        neighborhood.push_back(ter->struct_point[id + width - 1]);
        neighborhood.push_back(ter->struct_point[id + width]);
        neighborhood.push_back(ter->struct_point[id + width + 1]);
    } else if (id == width - 1) {
        neighborhood.push_back(ter->struct_point[id - 1]);
        neighborhood.push_back(ter->struct_point[id + width - 1]);
        neighborhood.push_back(ter->struct_point[id + width]);
    } else if (id == width*(height - 1)) {
        neighborhood.push_back(ter->struct_point[id - width]);
        neighborhood.push_back(ter->struct_point[id - width + 1]);
        neighborhood.push_back(ter->struct_point[id + 1]);
    } else if (id > width*(height - 1) && id < width*height - 1) {
        neighborhood.push_back(ter->struct_point[id - width - 1]);
        neighborhood.push_back(ter->struct_point[id - width]);
        neighborhood.push_back(ter->struct_point[id - width + 1]);
        neighborhood.push_back(ter->struct_point[id - 1]);
        neighborhood.push_back(ter->struct_point[id + 1]);
    } else if (id == width*height - 1) {
        neighborhood.push_back(ter->struct_point[id - width - 1]);
        neighborhood.push_back(ter->struct_point[id - width]);
        neighborhood.push_back(ter->struct_point[id - 1]);
    } else if (id%width == 0) {
        neighborhood.push_back(ter->struct_point[id - width]);
        neighborhood.push_back(ter->struct_point[id - width + 1]);
        neighborhood.push_back(ter->struct_point[id + 1]);
        neighborhood.push_back(ter->struct_point[id + width]);
        neighborhood.push_back(ter->struct_point[id + width + 1]);
    } else if (id%width == width - 1) {
        neighborhood.push_back(ter->struct_point[id - width - 1]);
        neighborhood.push_back(ter->struct_point[id - width]);
        neighborhood.push_back(ter->struct_point[id - 1]);
        neighborhood.push_back(ter->struct_point[id + width - 1]);
        neighborhood.push_back(ter->struct_point[id + width]);
    } else {
        neighborhood.push_back(ter->struct_point[id - width - 1]);
        neighborhood.push_back(ter->struct_point[id - width]);
        neighborhood.push_back(ter->struct_point[id - width + 1]);
        neighborhood.push_back(ter->struct_point[id - 1]);
        neighborhood.push_back(ter->struct_point[id + 1]);
        neighborhood.push_back(ter->struct_point[id + width - 1]);
        neighborhood.push_back(ter->struct_point[id + width]);
        neighborhood.push_back(ter->struct_point[id + width + 1]);
    }
    neighborhood.shrink_to_fit();
    return neighborhood;
}

void RWFloodAlgorithm::setDirectionTowardsAdjacentPoint(std::vector<runnel::Point*>& points, runnel::Point* source, runnel::Point* destination)
{
    int sourceId = source->ident;
    int diff = destination->ident - source->ident;
    int width = ter->width;

    if (diff == -width - 1) {
        points[sourceId]->flags = TOP_LEFT;
    } else if (diff == -width) {
        points[sourceId]->flags = TOP;
    } else if (diff == -width + 1) {
        points[sourceId]->flags = TOP_RIGHT;
    } else if (diff == -1) {
        points[sourceId]->flags = LEFT;
    } else if (diff == 1) {
        points[sourceId]->flags = RIGHT;
    } else if (diff == width -1) {
        points[sourceId]->flags = BOTTOM_LEFT;
    } else if (diff == width) {
        points[sourceId]->flags = BOTTOM;
    } else if (diff == width + 1) {
        points[sourceId]->flags = BOTTOM_RIGHT;
    }
}


bool RWFloodAlgorithm::isDirectedOutsideTerrainBoundary(int id, char flag)
{
    // Pre-condition: point->flags != 0 (a direction must be set.)
    int height = ter->height;
    int width = ter->width;

    if (id < width &&
        (flag == TOP ||
         flag == TOP_LEFT ||
         flag == TOP_RIGHT)) {
        return true;
    }

    if (id%width == 0 &&
        (flag == LEFT ||
         flag == TOP_LEFT ||
         flag == BOTTOM_LEFT)) {
        return true;
    }

    if (id%width == width-1 &&
        (flag == RIGHT ||
         flag == TOP_RIGHT ||
         flag == BOTTOM_RIGHT)) {
        return true;
    }

    if (id >= width*(height - 1) &&
        (flag == BOTTOM ||
         flag == BOTTOM_LEFT ||
         flag == BOTTOM_RIGHT)) {
        return true;
    }

    return false;
}


bool RWFloodAlgorithm::isDirectedOutsideTerrainBoundary(runnel::Point* point)
{
    // Pre-condition: point->flags != 0 (a direction must be set.)
    int id = point->ident;
    int height = ter->height;
    int width = ter->width;

    if (id < width &&
        (point->flags == TOP ||
         point->flags == TOP_LEFT ||
         point->flags == TOP_RIGHT )) {
        return true;
    }

    if (id%width == 0 &&
        (point->flags == LEFT ||
         point->flags == TOP_LEFT ||
         point->flags == BOTTOM_LEFT)) {
        return true;
    }

    if (id%width == width-1 &&
        (point->flags == RIGHT ||
         point->flags == TOP_RIGHT ||
         point->flags == BOTTOM_RIGHT)) {
        return true;
    }

    if (id >= width*(height - 1) &&
        (point->flags == BOTTOM ||
         point->flags == BOTTOM_LEFT ||
         point->flags == BOTTOM_RIGHT)){
        return true;
    }

    return false;
}

int RWFloodAlgorithm::getNextPointId(runnel::Point* point)
{
    // Pre-condition: !isDirectedOutsideTerrainBoundary(point) == true
    int id = point->ident;
    int width = ter->width;

    if (point->flags == TOP_LEFT) return (id - width - 1);
    if (point->flags == TOP) return (id - width);
    if (point->flags == TOP_RIGHT) return (id - width + 1);
    if (point->flags == LEFT) return (id - 1);
    if (point->flags == RIGHT) return (id + 1);
    if (point->flags == BOTTOM_LEFT) return (id + width - 1);
    if (point->flags == BOTTOM) return (id + width);
    if (point->flags == BOTTOM_RIGHT) return (id + width + 1);

    return -1;
}


int RWFloodAlgorithm::getNextPointId(int id, char flag)
{
    // Pre-condition: !isDirectedOutsideTerrainBoundary(point) == true
    int width = ter->width;

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

void RWFloodAlgorithm::changeAttr()
{
    this->waterThreshold = conf.getWaterThreshold();
    this->run(ter);
    emit reload();
}
