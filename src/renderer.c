#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "math3d.h"
#include "canvas.h"

extern int SCREEN_WIDTH;
extern int SCREEN_HEIGHT;

vec3 project_vertex(vec3 v, mat4 local, mat4 world, mat4 camera, mat4 proj) {
    mat4 mvp = mat4_mul(proj, mat4_mul(camera, mat4_mul(world, local)));
    vec3 projected = mat4_apply_to_vec3(mvp, v);

    
    // Map NDC [-1,1] to screen coordinates
    projected.x = (projected.x * 0.5f + 0.5f) * SCREEN_WIDTH;
    projected.y = (1.0f - (projected.y * 0.5f + 0.5f)) * SCREEN_HEIGHT; // Flip Y for screen space

    // printf("Projected vertex: (%f, %f, %f)\n", projected.x, projected.y, projected.z); // Debug output

    return projected;
}


int clip_to_circular_viewport(canvas_t* canvas, float x, float y) {
    float cx = canvas->width / 2.0f;
    float cy = canvas->height / 2.0f;
    float radius = fminf(cx, cy);
    float dx = x - cx;
    float dy = y - cy;
    return (dx * dx + dy * dy <= radius * radius);
}


typedef struct {
    int a, b;     // indices of vertices
    float depth;  // average Z for depth sorting
} edge_t;

int compare_edges(const void* a, const void* b) {
    float z1 = ((edge_t*)a)->depth;
    float z2 = ((edge_t*)b)->depth;
    return (z2 > z1) - (z2 < z1);  // sort back to front
}

void render_wireframe(canvas_t* canvas, vec3* vertices, int vertex_count, int (*edges)[2], int edge_count,
                      mat4 local, mat4 world, mat4 camera, mat4 proj) {
    vec3* projected = (vec3*)malloc(vertex_count * sizeof(vec3));
    for (int i = 0; i < vertex_count; i++) {
        projected[i] = project_vertex(vertices[i], local, world, camera, proj);
    }

    edge_t* edge_list = (edge_t*)malloc(edge_count * sizeof(edge_t));
    for (int i = 0; i < edge_count; i++) {
        vec3 va = projected[edges[i][0]];
        vec3 vb = projected[edges[i][1]];
        edge_list[i] = (edge_t){ edges[i][0], edges[i][1], (va.z + vb.z) * 0.5f };
    }

    qsort(edge_list, edge_count, sizeof(edge_t), compare_edges);

    for (int i = 0; i < edge_count; i++) {
        vec3 a = projected[edge_list[i].a];
        vec3 b = projected[edge_list[i].b];
        if (clip_to_circular_viewport(canvas, a.x, a.y) && clip_to_circular_viewport(canvas, b.x, b.y)) {
            // printf("Drawing edge (%f, %f) -> (%f, %f)\n", a.x, a.y, b.x, b.y);
            draw_line_f(canvas, a.x, a.y, b.x, b.y, 1.2f);
        }
        // draw_line_f(canvas, a.x, a.y, b.x, b.y, 2.0f);  // No clipping, higher intensity

    }

    free(projected);
    free(edge_list);
}