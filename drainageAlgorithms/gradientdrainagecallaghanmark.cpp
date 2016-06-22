#include "gradientdrainagecallaghanmark.h"
#include <iostream>
#include <algorithm>
#include "primitives/point.h"
#include "CL/cl.h"
#include <chrono>
#include <iostream>

using namespace std;
using namespace std::chrono;

extern "C"
{
    #include "openclutils.h"
}

struct customLess{
    inline bool operator()(runnel::Point* p1, runnel::Point* p2){
        return p1->coord.z > p2->coord.z;
    }
};


GradientDrainageCallaghanMark::GradientDrainageCallaghanMark():
    DrainageAlgorithms()
{
    w = 0;
    h = 0;
    shader_callaghan = 0;
    max_water = 0;
    QObject::connect(&conf, SIGNAL(changeAttr()), this, SLOT(changeAttr()));
}

GradientDrainageCallaghanMark::~GradientDrainageCallaghanMark(){
    if(shader_callaghan){
        delete shader_callaghan;
    }
}

QString GradientDrainageCallaghanMark::getName(){
    return QString("Callaghan");
}

void GradientDrainageCallaghanMark::sortElement(std::vector<runnel::Point *> points){
    points_terrain =  points;
    std::sort( points_terrain.begin(), points_terrain.end(), customLess());
}


void GradientDrainageCallaghanMark::run(Terrain* ter){
    this->ter = ter;

    high_resolution_clock::time_point t1 = high_resolution_clock::now();
    count_water.clear();
    position_water_points.clear();
    w = this->ter->width;
    h = this->ter->height;
    max_value_water = 0;

    for(runnel::Point* pto: this->ter->struct_point){
        pto->water_value = 1;
        pto->water_parent.clear();
    }

    GradientDrainageCallaghanMark::sortElement(ter->struct_point);

    for(runnel::Point* pto : points_terrain){
        this->chooseMoreDepthPoint(ter->struct_point, pto);
    }
    high_resolution_clock::time_point t2 = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>( t2 - t1 ).count();
    cout << "Elapsed time Sequential Callaghan: " <<  duration/1000 << " miliseg" << endl;

    this->getMoreWaterPoint();
    max_water = max_value_water;
    shader_callaghan->fillPositionBuffer(position_water_points, count_water);

}


void GradientDrainageCallaghanMark::runParallel(Terrain* ter){
    this->ter = ter;

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

    const char* kernelSource = loadKernel("callaghan.cl");
    cl_program callaghanProgram = clCreateProgramWithSource(context, 1, &kernelSource, NULL, &error);
    checkError(error, "Creating program");
    free((char*)kernelSource);

    count_water.clear();
    position_water_points.clear();
    w = ter->width;
    h = ter->height;
    max_value_water = 0;

    for(runnel::Point* pto: this->ter->struct_point){
        pto->water_parent.clear();
    }

    moreWater = conf.getWater()/100.0f;
    float deltaWater = moreWater;

    int coordszMemorySize = (ter->width)*(ter->height)*sizeof(float);
    int maxNeighboursMemorySize = (ter->width)*(ter->height)*sizeof(int);
    int waterValuesMemorySize = (ter->width)*(ter->height)*sizeof(int);

    float* coordsz = ter->pointsCoordZ.data();
    int* waterValues = (int*)malloc(waterValuesMemorySize);
    int* maxNeighbours = (int*)malloc(waterValuesMemorySize);

    high_resolution_clock::time_point t1 = high_resolution_clock::now();
    std::vector<runnel::Point*>& points = ter->struct_point;
    for(int i = 0; i < (ter->width)*(ter->height); i++){
        waterValues[i] = 0;
    }

    cl_mem d_coordsz = clCreateBuffer(context, CL_MEM_READ_WRITE, coordszMemorySize, NULL, &error);
    checkError(error, "Allocating memory in the device\n");

    cl_mem d_maxNeighbours = clCreateBuffer(context, CL_MEM_READ_WRITE, maxNeighboursMemorySize, NULL, &error);
    checkError(error, "Allocating memory in the device\n");

    cl_mem d_waterValues = clCreateBuffer(context, CL_MEM_READ_WRITE, waterValuesMemorySize, NULL, &error);
    checkError(error, "Allocating memory in the device\n");

    cl_mem d_width = clCreateBuffer(context, CL_MEM_READ_ONLY, sizeof(int), NULL, &error);
    checkError(error, "Allocating memory in the device\n");
    cl_mem d_height = clCreateBuffer(context, CL_MEM_READ_ONLY, sizeof(int), NULL, &error);
    checkError(error, "Allocating memory in the device\n");
    cl_mem d_deltaWater = clCreateBuffer(context, CL_MEM_READ_ONLY, sizeof(float), NULL, &error);
    checkError(error, "Allocating memory in the device\n");

    error = clEnqueueWriteBuffer(commandQueue, d_coordsz, CL_TRUE, 0, coordszMemorySize, coordsz, 0, 0, NULL);
    checkError(error, "Writing from cpu to device\n");
    error = clEnqueueWriteBuffer(commandQueue, d_waterValues, CL_TRUE, 0, waterValuesMemorySize, waterValues, 0, 0, NULL);
    checkError(error, "Writing from cpu to device\n");
    error = clEnqueueWriteBuffer(commandQueue, d_width, CL_TRUE, 0, sizeof(int), &(ter->width), 0, 0, NULL);
    checkError(error, "Writing from cpu to device\n");
    error = clEnqueueWriteBuffer(commandQueue, d_height, CL_TRUE, 0, sizeof(int), &(ter->height), 0, 0, NULL);
    checkError(error, "Writing from cpu to device\n");
    error = clEnqueueWriteBuffer(commandQueue, d_deltaWater, CL_TRUE, 0, sizeof(float), &deltaWater, 0, 0, NULL);
    checkError(error, "Writing from cpu to device\n");

    error = clBuildProgram(callaghanProgram, 0, NULL, "-Werror", NULL, NULL);
    checkError(error, "Building program\n");

    cl_kernel setMaxNeighbourKernel = clCreateKernel(callaghanProgram, "setMaxNeighbour", &error);
    checkError(error, "Creating setMaxNeighbour Kernel");
    error = clSetKernelArg(setMaxNeighbourKernel, 0, sizeof(cl_mem), (void*)&d_coordsz);
    checkError(error, "Setting first argument of the setMaxNeighour kernel");
    error = clSetKernelArg(setMaxNeighbourKernel, 1, sizeof(cl_mem), (void*)&d_maxNeighbours);
    checkError(error, "Setting first argument of the setMaxNeighour kernel");
    error = clSetKernelArg(setMaxNeighbourKernel, 2, sizeof(cl_mem), (void*)&d_width);
    checkError(error, "Setting third argument of the setMaxNeighour kernel");
    error = clSetKernelArg(setMaxNeighbourKernel, 3, sizeof(cl_mem), (void*)&d_height);
    checkError(error, "Setting fourth argument of the setMaxNeighour kernel");
    error = clSetKernelArg(setMaxNeighbourKernel, 4, sizeof(cl_mem), (void*)&d_deltaWater);
    checkError(error, "Setting fifth argument of the setMaxNeighbour kernel");

    cl_kernel setWaterPathKernel = clCreateKernel(callaghanProgram, "setWaterPath", &error);
    checkError(error, "Creating setWaterPath Kernel");
    error = clSetKernelArg(setWaterPathKernel, 0, sizeof(cl_mem), (void*)&d_maxNeighbours);
    checkError(error, "Setting first argument of the setWaterPath kernel");
    error = clSetKernelArg(setWaterPathKernel, 1, sizeof(cl_mem), (void*)&d_waterValues);
    checkError(error, "Setting first argument of the setWaterPath kernel");
    error = clSetKernelArg(setWaterPathKernel, 2, sizeof(cl_mem), (void*)&d_width);
    checkError(error, "Setting third argument of the setWaterPath kernel");
    error = clSetKernelArg(setWaterPathKernel, 3, sizeof(cl_mem), (void*)&d_height);
    checkError(error, "Setting fourth argument of the setWaterPath kernel");

    size_t localWorkSizeX;
    clGetKernelWorkGroupInfo(setWaterPathKernel, deviceIds[0], CL_KERNEL_PREFERRED_WORK_GROUP_SIZE_MULTIPLE, sizeof(size_t), &localWorkSizeX, NULL);
    int globalWorkSizeX = ter->height*ter->width;
    globalWorkSizeX = globalWorkSizeX - (globalWorkSizeX%localWorkSizeX) + localWorkSizeX;
    const size_t globalWorkSize [] = { globalWorkSizeX, 0, 0 };
    const size_t localWorkSize [] = { localWorkSizeX, 0, 0 };

    error = clEnqueueNDRangeKernel(commandQueue, setMaxNeighbourKernel, 1, NULL, globalWorkSize, localWorkSize, 0, NULL,  NULL);
    checkError(error, "Running setMaxNeighbour kernel\n");

    error = clEnqueueReadBuffer(commandQueue, d_maxNeighbours, CL_TRUE, 0, maxNeighboursMemorySize, maxNeighbours, 0, NULL, NULL);
    checkError(error, "Reading from device to cpu");

    error = clEnqueueNDRangeKernel(commandQueue, setWaterPathKernel, 1, NULL, globalWorkSize, localWorkSize, 0, NULL,  NULL);
    checkError(error, "Running setWaterPath kernel\n");

    error = clEnqueueReadBuffer(commandQueue, d_waterValues, CL_TRUE, 0, waterValuesMemorySize, waterValues, 0, NULL, NULL);
    checkError(error, "Reading from device to cpu");

    for(int i = 0; i < points.size(); i++) {
        if(maxNeighbours[i] != -1) points[maxNeighbours[i]]->water_parent.push_back(points[i]);
        points[i]->water_value = waterValues[i];
    }
    free(coordsz);
    free(waterValues);
    free(maxNeighbours);

    error = clReleaseMemObject(d_coordsz);
    checkError(error, "Releasing memory from device");
    error = clReleaseMemObject(d_waterValues);
    checkError(error, "Releasing memory from device");
    error = clReleaseMemObject(d_maxNeighbours);
    checkError(error, "Releasing memory from device");
    error = clReleaseMemObject(d_width);
    checkError(error, "Releasing memory from device");
    error = clReleaseMemObject(d_height);
    checkError(error, "Releasing memory from device");
    error = clReleaseMemObject(d_deltaWater);
    checkError(error, "Releasing memory from device");

    high_resolution_clock::time_point t2 = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>( t2 - t1 ).count();
    cout << "Elapsed time on Parallel Callaghan: " <<  duration/1000 << " miliseg" << endl;

    this->getMoreWaterPoint();
    max_water = max_value_water;
    shader_callaghan->fillPositionBuffer(position_water_points, count_water);

}

void GradientDrainageCallaghanMark::chooseMoreDepthPoint(std::vector<runnel::Point*>& points, runnel::Point *pto){
    moreWater = conf.getWater()/100.0f;
    float delta_water = moreWater;
    std::vector<int> position_neightbour;
    position_neightbour.push_back(pto->ident - this->w);
    position_neightbour.push_back(pto->ident + this->w);
    if(pto->ident%w > 0){
        position_neightbour.push_back(pto->ident + this->w - 1);
        position_neightbour.push_back(pto->ident - this->w - 1);
        position_neightbour.push_back(pto->ident - 1);
    }
    if(pto->ident%w < w-1){
        position_neightbour.push_back(pto->ident + 1);
        position_neightbour.push_back(pto->ident - this->w + 1);
        position_neightbour.push_back(pto->ident + this->w + 1);
    }
    float max_z = -1;
    int id_max = -1;
    for (int id : position_neightbour){
        if(id >= 0 && id < points.size()){
            if( points[id]->coord.z < (pto->coord.z +delta_water)){
                float dist_z = pto->coord.z - points[id]->coord.z;
                if( max_z < dist_z){
                    max_z = dist_z;
                    id_max = id;
                }
            }
        }
    }
    if(!(max_z == -1)){
        points[id_max]->water_parent.push_back(points[pto->ident]);
        points[id_max]->water_value = points[id_max]->water_value + points[pto->ident]->water_value ;
    }

}


void GradientDrainageCallaghanMark::glewReady(){
    shader_callaghan = new ShaderCallaghan();
}

void GradientDrainageCallaghanMark::render(glm::mat4 matrix, float exag_z, glm::vec3 color){
    linewater = conf.getLineWater()/1000.0f;
    if (shader_callaghan){
        shader_callaghan->render(matrix, exag_z, linewater , max_water, color);
    }
}

QWidget* GradientDrainageCallaghanMark::getConf(){

    return &conf;
}

void GradientDrainageCallaghanMark::changeAttr(){
    this->run(ter);
    emit reload();
}

std::vector<glm::vec3> GradientDrainageCallaghanMark::getPathTree(){
    std::vector<glm::vec3> points_path;
    std::vector<glm::vec3> position_water = position_water_points;
    int i = 0;
    while(i< position_water.size()){
        if (count_water[i]/max_water > linewater){
            points_path.push_back(position_water[i]);
        }
        ++i;
    }
    return points_path;
}


void GradientDrainageCallaghanMark::getMoreWaterPoint(){

    for (runnel::Edge* edge: this->ter->struct_edge){
        float water_1 = edge->point1->water_value;
        float water_2 = edge->point2->water_value;
        float value_water =  std::max(water_1, water_2);
        max_value_water = std::max(max_value_water,value_water);
        count_water.push_back(value_water);
        count_water.push_back(value_water);
        position_water_points.push_back(edge->point1->coord);
        position_water_points.push_back(edge->point2->coord);
    }
}
