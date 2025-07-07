#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "math3d.h"
#include "canvas.h"
#include "lightsource_renderer.h"
#include "lightning.h"

extern int SCREEN_WIDTH;
extern int SCREEN_HEIGHT;


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




//test for orthoganal projection
vec3 orthographicProject(vec3 p) {
    vec3 projected;
    projected.x = p.x*10 + SCREEN_WIDTH / 2.0f;
    projected.y = -p.y*10 + SCREEN_HEIGHT / 2.0f; // Y is flipped for screen space

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


    vec3 light1 = { 5.0, 5.0, 0 };
    vec3 light2 = { 0.0f, 1.0f, 0.0f };
    
    vec3 reference_point[] = {light1};  // Camera location, light source
    

    //light1 = project_vertex(light1, local, world, camera, proj);
    light1 = orthographicProject(light1);
    draw_line_f(canvas, light1.x, light1.y, light1.x+3, light1.y+3, 30.5f, 0.85);


    light2 = project_vertex(light2, local, world, camera, proj);



    for (int i = 0; i < vertex_count; i++) {


        //edges on the 3d virtuality
        rotated[i] = mat4_apply_to_vec3(world_transform, vertices[i]);

        //edges on the screen
        projected[i] = project_vertex(vertices[i], local, world, camera, proj);
    }




    //distance finding and sorting
edge_t* edge_list = (edge_t*)malloc(edge_count * sizeof(edge_t));

lightning(edge_list, edge_count, rotated, edges, &reference_point, 1);



    for (int i = 0; i < edge_count; i++) {
        vec3 a = projected[edge_list[i].a];
        vec3 b = projected[edge_list[i].b];

        //i added
        // vec3 a = orthographicProject(rotated[edge_list[i].a]);
        // vec3 b = orthographicProject(rotated[edge_list[i].b]);


        if (clip_to_circular_viewport(canvas, a.x, a.y) && clip_to_circular_viewport(canvas, b.x, b.y)) {



            //draw the edges
            draw_line_f(canvas, a.x, a.y, b.x, b.y, 2.5f, (edge_list[i].brightness));
        }
        // draw_line_f(canvas, a.x, a.y, b.x, b.y, 2.0f);  // No clipping, higher intensity

    }

    free(projected);
    free(edge_list);
}


