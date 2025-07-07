#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "math3d.h"
#include "canvas.h"
#include "lightning.h"



// Comparator to sort edges from farthest to closest
int compare_edges_by_distance(const void* a, const void* b) {
    float d1 = ((edge_t*)a)->distance;
    float d2 = ((edge_t*)b)->distance;
    return (d2 > d1) - (d2 < d1); // descending order: farthest first
}


float map_value(float value, float old_min, float old_max, float new_min, float new_max) {
    return new_min + (value - old_min) * (new_max - new_min) / (old_max - old_min);
}



void lightning(edge_t* edge_list, int edge_count, vec3* rotated, int (*edges)[2], vec3* reference_point, int referencePoints){
   
  // printf("Entered lightning");
    //edge_t* edge_list = (edge_t*)malloc(edge_count * sizeof(edge_t));


for (int i = 0; i < edge_count; i++) {
    vec3 va = rotated[edges[i][0]];
    vec3 vb = rotated[edges[i][1]];

    // Midpoint of edge
    vec3 mid = {
        .x = (va.x + vb.x) * 0.5f,
        .y = (va.y + vb.y) * 0.5f,
        .z = (va.z + vb.z) * 0.5f
    };

    float dist_sq = 0;

    

    for(int j=0; j< referencePoints; j++){


    // Distance squared to reference point
    float dx = mid.x - reference_point[j].x;
    float dy = mid.y - reference_point[j].y;
    float dz = mid.z - reference_point[j].z;
    dist_sq += (dx * dx + dy * dy + dz * dz);

    

    }

    edge_list[i] = (edge_t){ edges[i][0], edges[i][1], dist_sq, 0 };

}

// Sort edges from farthest to closest
qsort(edge_list, edge_count, sizeof(edge_t), compare_edges_by_distance);


float maxd = edge_list[0].distance;
float mind = edge_list[edge_count-1].distance;

float intensity, distance;

    
for (int i = 0; i < edge_count; i++) {

            //group lines for proper brightness gradient
            if(i%4==0){

                //brightness check on distance to the sources
                distance = edge_list[i].distance;
                intensity = map_value(distance,mind, maxd,0.85,0.15);

            }

            edge_list[i].brightness = intensity;

            
        }

   


}