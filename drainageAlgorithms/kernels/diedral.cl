typedef struct {
    float x;
    float y;
    float z;
} __attribute__((__packed__)) float3Packed;


__kernel void calculateHeightArray(__global float3* trianglePointsCoords,
                                   __global int* pTrianglesSize,
                                   __global float3Packed* triangleHeight) {
    int id = get_global_id(0);
    int trianglesSize = *pTrianglesSize;

    if(id < trianglesSize) {
        id = id*3;
        float3 point0 = trianglePointsCoords[id];
        float3 point1 = trianglePointsCoords[id+1];
        float3 point2 = trianglePointsCoords[id+2];

        float3 vec_a = point1 - point0;
        float3 vec_b = point2 - point1;
        float3 vec_c = point0 - point2;

        float3 sides;
        sides.x = sqrt(vec_a.x*vec_a.x + vec_a.y*vec_a.y + vec_a.z*vec_a.z);
        sides.y = sqrt(vec_b.x*vec_b.x + vec_b.y*vec_b.y + vec_b.z*vec_b.z);
        sides.z = sqrt(vec_c.x*vec_c.x + vec_c.y*vec_c.y + vec_c.z*vec_c.z);

        float s = sides.x + sides.y + sides.z;
        float3 heightY = (float3)(0.0f, 2*sqrt(s*(s - sides.x)*(s - sides.y)*(s - sides.z))/sides.y, 0.0f);
        float3 heightZ = (float3)(0.0f, 0.0f, 2*sqrt(s*(s - sides.x)*(s - sides.y)*(s - sides.z))/sides.z);
        float3 heightX = (float3)(2*sqrt(s*(s - sides.x)*(s - sides.y)*(s - sides.z))/sides.x, 0.0f, 0.0f);

        triangleHeight[id].x = heightY.x;
        triangleHeight[id].y = heightY.y;
        triangleHeight[id].z = heightY.z;

        triangleHeight[id+1].x = heightZ.x;
        triangleHeight[id+1].y = heightZ.y;
        triangleHeight[id+1].z = heightZ.z;

        triangleHeight[id+2].x = heightX.x;
        triangleHeight[id+2].y = heightX.y;
        triangleHeight[id+2].z = heightX.z;

    }
}

__kernel void calculateNeighbourByEdges(__global int3* trianglePointsId,
                                        __global int2* triangleEdgesId,
                                        __global int* edgeNeighbourTriangles,
                                        __global float3* edgeNeighbourTrianglesNormal,
                                        __global float3* edgeVectors,
                                        __global int* pTrianglesSize,
                                        __global float3Packed* angles) {

    int id = get_global_id(0);
    int trianglesSize = *pTrianglesSize;

    if(id < trianglesSize) {

        float3 value;
        int triangleEdge = id*3;
        int currentEdgeNeighbourTrianglesNormal = triangleEdge*2;
        int anglesIndex = id*3;

        for(int edgeIndex = 0; edgeIndex<3; edgeIndex++, currentEdgeNeighbourTrianglesNormal+=2, triangleEdge++) {

            float diedralAngle = 180;

            if(edgeNeighbourTriangles[triangleEdge] == 2) {
                diedralAngle = acospi(dot(edgeNeighbourTrianglesNormal[currentEdgeNeighbourTrianglesNormal], edgeNeighbourTrianglesNormal[currentEdgeNeighbourTrianglesNormal+1]))*180.0;
                float3 crossProduct = cross(edgeNeighbourTrianglesNormal[currentEdgeNeighbourTrianglesNormal], edgeNeighbourTrianglesNormal[currentEdgeNeighbourTrianglesNormal+1]);

                if(dot(crossProduct, edgeVectors[triangleEdge]) >= 0){
                        diedralAngle = -diedralAngle;
                }

            }

            if(trianglePointsId[id].x == triangleEdgesId[triangleEdge].x && trianglePointsId[id].y == triangleEdgesId[triangleEdge].y) {
                value.x = diedralAngle;
            }

            if(trianglePointsId[id].y == triangleEdgesId[triangleEdge].x && trianglePointsId[id].x == triangleEdgesId[triangleEdge].y) {
                value.x = diedralAngle;
            }

            if(trianglePointsId[id].z == triangleEdgesId[triangleEdge].x && trianglePointsId[id].y == triangleEdgesId[triangleEdge].y) {
                value.y = diedralAngle;
            }

            if(trianglePointsId[id].y == triangleEdgesId[triangleEdge].x && trianglePointsId[id].z == triangleEdgesId[triangleEdge].y) {
                value.y = diedralAngle;
            }

            if(trianglePointsId[id].z == triangleEdgesId[triangleEdge].x && trianglePointsId[id].x == triangleEdgesId[triangleEdge].y) {
                value.z = diedralAngle;
            }

            if(trianglePointsId[id].x == triangleEdgesId[triangleEdge].x && trianglePointsId[id].z == triangleEdgesId[triangleEdge].y) {
                value.z = diedralAngle;
            }
        }

       angles[anglesIndex].x = value.x;
       angles[anglesIndex].y = value.y;
       angles[anglesIndex].z = value.z;

       angles[anglesIndex+1].x = value.x;
       angles[anglesIndex+1].y = value.y;
       angles[anglesIndex+1].z = value.z;

       angles[anglesIndex+2].x = value.x;
       angles[anglesIndex+2].y = value.y;
       angles[anglesIndex+2].z = value.z;


    }
}
