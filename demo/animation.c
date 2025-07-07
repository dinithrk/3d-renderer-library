#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "canvas.h"
#include "math3d.h"
#include "lightning.h"

int SCREEN_WIDTH = 800;
int SCREEN_HEIGHT = 800;

#define VERTEX_COUNT 60
#define EDGE_COUNT 90

#define FRAME_COUNT 130
#define DELTA_T 0.08f
#define ROT_SPEED 1.0f

void render_wireframe(canvas_t* canvas, vec3* vertices, int vertex_count, int (*edges)[2], int edge_count,
                      mat4 local, mat4 world, mat4 camera, mat4 projection);

// Ball vertices for a truncated icosahedron
static const vec3 ico_vertices[60] = {
    {.x = 0.000000, -1.000000, .z = -4.854102, .is_cartesian = 1}, {.x = 0.000000, .y = 1.000000, .z = -4.854102, .is_cartesian = 1},
    {.x = -1.618034, .y = -2.000000, .z = -4.236068, .is_cartesian = 1}, {.x = 1.618034, .y = -2.000000, .z = -4.236068, .is_cartesian = 1},
    {.x = -1.618034, .y = 2.000000, .z = -4.236068, .is_cartesian = 1}, {.x = 1.618034, .y = 2.000000, .z = -4.236068, .is_cartesian = 1},
    {.x = -3.236068, .y = -1.000000, .z = -3.618034, .is_cartesian = 1}, {.x = 3.236068, .y = -1.000000, .z = -3.618034, .is_cartesian = 1},
    {.x = -3.236068, .y = 1.000000, .z = -3.618034, .is_cartesian = 1}, {.x = 3.236068, .y = 1.000000, .z = -3.618034, .is_cartesian = 1},
    {.x = -1.000000, .y = -3.618034, .z = -3.236068, .is_cartesian = 1}, {.x = 1.000000, .y = -3.618034, .z = -3.236068, .is_cartesian = 1},
    {.x = -1.000000, .y = 3.618034, .z = -3.236068, .is_cartesian = 1}, {.x = 1.000000, .y = 3.618034, .z = -3.236068, .is_cartesian = 1},
    {.x = -4.236068, .y = -1.618034, .z = -2.000000, .is_cartesian = 1}, {.x = 4.236068, .y = -1.618034, .z = -2.000000, .is_cartesian = 1},
    {.x = -4.236068, .y = 1.618034, .z = -2.000000, .is_cartesian = 1}, {.x = 4.236068, .y = 1.618034, .z = -2.000000, .is_cartesian = 1},
    {.x = -2.000000, .y = -4.236068, .z = -1.618034, .is_cartesian = 1}, {.x = 2.000000, .y = -4.236068, .z = -1.618034, .is_cartesian = 1},
    {.x = -2.000000, .y = 4.236068, .z = -1.618034, .is_cartesian = 1}, {.x = 2.000000, .y = 4.236068, .z = -1.618034, .is_cartesian = 1},
    {.x = -3.618034, .y = -3.236068, .z = -1.000000, .is_cartesian = 1}, {.x = 3.618034, .y = -3.236068, .z = -1.000000, .is_cartesian = 1},
    {.x = -4.854102, .y = 0.000000, .z = -1.000000, .is_cartesian = 1}, {.x = 4.854102, .y = 0.000000, .z = -1.000000, .is_cartesian = 1},
    {.x = -3.618034, .y = 3.236068, .z = -1.000000, .is_cartesian = 1}, {.x = 3.618034, .y = 3.236068, .z = -1.000000, .is_cartesian = 1},
    {.x = -1.000000, .y = -4.854102, .z = 0.000000, .is_cartesian = 1}, {.x = 1.000000, .y = -4.854102, .z = 0.000000, .is_cartesian = 1},
    {.x = -1.000000, .y = 4.854102, .z = 0.000000, .is_cartesian = 1}, {.x = 1.000000, .y = 4.854102, .z = 0.000000, .is_cartesian = 1},
    {.x = -3.618034, .y = -3.236068, .z = 1.000000, .is_cartesian = 1}, {.x = 3.618034, .y = -3.236068, .z = 1.000000, .is_cartesian = 1},
    {.x = -4.854102, .y = 0.000000, .z = 1.000000, .is_cartesian = 1}, {.x = 4.854102, .y = 0.000000, .z = 1.000000, .is_cartesian = 1},
    {.x = -3.618034, .y = 3.236068, .z = 1.000000, .is_cartesian = 1}, {.x = 3.618034, .y = 3.236068, .z = 1.000000, .is_cartesian = 1},
    {.x = -2.000000, .y = -4.236068, .z = 1.618034, .is_cartesian = 1}, {.x = 2.000000, .y = -4.236068, .z = 1.618034, .is_cartesian = 1},
    {.x = -2.000000, .y = 4.236068, .z = 1.618034, .is_cartesian = 1}, {.x = 2.000000, .y = 4.236068, .z = 1.618034, .is_cartesian = 1},
    {.x = -4.236068, .y = -1.618034, .z = 2.000000, .is_cartesian = 1}, {.x = 4.236068, .y = -1.618034, .z = 2.000000, .is_cartesian = 1},
    {.x = -4.236068, .y = 1.618034, .z = 2.000000, .is_cartesian = 1}, {.x = 4.236068, .y = 1.618034, .z = 2.000000, .is_cartesian = 1},
    {.x = -1.000000, .y = -3.618034, .z = 3.236068, .is_cartesian = 1}, {.x = 1.000000, .y = -3.618034, .z = 3.236068, .is_cartesian = 1},
    {.x = -1.000000, .y = 3.618034, .z = 3.236068, .is_cartesian = 1}, {.x = 1.000000, .y = 3.618034, .z = 3.236068, .is_cartesian = 1},
    {.x = -3.236068, .y = -1.000000, .z = 3.618034, .is_cartesian = 1}, {.x = 3.236068, .y = -1.000000, .z = 3.618034, .is_cartesian = 1},
    {.x = -3.236068, .y = 1.000000, .z = 3.618034, .is_cartesian = 1}, {.x = 3.236068, .y = 1.000000, .z = 3.618034, .is_cartesian = 1},
    {.x = -1.618034, .y = -2.000000, .z = 4.236068, .is_cartesian = 1}, {.x = 1.618034, .y = -2.000000, .z = 4.236068, .is_cartesian = 1},
    {.x = -1.618034, .y = 2.000000, .z = 4.236068, .is_cartesian = 1}, {.x = 1.618034, .y = 2.000000, .z = 4.236068, .is_cartesian = 1},
    {.x = 0.000000, .y = -1.000000, .z = 4.854102, .is_cartesian = 1}, {.x = 0.000000, .y = 1.000000, .z = 4.854102, .is_cartesian = 1}
};


// Edges for the truncated icosahedron
static const int ico_edges[90][2] = {
    {0, 1}, {0, 2}, {0, 3}, {1, 4}, {1, 5},
    {2, 6}, {2, 10}, {3, 7}, {3, 11}, {4, 8},
    {4, 12}, {5, 9}, {5, 13}, {6, 8}, {6, 14},
    {7, 9}, {7, 15}, {8, 16}, {9, 17}, {10, 11},
    {10, 18}, {11, 19}, {12, 13}, {12, 20}, {13, 21},
    {14, 22}, {14, 24}, {15, 23}, {15, 25}, {16, 24},
    {16, 26}, {17, 25}, {17, 27}, {18, 22}, {18, 28},
    {19, 23}, {19, 29}, {20, 26}, {20, 30}, {21, 27},
    {21, 31}, {22, 32}, {23, 33}, {24, 34}, {25, 35},
    {26, 36}, {27, 37}, {28, 29}, {28, 38}, {29, 39},
    {30, 31}, {30, 40}, {31, 41}, {32, 38}, {32, 42},
    {33, 39}, {33, 43}, {34, 42}, {34, 44}, {35, 43},
    {35, 45}, {36, 40}, {36, 44}, {37, 41}, {37, 45},
    {38, 46}, {39, 47}, {40, 48}, {41, 49}, {42, 50},
    {43, 51}, {44, 52}, {45, 53}, {46, 47}, {46, 54},
    {47, 55}, {48, 49}, {48, 56}, {49, 57}, {50, 52},
    {50, 54}, {51, 53}, {51, 55}, {52, 56}, {53, 57},
    {54, 58}, {55, 58}, {56, 59}, {57, 59}, {58, 59}
};


static const vec3 tetra_vertices[4] = {
    {.x =  0.0f,  .y =  4.0f,  .z =  4.0f,  .is_cartesian = 1}, // 0
    {.x = -8.0f,  .y = -4.0f,  .z =  4.0f,  .is_cartesian = 1}, // 1
    {.x = -8.0f,  .y =  4.0f,  .z = -4.0f,  .is_cartesian = 1}, // 2
    {.x =  0.0f,  .y = -4.0f,  .z = -4.0f,  .is_cartesian = 1}  // 3
};



// Edges for the tetrahedron
static const int tetra_edges[6][2] = {
    {0, 1}, {0, 2}, {0, 3},
    {1, 2}, {1, 3}, {2, 3}
};





static const vec3 octa_vertices[6] = {
    {.x = 10.0f,  .y =  0.0f,  .z =  0.0f,  .is_cartesian = 1}, // 0 - Right
    {.x = -2.0f,  .y =  0.0f,  .z =  0.0f,  .is_cartesian = 1}, // 1 - Left
    {.x =  4.0f,  .y =  6.0f,  .z =  0.0f,  .is_cartesian = 1}, // 2 - Top
    {.x =  4.0f,  .y = -6.0f,  .z =  0.0f,  .is_cartesian = 1}, // 3 - Bottom
    {.x =  4.0f,  .y =  0.0f,  .z =  6.0f,  .is_cartesian = 1}, // 4 - Front
    {.x =  4.0f,  .y =  0.0f,  .z = -6.0f,  .is_cartesian = 1}  // 5 - Back
};


// Edges for the octahedron
static const int octa_edges[12][2] = {
    {0, 2}, {0, 3}, {0, 4}, {0, 5}, // Right vertex connections
    {1, 2}, {1, 3}, {1, 4}, {1, 5}, // Left vertex connections
    {2, 4}, {2, 5}, // Top connections
    {3, 4}, {3, 5}  // Bottom connections
};



int main() {
    canvas_t* canvas = create_canvas(SCREEN_WIDTH, SCREEN_HEIGHT);

    // vec3 tetralan_vertices[4];
    // for (int i = 0; i < 4; i++) {
    //     tetralan_vertices[i] = vec3_normalize_fast(tetra_vertices[i]);
    // }

    // vec3 octalan_vertices[6];
    // for (int i = 0; i < 6; i++) {
    //     octalan_vertices[i] = vec3_normalize_fast(octa_vertices[i]);
    // }

     vec3 vertices[VERTEX_COUNT];
    for (int i = 0; i < VERTEX_COUNT; i++) {
        vertices[i] = vec3_normalize_fast(ico_vertices[i]);
    }

    //camera and projection setup
    mat4 camera = mat4_translate(0, 0, -5.0f);
    mat4 projection = mat4_frustum_asymmetric(-1, 1, -1, 1, 1.0f, 3.0f);


    vec3 axis_start = {1, 0, 0, 1};
    vec3 axis_end = {0, 1, 0, 1};

    printf("This may take while...\n");
    for (int frame = 0; frame < FRAME_COUNT; frame++) {


        if(frame = FRAME_COUNT/2){
            printf("Halfway done...\n");
        }



        float t = frame * DELTA_T;
        float slerp_t = (sinf(t * ROT_SPEED * 0.5f) * 0.5f) + 0.5f;
        vec3 axis = vec3_slerp(axis_start, axis_end, slerp_t);

        mat4 local = mat4_identity();
       
        // mat4 rotate = mat4_rotate_xyz(axis.x * t * ROT_SPEED,
        //                       axis.y * t * ROT_SPEED,
        //                       axis.z * t * ROT_SPEED);

        mat4 rotate = mat4_rotate_xyz(0,axis.y * t * ROT_SPEED,0);

        
        //curvy translation
        float x = 0 + 0.7 *(1 - cosf(t * ROT_SPEED));
        float z = 0 + 0.7 *(1- sinf(t * ROT_SPEED));

        //translation
        mat4 translate = mat4_translate(x, .0f, z);

        //the total changes
        mat4 world = mat4_mul( rotate, translate);


        render_wireframe(canvas, vertices, VERTEX_COUNT,
                 (int (*)[2])ico_edges, EDGE_COUNT,
                 local, world, camera, projection);


        // render_wireframe(canvas, tetralan_vertices, 4,
        //          (int (*)[2])tetra_edges, 6,
        //          local, world, camera, projection);

        // render_wireframe(canvas, octalan_vertices, 6,
        //          (int (*)[2])octa_edges, 8,
        //          local, world, camera, projection);


        //endpoint of drawing         

        // Debug: Count bright pixels in the frame
        int bright_pixels = 0;
        for (int y = 0; y < canvas->height; y++) {
            for (int x = 0; x < canvas->width; x++) {
                if (canvas->pixels[y][x] > 0.1f) {
                    bright_pixels++;
                }
            }
        }
        //printf("Frame %d: Bright pixels = %d\n", frame, bright_pixels);

        //saving framers to a ppm
        char filename[134];
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
