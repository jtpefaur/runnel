#include "diedralangledrainage.h"
#include "CL/cl.h"
#include <chrono>
#include <iostream>

using namespace std;
using namespace std::chrono;

extern "C"
{
    #include "openclutils.h"
}

DiedralAngleDrainage::DiedralAngleDrainage(){
    QObject::connect(&conf, SIGNAL(changeAttr()), this, SLOT(changeAttr()));

}

DiedralAngleDrainage::~DiedralAngleDrainage(){
    if( shader ){
        delete shader;
    }
}

void DiedralAngleDrainage::run(Terrain *terrain){
    this->terrain = terrain;

    high_resolution_clock::time_point t1 = high_resolution_clock::now();
    std::vector<glm::vec3> angle_value_edge = terrain->calculateNeighbourByEdges();
    std::vector<glm::vec3> height = terrain->calculateHeightArray();
    high_resolution_clock::time_point t2 = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>( t2 - t1 ).count();
    cout << "Elapsed time on Sequential Diedral: " <<  duration/1000 << " miliseg" << endl;

    position_terrain = terrain->getVectorPoints();
    shader->fillPositionBuffer(position_terrain, angle_value_edge, height );
}

void DiedralAngleDrainage::runParallel(Terrain *terrain){
    terrain = terrain;

    high_resolution_clock::time_point t1;
    high_resolution_clock::time_point t2;
    high_resolution_clock::time_point t3;
    high_resolution_clock::time_point t4;

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

    cl_command_queue commandQueue = clCreateCommandQueue(context, deviceIds[0], CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE  , &error);
    checkError(error, "Creating command queue");

    const char* kernelSource = loadKernel("diedral.cl");
    cl_program diedralProgram = clCreateProgramWithSource(context, 1, &kernelSource, NULL, &error);
    checkError(error, "Creating program");
    free((char*)kernelSource);

    std::unordered_set <runnel::Triangle*>  &triangles = terrain->struct_triangle;
    int trianglesSize = triangles.size();

    int trianglePointsCoordsSize = trianglesSize * sizeof(cl_float3)*3;
    int triangleHeightBytes = trianglesSize * sizeof(glm::vec3)*3;

    int trianglePointsIdSize = trianglesSize * sizeof(cl_int3);
    int triangleEdgesIdSize = trianglesSize * sizeof(cl_int2) * 3;
    int neighbourTrianglesSize = trianglesSize * sizeof(cl_int)*3;
    int neighbourTrianglesNormalSize = trianglesSize*sizeof(cl_float3)*3*2;
    int edgeVectorsSize = trianglesSize*sizeof(cl_float3)*3;
    int anglesBytes = trianglesSize*sizeof(glm::vec3)*3;

    cl_float3* trianglePointsCoords = (cl_float3*)malloc(trianglePointsCoordsSize);
    glm::vec3* triangleHeight = (glm::vec3*)malloc(triangleHeightBytes);

    cl_int3* trianglePointsId = (cl_int3*)malloc(trianglePointsIdSize);
    cl_int2* triangleEdgesId= (cl_int2*)malloc(triangleEdgesIdSize);
    cl_int* neighbourTriangles = (cl_int*)malloc(neighbourTrianglesSize);
    cl_float3* neighbourTrianglesNormal = (cl_float3*)malloc(neighbourTrianglesNormalSize);
    cl_float3* edgeVectors = (cl_float3*)malloc(edgeVectorsSize);
    glm::vec3* angles = (glm::vec3*)malloc(anglesBytes);

    t3 = high_resolution_clock::now();
    int triangleIndex = 0;
    for(runnel::Triangle* triangle : triangles) {
        runnel::Triangle& currentTriangle =*triangle;

        int j = triangleIndex*3;
        trianglePointsCoords[j] = {currentTriangle.points[0]->coord.x, currentTriangle.points[0]->coord.y, currentTriangle.points[0]->coord.z};

        trianglePointsCoords[j+1] = {currentTriangle.points[1]->coord.x, currentTriangle.points[1]->coord.y, currentTriangle.points[1]->coord.z};

        trianglePointsCoords[j+2] = {currentTriangle.points[2]->coord.x, currentTriangle.points[2]->coord.y, currentTriangle.points[2]->coord.z};


        trianglePointsId[triangleIndex] = {currentTriangle.points[0]->ident,
                               currentTriangle.points[1]->ident,
                               currentTriangle.points[2]->ident};

        int triangleEdgesIndex = triangleIndex*3;
        triangleEdgesId[triangleEdgesIndex] = {currentTriangle.edges[0]->point1->ident,
                                               currentTriangle.edges[0]->point2->ident};
        triangleEdgesId[triangleEdgesIndex+1] = {currentTriangle.edges[1]->point1->ident,
                                               currentTriangle.edges[1]->point2->ident};
        triangleEdgesId[triangleEdgesIndex+2] = {currentTriangle.edges[2]->point1->ident,
                                               currentTriangle.edges[2]->point2->ident};

        int neighbourTrianglesIndex = triangleIndex*3;
        neighbourTriangles[neighbourTrianglesIndex] = currentTriangle.edges[0]->neighbour_triangle.size();
        neighbourTriangles[neighbourTrianglesIndex+1] = currentTriangle.edges[1]->neighbour_triangle.size();
        neighbourTriangles[neighbourTrianglesIndex+2] = currentTriangle.edges[2]->neighbour_triangle.size();

        int neighbourTrianglesNormalIndex = triangleIndex*6;
        if(neighbourTriangles[neighbourTrianglesIndex] == 2) {
            neighbourTrianglesNormal[neighbourTrianglesNormalIndex] = {currentTriangle.edges[0]->neighbour_triangle[0]->normal.x,
                                                                       currentTriangle.edges[0]->neighbour_triangle[0]->normal.y,
                                                                       currentTriangle.edges[0]->neighbour_triangle[0]->normal.z};

            neighbourTrianglesNormal[neighbourTrianglesNormalIndex+1] = {currentTriangle.edges[0]->neighbour_triangle[1]->normal.x,
                                                                       currentTriangle.edges[0]->neighbour_triangle[1]->normal.y,
                                                                       currentTriangle.edges[0]->neighbour_triangle[1]->normal.z};
        }

        if(neighbourTriangles[neighbourTrianglesIndex+1] == 2) {
            neighbourTrianglesNormal[neighbourTrianglesNormalIndex+2] = {currentTriangle.edges[1]->neighbour_triangle[0]->normal.x,
                                                                       currentTriangle.edges[1]->neighbour_triangle[0]->normal.y,
                                                                       currentTriangle.edges[1]->neighbour_triangle[0]->normal.z};

            neighbourTrianglesNormal[neighbourTrianglesNormalIndex+3] = {currentTriangle.edges[1]->neighbour_triangle[1]->normal.x,
                                                                       currentTriangle.edges[1]->neighbour_triangle[1]->normal.y,
                                                                       currentTriangle.edges[1]->neighbour_triangle[1]->normal.z};
        }

        if(neighbourTriangles[neighbourTrianglesIndex+2] == 2) {
            neighbourTrianglesNormal[neighbourTrianglesNormalIndex+4] = {currentTriangle.edges[2]->neighbour_triangle[0]->normal.x,
                                                                       currentTriangle.edges[2]->neighbour_triangle[0]->normal.y,
                                                                       currentTriangle.edges[2]->neighbour_triangle[0]->normal.z};

            neighbourTrianglesNormal[neighbourTrianglesNormalIndex+5] = {currentTriangle.edges[2]->neighbour_triangle[1]->normal.x,
                                                                       currentTriangle.edges[2]->neighbour_triangle[1]->normal.y,
                                                                       currentTriangle.edges[2]->neighbour_triangle[1]->normal.z};
        }

        int edgeVectorIndex = triangleIndex*3;
        edgeVectors[edgeVectorIndex] = {currentTriangle.edges[0]->edge_vector.x,
                                        currentTriangle.edges[0]->edge_vector.y,
                                        currentTriangle.edges[0]->edge_vector.z};

        edgeVectors[edgeVectorIndex+1] = {currentTriangle.edges[1]->edge_vector.x,
                                        currentTriangle.edges[1]->edge_vector.y,
                                        currentTriangle.edges[1]->edge_vector.z};

        edgeVectors[edgeVectorIndex+2] = {currentTriangle.edges[2]->edge_vector.x,
                                        currentTriangle.edges[2]->edge_vector.y,
                                        currentTriangle.edges[2]->edge_vector.z};
        triangleIndex++;

    }
    t4 = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>( t4 - t3 ).count();
    cout << "Elapsed time on setting everything for calculateNeighbourhoodByEdges: " <<  duration/1000 << " miliseg" << endl;

    t1 = high_resolution_clock::now();
    cl_mem d_trianglePointsCoords = clCreateBuffer(context, CL_MEM_READ_ONLY, trianglePointsCoordsSize, NULL, &error);
    checkError(error, "Allocating memory in the device\n");
    cl_mem d_trianglesSize = clCreateBuffer(context, CL_MEM_READ_ONLY, sizeof(int), NULL, &error);
    checkError(error, "Allocating memory in the device\n");
    cl_mem d_triangleHeight = clCreateBuffer(context, CL_MEM_WRITE_ONLY, triangleHeightBytes, NULL, &error);
    checkError(error, "Allocating memory in the device\n");

    cl_mem d_trianglePointsId = clCreateBuffer(context, CL_MEM_READ_ONLY, trianglePointsIdSize, NULL, &error);
    checkError(error, "Allocating memory in the device\n");
    cl_mem d_triangleEdgesId = clCreateBuffer(context, CL_MEM_READ_ONLY, triangleEdgesIdSize, NULL, &error);
    checkError(error, "Allocating memory in the device\n");
    cl_mem d_neighbourTriangles = clCreateBuffer(context, CL_MEM_READ_ONLY, neighbourTrianglesSize, NULL, &error);
    checkError(error, "Allocating memory in the device\n");
    cl_mem d_neighbourTrianglesNormal = clCreateBuffer(context, CL_MEM_READ_ONLY, neighbourTrianglesNormalSize, NULL, &error);
    checkError(error, "Allocating memory in the device\n");
    cl_mem d_edgeVectors = clCreateBuffer(context, CL_MEM_READ_ONLY, edgeVectorsSize, NULL, &error);
    checkError(error, "Allocating memory in the device\n");
    cl_mem d_angles = clCreateBuffer(context, CL_MEM_WRITE_ONLY, anglesBytes, NULL, &error);
    checkError(error, "Allocating memory in the device\n");

    cl_event writeTrianglePointsCoordsEvent;
    cl_event writeTrianglesSizeEvent;

    error = clEnqueueWriteBuffer(commandQueue, d_trianglePointsCoords, CL_TRUE, 0, trianglePointsCoordsSize, trianglePointsCoords, 0, 0, &writeTrianglePointsCoordsEvent);
    checkError(error, "Writing from cpu to device\n");
    error = clEnqueueWriteBuffer(commandQueue, d_trianglesSize, CL_TRUE, 0, sizeof(int), &trianglesSize, 0, 0, &writeTrianglesSizeEvent);
    checkError(error, "Writing from cpu to device\n");

    cl_event writeTrianglePointsIdEvent;
    cl_event writeTriangleEdgesIdEvent;
    cl_event writeNeighbourTrianglesEvent;
    cl_event writeNeighbourTrianglesNormalEvent;
    cl_event writeEdgeVectorsEvent;

    error = clEnqueueWriteBuffer(commandQueue, d_trianglePointsId, CL_TRUE, 0, trianglePointsIdSize, trianglePointsId, 0, 0, &writeTrianglePointsIdEvent);
    checkError(error, "Writing from cpu to device\n");
    error = clEnqueueWriteBuffer(commandQueue, d_triangleEdgesId, CL_TRUE, 0, triangleEdgesIdSize, triangleEdgesId, 0, 0, &writeTriangleEdgesIdEvent);
    checkError(error, "Writing from cpu to device\n");
    error = clEnqueueWriteBuffer(commandQueue, d_neighbourTriangles, CL_TRUE, 0, neighbourTrianglesSize, neighbourTriangles, 0, 0, &writeNeighbourTrianglesEvent);
    checkError(error, "Writing from cpu to device\n");
    error = clEnqueueWriteBuffer(commandQueue, d_neighbourTrianglesNormal, CL_TRUE, 0, neighbourTrianglesNormalSize, neighbourTrianglesNormal, 0, 0, &writeNeighbourTrianglesNormalEvent);
    checkError(error, "Writing from cpu to device\n");
    error = clEnqueueWriteBuffer(commandQueue, d_edgeVectors, CL_TRUE, 0, edgeVectorsSize, edgeVectors, 0, 0, &writeEdgeVectorsEvent);
    checkError(error, "Writing from cpu to device\n");

    error = clBuildProgram(diedralProgram, 0, NULL, "-Werror", NULL, NULL);
    checkError(error, "Building program\n");

    cl_kernel calculateHeightArrayKernel = clCreateKernel(diedralProgram, "calculateHeightArray", &error);
    checkError(error, "Creating calculateHeightArray Kernel");
    error = clSetKernelArg(calculateHeightArrayKernel, 0, sizeof(cl_mem), (void*)&d_trianglePointsCoords);
    checkError(error, "Setting first argument of the calculateHeightArray kernel");
    error = clSetKernelArg(calculateHeightArrayKernel, 1, sizeof(cl_mem), (void*)&d_trianglesSize);
    checkError(error, "Setting second argument of the calculateHeightArray kernel");
    error = clSetKernelArg(calculateHeightArrayKernel, 2, sizeof(cl_mem), (void*)&d_triangleHeight);
    checkError(error, "Setting third argument of the calculateHeightArray kernel");

    cl_kernel calculateNeighbourByEdgesKernel = clCreateKernel(diedralProgram, "calculateNeighbourByEdges", &error);
    checkError(error, "Creating calculateNeighbourByEdges Kernel");
    error = clSetKernelArg(calculateNeighbourByEdgesKernel, 0, sizeof(cl_mem), (void*)&d_trianglePointsId);
    checkError(error, "Setting first argument of the calculateNeighbourByEdges kernel");
    error = clSetKernelArg(calculateNeighbourByEdgesKernel, 1, sizeof(cl_mem), (void*)&d_triangleEdgesId);
    checkError(error, "Setting second argument of the calculateNeighbourByEdges kernel");
    error = clSetKernelArg(calculateNeighbourByEdgesKernel, 2, sizeof(cl_mem), (void*)&d_neighbourTriangles);
    checkError(error, "Setting third argument of the calculateNeighbourByEdges kernel");
    error = clSetKernelArg(calculateNeighbourByEdgesKernel, 3, sizeof(cl_mem), (void*)&d_neighbourTrianglesNormal);
    checkError(error, "Setting fourth argument of the calculateNeighbourByEdges kernel");
    error = clSetKernelArg(calculateNeighbourByEdgesKernel, 4, sizeof(cl_mem), (void*)&d_edgeVectors);
    checkError(error, "Setting fifth argument of the calculateNeighbourByEdges kernel");
    error = clSetKernelArg(calculateNeighbourByEdgesKernel, 5, sizeof(cl_mem), (void*)&d_trianglesSize);
    checkError(error, "Setting sixth argument of the calculateNeighbourByEdges kernel");
    error = clSetKernelArg(calculateNeighbourByEdgesKernel, 6, sizeof(cl_mem), (void*)&d_angles);
    checkError(error, "Setting seventh argument of the calculateNeighbourByEdges kernel");

    size_t localWorkSizeX;
    clGetKernelWorkGroupInfo(calculateNeighbourByEdgesKernel, deviceIds[0], CL_KERNEL_PREFERRED_WORK_GROUP_SIZE_MULTIPLE, sizeof(size_t), &localWorkSizeX, NULL);
    int globalWorkSizeX = triangles.size();
    globalWorkSizeX = globalWorkSizeX - (globalWorkSizeX%localWorkSizeX) + localWorkSizeX;
    const size_t globalWorkSize [] = { globalWorkSizeX, 0, 0 };
    const size_t localWorkSize [] = { localWorkSizeX, 0, 0 };

    cl_event calculateHeightArrayEvent;
    cl_event calculateNeighbourByEdgesEvent;

    cl_event calculateHeightArrayWaitingList[] = {writeTrianglePointsCoordsEvent,
                                                  writeTrianglesSizeEvent};
    error = clEnqueueNDRangeKernel(commandQueue, calculateHeightArrayKernel, 1, NULL, globalWorkSize, localWorkSize, 2, calculateHeightArrayWaitingList,  &calculateHeightArrayEvent);
    checkError(error, "Running calculateHeightArray kernel\n");
    cl_event calculateNeighbourByEdgesWaitingList[] = {writeTrianglePointsIdEvent,
                                                       writeTriangleEdgesIdEvent,
                                                       writeNeighbourTrianglesEvent,
                                                       writeNeighbourTrianglesNormalEvent,
                                                       writeEdgeVectorsEvent,
                                                       writeTrianglesSizeEvent};
    error = clEnqueueNDRangeKernel(commandQueue, calculateNeighbourByEdgesKernel, 1, NULL, globalWorkSize, localWorkSize, 6, calculateNeighbourByEdgesWaitingList,  &calculateNeighbourByEdgesEvent);
    checkError(error, "Running calculateNeighbourByEdges kernel\n");

    error = clEnqueueReadBuffer(commandQueue, d_triangleHeight, CL_TRUE, 0, triangleHeightBytes, triangleHeight, 1, &calculateHeightArrayEvent, NULL);
    checkError(error, "Reading from device to cpu");
    error = clEnqueueReadBuffer(commandQueue, d_angles, CL_TRUE, 0, anglesBytes, angles, 1, &calculateNeighbourByEdgesEvent, NULL);
    checkError(error, "Reading from device to cpu");

    free(trianglePointsCoords);

    free(trianglePointsId);
    free(triangleEdgesId);
    free(neighbourTriangles);
    free(neighbourTrianglesNormal);
    free(edgeVectors);

    error = clReleaseMemObject(d_triangleHeight);
    checkError(error, "Releasing memory from device");
    error = clReleaseMemObject(d_trianglesSize);
    checkError(error, "Releasing memory from device");
    error = clReleaseMemObject(d_trianglePointsCoords);
    checkError(error, "Releasing memory from device");

    error = clReleaseMemObject(d_trianglePointsId);
    checkError(error, "Releasing memory from device");
    error = clReleaseMemObject(d_triangleEdgesId);
    checkError(error, "Releasing memory from device");
    error = clReleaseMemObject(d_neighbourTriangles);
    checkError(error, "Releasing memory from device");
    error = clReleaseMemObject(d_neighbourTrianglesNormal);
    checkError(error, "Releasing memory from device");
    error = clReleaseMemObject(d_edgeVectors);
    checkError(error, "Releasing memory from device");
    error = clReleaseMemObject(d_angles);
    checkError(error, "Releasing memory from device");

    t2 = high_resolution_clock::now();
    duration = duration_cast<microseconds>( t2 - t1 ).count();
    cout << "Elapsed time on Parallel Diedral: " <<  duration/1000 << " miliseg" << endl;

    position_terrain = terrain->getVectorPoints();
    shader->fillPositionBuffer(position_terrain, angles, anglesBytes/sizeof(glm::vec3),  triangleHeight, triangleHeightBytes/sizeof(glm::vec3));
    free(triangleHeight);
    free(angles);
}

void DiedralAngleDrainage::render(glm::mat4 matrix, float exag_z, glm::vec3 color){
    float min_angle = conf.getMinAngle();
    float max_angle = conf.getMaxAngle();
    float width_line = conf.getLineWidth();
    glm::vec3 color_min= glm::vec3(0,1,0);
    glm::vec3 color_max = glm::vec3(1,0,0);;
    if ( shader ){
        shader->render(matrix, exag_z, min_angle, max_angle, width_line, color_min, color_max);
    }
}

void DiedralAngleDrainage::glewReady(){
    shader = new ShaderAngleEdge();
}

QString DiedralAngleDrainage::getName(){
    return QString("Angle Diedral");
}

QWidget* DiedralAngleDrainage::getConf(){
    return &conf;
}

void DiedralAngleDrainage::changeAttr(){
    this->run(terrain);
    emit reload();
}

std::vector<glm::vec3> DiedralAngleDrainage::getPathTree(){
    return position_terrain;
}
