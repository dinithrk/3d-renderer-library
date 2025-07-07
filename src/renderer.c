#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "math3d.h"
#include "canvas.h"

extern int SCREEN_WIDTH;
extern int SCREEN_HEIGHT;


float map_value(float value, float old_min, float old_max, float new_min, float new_max) {
    return new_min + (value - old_min) * (new_max - new_min) / (old_max - old_min);
}

vec3 project_vertex(vec3 v, mat4 local, mat4 world, mat4 camera, mat4 proj) {

    mat4 mvp = mat4_mul(proj, mat4_mul(camera, mat4_mul(world, local)));
    //mat4 mvp = mat4_mul(camera, mat4_mul(world, local));
    vec3 projected = mat4_apply_to_vec3(mvp, v);

    
    // Map NDC [-1,1] to screen coordinates
    projected.x = (projected.x * 0.5f + 0.5f) * SCREEN_WIDTH;
    projected.y = (1.0f - (projected.y * 0.5f + 0.5f)) * SCREEN_HEIGHT; // Flip Y for screen space

    // printf("Projected vertex: (%f, %f, %f)\n", projected.x, projected.y, projected.z); // Debug output

    return projected;
}

//circular viewport
int clip_to_circular_viewport(canvas_t* canvas, float x, float y) {
    float cx = canvas->width / 2.0f;
    float cy = canvas->height / 2.0f;
    float radius = fminf(cx, cy);
    float dx = x - cx;
    float dy = y - cy;
    return (dx * dx + dy * dy <= radius * radius);
}



typedef struct {
    int a, b;           // indices of the two vertices forming the edge
    float distance;     // distance from a point (used for sorting)
} edge_t;

// Comparator to sort edges from farthest to closest
int compare_edges_by_distance(const void* a, const void* b) {
    float d1 = ((edge_t*)a)->distance;
    float d2 = ((edge_t*)b)->distance;
    return (d2 > d1) - (d2 < d1); // descending order: farthest first
}


//test for orthoganal projection
vec3 orthographicProject(vec3 p) {
    vec3 projected;
    projected.x = p.x*100 + SCREEN_WIDTH / 2.0f;
    projected.y = -p.y*100 + SCREEN_HEIGHT / 2.0f; // Y is flipped for screen space

    //clear the *100 in future usage
    return projected;
}



void render_wireframe(canvas_t* canvas, vec3* vertices, int vertex_count, int (*edges)[2], int edge_count,
                      mat4 local, mat4 world, mat4 camera, mat4 proj) {
    
    
    
    ///for the edges projected
    vec3* projected = (vec3*)malloc(vertex_count * sizeof(vec3));


    ///for the edges on the 3d
    mat4 world_transform = mat4_mul(world, local);
    vec3* rotated = (vec3*)malloc(vertex_count * sizeof(vec3));


    for (int i = 0; i < vertex_count; i++) {


        //edges on the 3d virtuality
        rotated[i] = mat4_apply_to_vec3(world_transform, vertices[i]);

        //edges on the screen
        projected[i] = project_vertex(vertices[i], local, world, camera, proj);
    }


    vec3 reference_point = { 0.0f, 0.0f, 4.0f };  // Camera location, light source

    //distance finding and sorting
edge_t* edge_list = (edge_t*)malloc(edge_count * sizeof(edge_t));
float max_distance = -1.0f;
int max_edge_index = -1;

for (int i = 0; i < edge_count; i++) {
    vec3 va = rotated[edges[i][0]];
    vec3 vb = rotated[edges[i][1]];

    // Midpoint of edge
    vec3 mid = {
        .x = (va.x + vb.x) * 0.5f,
        .y = (va.y + vb.y) * 0.5f,
        .z = (va.z + vb.z) * 0.5f
    };

    // Distance squared to reference point
    float dx = mid.x - reference_point.x;
    float dy = mid.y - reference_point.y;
    float dz = mid.z - reference_point.z;
    float dist_sq = dx * dx + dy * dy + dz * dz;

    edge_list[i] = (edge_t){ edges[i][0], edges[i][1], dist_sq };

    // Track max
    if (dist_sq > max_distance) {
        max_distance = dist_sq;
        max_edge_index = i;
    }
}

// Sort edges from farthest to closest
qsort(edge_list, edge_count, sizeof(edge_t), compare_edges_by_distance);


float maxd = edge_list[0].distance;
float mind = edge_list[edge_count-1].distance;

float intensity, distance;

    for (int i = 0; i < edge_count; i++) {
        vec3 a = projected[edge_list[i].a];
        vec3 b = projected[edge_list[i].b];

        //i added
        // vec3 a = orthographicProject(rotated[edge_list[i].a]);
        // vec3 b = orthographicProject(rotated[edge_list[i].b]);


        if (clip_to_circular_viewport(canvas, a.x, a.y) && clip_to_circular_viewport(canvas, b.x, b.y)) {

            //group lines for proper brightness gradient
            if( i%4 == 0){

                //brightness check on distance to the camera
                distance = edge_list[i].distance;
                intensity = map_value(distance,mind, maxd,0.85,0.15);
            }



            //draw the edges
            draw_line_f(canvas, a.x, a.y, b.x, b.y, 2.5f, intensity);
        }
        // draw_line_f(canvas, a.x, a.y, b.x, b.y, 2.0f);  // No clipping, higher intensity

    }

    free(projected);
    free(edge_list);
}