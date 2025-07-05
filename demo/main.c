#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "canvas.h"
#include "math3d.h"

int SCREEN_WIDTH = 800;
int SCREEN_HEIGHT = 800;

#define VERTEX_COUNT 8
#define EDGE_COUNT 12

#define FRAME_COUNT 60
#define DELTA_T 0.1f
#define ROT_SPEED 1.0f

void render_wireframe(canvas_t* canvas, vec3* vertices, int vertex_count, int (*edges)[2], int edge_count,
                      mat4 local, mat4 world, mat4 camera, mat4 projection);

// Ball vertices for a truncated icosahedron
static const vec3 CUBE_VERTICES[8] = {
    { .x = -1, .y = -1, .z = -1, .is_cartesian = 1 }, // 0
    { .x =  1, .y = -1, .z = -1, .is_cartesian = 1 }, // 1
    { .x =  1, .y =  1, .z = -1, .is_cartesian = 1 }, // 2
    { .x = -1, .y =  1, .z = -1, .is_cartesian = 1 }, // 3
    { .x = -1, .y = -1, .z =  1, .is_cartesian = 1 }, // 4
    { .x =  1, .y = -1, .z =  1, .is_cartesian = 1 }, // 5
    { .x =  1, .y =  1, .z =  1, .is_cartesian = 1 }, // 6
    { .x = -1, .y =  1, .z =  1, .is_cartesian = 1 }  // 7
};


// Edges for the truncated icosahedron
static const int CUBE_EDGES[12][2] = {
    {0, 1}, {1, 2}, {2, 3}, {3, 0}, // Bottom face
    {4, 5}, {5, 6}, {6, 7}, {7, 4}, // Top face
    {0, 4}, {1, 5}, {2, 6}, {3, 7}  // Vertical edges
};


int main() {
    canvas_t* canvas = create_canvas(SCREEN_WIDTH, SCREEN_HEIGHT);

    vec3 vertices[VERTEX_COUNT];
    for (int i = 0; i < VERTEX_COUNT; i++) {
        vertices[i] = vec3_normalize_fast(CUBE_VERTICES[i]);
    }

    mat4 camera = mat4_translate(0, 0, -2.0f);
    mat4 projection = mat4_frustum_asymmetric(-2, 2, -2, 2, 1.0f, 10.0f);


    vec3 axis_start = {1, 0, 0, 1};
    vec3 axis_end = {0, 1, 0, 1};

    // // Debug: Save initial frame with center pixel set
    // set_pixel_f(canvas, 400, 400, 1.0f);  // center pixel
    // save_canvas_ppm(canvas, "test_frame.ppm");
    // exit(0);  // quit early to isolate test

    
    for (int frame = 0; frame < FRAME_COUNT; frame++) {
        float t = frame * DELTA_T;
        float slerp_t = (sinf(t * ROT_SPEED * 0.5f) * 0.5f) + 0.5f;
        vec3 axis = vec3_slerp(axis_start, axis_end, slerp_t);

        mat4 local = mat4_identity();
        mat4 world = mat4_rotate_xyz(axis.x * t * ROT_SPEED,
                                     axis.y * t * ROT_SPEED,
                                     axis.z * t * ROT_SPEED);

        render_wireframe(canvas, vertices, VERTEX_COUNT,
                 (int (*)[2])CUBE_EDGES, EDGE_COUNT,
                 local, world, camera, projection);

        // Debug: Count bright pixels in the frame
        int bright_pixels = 0;
        for (int y = 0; y < canvas->height; y++) {
            for (int x = 0; x < canvas->width; x++) {
                if (canvas->pixels[y][x] > 0.1f) {
                    bright_pixels++;
                }
            }
        }
        printf("Frame %d: Bright pixels = %d\n", frame, bright_pixels);

        
        char filename[64];
        sprintf(filename, "frame_%03d.ppm", frame);
        save_canvas_ppm(canvas, filename);

        // Clear the canvas between frames
        for (int y = 0; y < canvas->height; y++)
            memset(canvas->pixels[y], 0, sizeof(float) * canvas->width);
    }

    clear_canvas(canvas);
    printf("Animation frames saved. Run convert to make GIF.\n");

    return 0;
}
