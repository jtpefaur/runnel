#include "peuckerdrainagenetwork.h"
#include "terrain.h"
#include "primitives/point.h"
#include <cmath>
#include "CL/cl.h"
#include <chrono>
#include <iostream>

using namespace std;
using namespace std::chrono;

extern "C"
{
    #include "openclutils.h"
}

PeuckerDrainageNetwork::PeuckerDrainageNetwork():
    DrainageAlgorithms()
{
    shader = 0;
    terr = 0;

}

PeuckerDrainageNetwork::~PeuckerDrainageNetwork(){
    if(shader){
        delete shader;
    }
}

void PeuckerDrainageNetwork::run(Terrain *ter){
    terr = ter;
    high_resolution_clock::time_point t1 = high_resolution_clock::now();
    this->calculateGrid(ter);
    high_resolution_clock::time_point t2 = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>( t2 - t1 ).count();
    cout << "Elapsed time on Sequential Peucker: " <<  duration/1000 << " miliseg" << endl;
    points_edge.clear();
    std::vector<glm::vec3> color = this->getDrainageColor();
    shader->fillPositionBuffer(points_edge, color);
}

void PeuckerDrainageNetwork::runParallel(Terrain *ter){
    terr = ter;
    this->calculateGridParallel(ter);
    points_edge.clear();
    std::vector<glm::vec3> color = this->getDrainageColor();
    shader->fillPositionBuffer(points_edge, color);
}

void PeuckerDrainageNetwork::render(glm::mat4 matrix, float exag_z, glm::vec3 color){
    if(shader){
        shader->render(matrix, exag_z, color);
    }
}

void PeuckerDrainageNetwork::glewReady(){
    shader = new ShaderDrainage();
}

QString PeuckerDrainageNetwork::getName(){
    return QString("Peucker");
}

QWidget* PeuckerDrainageNetwork::getConf(){
    return &conf;
}

void PeuckerDrainageNetwork::calculateGridParallel(Terrain *ter){

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

    const char* kernelSource = loadKernel("peucker.cl");
    cl_program peuckerProgram = clCreateProgramWithSource(context, 1, &kernelSource, NULL, &error);
    checkError(error, "Creating program");
    free((char*)kernelSource);

    int coordszMemorySize = (ter->width)*(ter->height)*sizeof(float);
    int flagsMemorySize = (ter->width)*(ter->height)*sizeof(char);

    float* coordsz = ter->pointsCoordZ.data();
    char* flags = (char*)malloc(flagsMemorySize);


    high_resolution_clock::time_point t1 = high_resolution_clock::now();
    cl_mem d_coordsz = clCreateBuffer(context, CL_MEM_READ_WRITE, coordszMemorySize, NULL, &error);
    checkError(error, "Allocating memory in the device\n");

    cl_mem d_flags = clCreateBuffer(context, CL_MEM_READ_WRITE, flagsMemorySize, NULL, &error);
    checkError(error, "Allocating memory in the device\n");

    cl_mem d_width = clCreateBuffer(context, CL_MEM_READ_ONLY, sizeof(int), NULL, &error);
    checkError(error, "Allocating memory in the device\n");
    cl_mem d_height = clCreateBuffer(context, CL_MEM_READ_ONLY, sizeof(int), NULL, &error);
    checkError(error, "Allocating memory in the device\n");

    error = clEnqueueWriteBuffer(commandQueue, d_coordsz, CL_TRUE, 0, coordszMemorySize, coordsz, 0, 0, NULL);
    checkError(error, "Writing from cpu to device\n");
    error = clEnqueueWriteBuffer(commandQueue, d_flags, CL_TRUE, 0, flagsMemorySize, flags, 0, 0, NULL);
    checkError(error, "Writing from cpu to device\n");
    error = clEnqueueWriteBuffer(commandQueue, d_width, CL_TRUE, 0, sizeof(int), &(ter->width), 0, 0, NULL);
    checkError(error, "Writing from cpu to device\n");
    error = clEnqueueWriteBuffer(commandQueue, d_height, CL_TRUE, 0, sizeof(int), &(ter->height), 0, 0, NULL);
    checkError(error, "Writing from cpu to device\n");

    error = clBuildProgram(peuckerProgram, 0, NULL, "-Werror", NULL, NULL);
    checkError(error, "Building program\n");


    cl_kernel setPeuckerFlagsKernel = clCreateKernel(peuckerProgram, "setPeuckerFlag", &error);
    checkError(error, "Creating setPeuckerFlag Kernel");
    error = clSetKernelArg(setPeuckerFlagsKernel, 0, sizeof(cl_mem), (void*)&d_flags);
    checkError(error, "Setting first argument of the setPeuckerFlag kernel");
    error = clSetKernelArg(setPeuckerFlagsKernel, 1, sizeof(cl_mem), (void*)&d_width);
    checkError(error, "Setting third argument of the markHighestPoint kernel");
    error = clSetKernelArg(setPeuckerFlagsKernel, 2, sizeof(cl_mem), (void*)&d_height);
    checkError(error, "Setting fourth argument of the markHighestPoint kernel");

    cl_kernel markHighestPointKernel = clCreateKernel(peuckerProgram, "markHighestPoint", &error);
    checkError(error, "Creating markHighestPoint Kernel\n");
    error = clSetKernelArg(markHighestPointKernel, 0, sizeof(cl_mem), (void*)&d_coordsz);
    checkError(error, "Setting first argument of the markHighestPoint  kernel");
    error = clSetKernelArg(markHighestPointKernel, 1, sizeof(cl_mem), (void*)&d_flags);
    checkError(error, "Setting second argument of the markHighestPoint kernel");
    error = clSetKernelArg(markHighestPointKernel, 2, sizeof(cl_mem), (void*)&d_width);
    checkError(error, "Setting third argument of the markHighestPoint kernel");
    error = clSetKernelArg(markHighestPointKernel, 3, sizeof(cl_mem), (void*)&d_height);
    checkError(error, "Setting fourth argument of the markHighestPoint kernel");

    size_t localWorkSizeX;
    clGetKernelWorkGroupInfo(markHighestPointKernel, deviceIds[0], CL_KERNEL_PREFERRED_WORK_GROUP_SIZE_MULTIPLE, sizeof(size_t), &localWorkSizeX, NULL);
    int globalWorkSizeX = ter->height*ter->width;
    globalWorkSizeX = globalWorkSizeX - (globalWorkSizeX%localWorkSizeX) + localWorkSizeX;
    const size_t globalWorkSize [] = { globalWorkSizeX, 0, 0 };
    const size_t localWorkSize [] = { localWorkSizeX, 0, 0 };

    error = clEnqueueNDRangeKernel(commandQueue, setPeuckerFlagsKernel, 1, NULL, globalWorkSize, localWorkSize, 0, NULL,  NULL);
    checkError(error, "Running setPeuckerFlag kernel\n");

    error = clEnqueueNDRangeKernel(commandQueue, markHighestPointKernel, 1, NULL, globalWorkSize, localWorkSize, 0, NULL, NULL);
    checkError(error, "Running markHighestPoint kernel\n");

    error = clEnqueueReadBuffer(commandQueue, d_flags, CL_TRUE, 0, flagsMemorySize, flags, 0, NULL, NULL);
    checkError(error, "Reading from device to cpu");

    std::vector<runnel::Point*>& points = ter->struct_point;
    for (int i = 0; i < ter->height*ter->width; i++) {
        points[i]->flags = flags[i];
    }
    free(coordsz);
    free(flags);

    error = clReleaseMemObject(d_coordsz);
    checkError(error, "Releasing memory from device");
    error = clReleaseMemObject(d_flags);
    checkError(error, "Releasing memory from device");
    error = clReleaseMemObject(d_width);
    checkError(error, "Releasing memory from device");
    error = clReleaseMemObject(d_height);
    checkError(error, "Releasing memory from device");

    high_resolution_clock::time_point t2 = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>( t2 - t1 ).count();
    cout << "Elapsed time on Parallel Peucker: " <<  duration/1000 << " miliseg" << endl;


}
void PeuckerDrainageNetwork::calculateGridParallelProfiler(Terrain *ter){

    cl_int error = CL_SUCCESS;

    cl_uint platformIdCount = 0;
    error = clGetPlatformIDs(0, NULL, &platformIdCount);
    checkError(error, "Finding amount of platforms");

    if(platformIdCount != 1) {
        printf("Found %d platforms (only one platform was expected)\n", platformIdCount);
        exit(EXIT_FAILURE);
    }
    cl_platform_id platformIds[platformIdCount];
    error = clGetPlatformIDs (platformIdCount, platformIds, NULL);
    checkError(error, "Getting platforms");

    cl_uint deviceIdCount = 0;
    error = clGetDeviceIDs (platformIds[0], CL_DEVICE_TYPE_CPU, 0, NULL, &deviceIdCount);
    checkError(error, "Finding devices");
    if(deviceIdCount != 1) {
        printf("Found %d devices (only one device was expected)\n", deviceIdCount);
        exit(EXIT_FAILURE);
    }
    cl_device_id deviceIds[deviceIdCount];
    error = clGetDeviceIDs (platformIds [0], CL_DEVICE_TYPE_CPU, deviceIdCount, deviceIds, NULL);
    checkError(error, "Getting devices");

    cl_context context = clCreateContext(NULL, 1, deviceIds, NULL, NULL, &error);
    checkError(error, "Creating context");

    cl_command_queue commandQueue = clCreateCommandQueue(context, deviceIds[0], CL_QUEUE_PROFILING_ENABLE, &error);
    checkError(error, "Creating command queue");

    const char* kernelSource = loadKernel("peucker.cl");
    cl_program peuckerProgram = clCreateProgramWithSource(context, 1, &kernelSource, NULL, &error);
    checkError(error, "Creating program");
    free((char*)kernelSource);

    int coordszMemorySize = (ter->width)*(ter->height)*sizeof(float);
    int flagsMemorySize = (ter->width)*(ter->height)*sizeof(char);

    float* coordsz = (float*)malloc(coordszMemorySize);
    char* flags = (char*)malloc(flagsMemorySize);

    high_resolution_clock::time_point t1 = high_resolution_clock::now();
    std::vector<runnel::Point*>& points = ter->struct_point;
    for(int i = 0; i < (ter->width)*(ter->height); i++){
        coordsz[i] = points[i]->coord.z;
    }
    high_resolution_clock::time_point t2 = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>( t2 - t1 ).count();
    cout << "Elapsed time copying coords: " <<  duration/1000 << " miliseg" << endl;

    printf("Device memory allocation for coordsz: %fMB\n", (float)coordszMemorySize/1048576);
    fflush(stdout);
    cl_mem d_coordsz = clCreateBuffer(context, CL_MEM_READ_WRITE, coordszMemorySize, NULL, &error);
    checkError(error, "Allocating memory in the device\n");

    printf("Device memory allocation for flags: %fMB\n", (float)flagsMemorySize/1048576);
    fflush(stdout);
    cl_mem d_flags = clCreateBuffer(context, CL_MEM_READ_WRITE, flagsMemorySize, NULL, &error);
    checkError(error, "Allocating memory in the device\n");

    cl_mem d_width = clCreateBuffer(context, CL_MEM_READ_ONLY, sizeof(int), NULL, &error);
    checkError(error, "Allocating memory in the device\n");
    cl_mem d_height = clCreateBuffer(context, CL_MEM_READ_ONLY, sizeof(int), NULL, &error);
    checkError(error, "Allocating memory in the device\n");

    error = clEnqueueWriteBuffer(commandQueue, d_coordsz, CL_TRUE, 0, coordszMemorySize, coordsz, 0, 0, NULL);
    checkError(error, "Writing from cpu to device\n");
    error = clEnqueueWriteBuffer(commandQueue, d_flags, CL_TRUE, 0, flagsMemorySize, flags, 0, 0, NULL);
    checkError(error, "Writing from cpu to device\n");
    error = clEnqueueWriteBuffer(commandQueue, d_width, CL_TRUE, 0, sizeof(int), &(ter->width), 0, 0, NULL);
    checkError(error, "Writing from cpu to device\n");
    error = clEnqueueWriteBuffer(commandQueue, d_height, CL_TRUE, 0, sizeof(int), &(ter->height), 0, 0, NULL);
    checkError(error, "Writing from cpu to device\n");

    error = clBuildProgram(peuckerProgram, 0, NULL, "-Werror", NULL, NULL);
    checkError(error, "Building program\n");


    cl_kernel setPeuckerFlagsKernel = clCreateKernel(peuckerProgram, "setPeuckerFlag", &error);
    checkError(error, "Creating setPeuckerFlag Kernel");
    error = clSetKernelArg(setPeuckerFlagsKernel, 0, sizeof(cl_mem), (void*)&d_flags);
    checkError(error, "Setting first argument of the setPeuckerFlag kernel");
    error = clSetKernelArg(setPeuckerFlagsKernel, 1, sizeof(cl_mem), (void*)&d_width);
    checkError(error, "Setting third argument of the markHighestPoint kernel");
    error = clSetKernelArg(setPeuckerFlagsKernel, 2, sizeof(cl_mem), (void*)&d_height);
    checkError(error, "Setting fourth argument of the markHighestPoint kernel");

    cl_kernel markHighestPointKernel = clCreateKernel(peuckerProgram, "markHighestPoint", &error);
    checkError(error, "Creating markHighestPoint Kernel\n");
    error = clSetKernelArg(markHighestPointKernel, 0, sizeof(cl_mem), (void*)&d_coordsz);
    checkError(error, "Setting first argument of the markHighestPoint  kernel");
    error = clSetKernelArg(markHighestPointKernel, 1, sizeof(cl_mem), (void*)&d_flags);
    checkError(error, "Setting second argument of the markHighestPoint kernel");
    error = clSetKernelArg(markHighestPointKernel, 2, sizeof(cl_mem), (void*)&d_width);
    checkError(error, "Setting third argument of the markHighestPoint kernel");
    error = clSetKernelArg(markHighestPointKernel, 3, sizeof(cl_mem), (void*)&d_height);
    checkError(error, "Setting fourth argument of the markHighestPoint kernel");

    size_t prefered = 0;
    clGetKernelWorkGroupInfo(markHighestPointKernel, deviceIds[0], CL_KERNEL_PREFERRED_WORK_GROUP_SIZE_MULTIPLE, sizeof(size_t), &prefered, NULL);
    cout << "prefered size: " << prefered << endl;

    int gws = ter->height*ter->width;
    gws = gws - (gws%prefered) + prefered;
    const size_t globalWorkSize [] = { gws, 0, 0 };
    const size_t localWorkSize [] = { prefered, 0, 0 };
    cl_event event = NULL;
    double elapsed = 0;
    cl_ulong time_start, time_end;

    error = clEnqueueNDRangeKernel(commandQueue, setPeuckerFlagsKernel, 1, NULL, globalWorkSize, localWorkSize, 0, NULL,  &event);
    checkError(error, "Running setPeuckerFlag kernel\n");
    clWaitForEvents(1, &event);
    clGetEventProfilingInfo(event, CL_PROFILING_COMMAND_START, sizeof(time_start), &time_start, NULL);
    clGetEventProfilingInfo(event, CL_PROFILING_COMMAND_END, sizeof(time_end), &time_end, NULL);
    elapsed = (time_end - time_start);
    cout << "Elapsed time in setting flags: " <<  elapsed/1000000.0 << " miliseg" << endl;

    cl_event event2 = NULL;
    error = clEnqueueNDRangeKernel(commandQueue, markHighestPointKernel, 1, NULL, globalWorkSize, localWorkSize, 0, NULL, &event2);
    checkError(error, "Running markHighestPoint kernel\n");
    clWaitForEvents(1, &event2);
    clGetEventProfilingInfo(event2, CL_PROFILING_COMMAND_START, sizeof(time_start), &time_start, NULL);
    clGetEventProfilingInfo(event2, CL_PROFILING_COMMAND_END, sizeof(time_end), &time_end, NULL);
    elapsed = (time_end - time_start);
    cout << "Elapsed time in marking highest: " <<  elapsed/1000000.0 << " miliseg" << endl;

    cl_event event3 = NULL;
    error = clEnqueueReadBuffer(commandQueue, d_flags, CL_TRUE, 0, flagsMemorySize, flags, 0, NULL, &event3);
    checkError(error, "Reading from device to cpu");
    clWaitForEvents(1, &event3);
    clGetEventProfilingInfo(event3, CL_PROFILING_COMMAND_START, sizeof(time_start), &time_start, NULL);
    clGetEventProfilingInfo(event3, CL_PROFILING_COMMAND_END, sizeof(time_end), &time_end, NULL);
    elapsed = (time_end - time_start);
    cout << "Elapsed time reading flags from device to CPU: " <<  elapsed/1000000.0 << " miliseg" << endl;

    t1 = high_resolution_clock::now();
    for (int i = 0; i < ter->height*ter->width; i++) {
        points[i]->flags = flags[i];
    }
    t2 = high_resolution_clock::now();
    duration = duration_cast<microseconds>( t2 - t1 ).count();
    cout << "Elapsed time copying flags: " <<  duration/1000 << " miliseg" << endl;

    free(coordsz);
    free(flags);
    error = clReleaseMemObject(d_coordsz);
    checkError(error, "Releasing memory from device");
    error = clReleaseMemObject(d_flags);
    checkError(error, "Releasing memory from device");
    error = clReleaseMemObject(d_width);
    checkError(error, "Releasing memory from device");
    error = clReleaseMemObject(d_height);
    checkError(error, "Releasing memory from device");

}

void PeuckerDrainageNetwork::calculateGrid(Terrain *ter){
    int width = ter->width;
    int height = ter->height;

    for (runnel::Point* point : ter->struct_point) {
        point->flags = 0;
    }

    for(unsigned int i = 0; i < ter->struct_point.size() ; ++i){
        int fila = i/width;

        if ( (i+1) % width == 0){
            continue;
        }
        if( fila == (height-1)){
            break;
        }
        runnel::Point *points[4];
        points[0] = ter->struct_point[i];
        points[1] = ter->struct_point[i + 1];
        points[2] = ter->struct_point[i + width];
        points[3] = ter->struct_point[i + width + 1];

        if(!points[0] || !points[1] || !points[2] || !points[3]){
            continue;
        }

        runnel::Point *max_point = points[0];
        for(runnel::Point* pto: points){
            if(pto->coord.z > max_point->coord.z){
                max_point = pto;
            }
        }

        for(runnel::Point* pto: points){
            if(pto->coord.z == max_point->coord.z){
                pto->flags = runnel::Point::PEUCKER;
            }
        }
    }
}

std::vector<glm::vec3> PeuckerDrainageNetwork::getPathTree(){
    return points_edge;
}


std::vector<glm::vec3> PeuckerDrainageNetwork::getDrainageColor(){
    std::vector<glm::vec3> color;
    for( runnel::Edge* ed : terr->struct_edge){

        runnel::Point* p1 = ed->point1;
        runnel::Point* p2 = ed->point2;
        float color_p1 = (p1->flags == (p1->PEUCKER))? 1.0f : 0.0f;
        float color_p2 = (p2->flags == (p2->PEUCKER))? 1.0f : 0.0f;
        if((color_p1==0.0f && color_p2==0.0f) ){
            color.push_back(glm::vec3(0.0f,0.0f,1.0f));
            color.push_back(glm::vec3(0.0f,0.0f,1.0f));
            points_edge.push_back(p1->coord);
            points_edge.push_back(p2->coord);
        }
    }
    return color;
}
