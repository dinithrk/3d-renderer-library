#include <stdio.h>
#include "math3d.h"

int main() {
    vec3 cube[8] = {
        {-1, -1, -1}, {1, -1, -1}, {1, 1, -1}, {-1, 1, -1},
        {-1, -1,  1}, {1, -1,  1}, {1, 1,  1}, {-1, 1,  1}
    };

    // Build transform matrix
    mat4 model = mat4_rotate_xyz(0.5f, 0.7f, 0.3f);         // Rotate
    mat4 translate = mat4_translate(0.0f, 0.0f, -5.0f);     // Move cube back
    model = mat4_mul(translate, model);                     // Translate * Rotate

    // Perspective projection matrix
    float near = 1.0f, far = 10.0f;
    float left = -1.0f, right = 1.0f;
    float bottom = -1.0f, top = 1.0f;
    mat4 projection = mat4_frustum_asymmetric(left, right, bottom, top, near, far);

    // Final MVP matrix
    mat4 mvp = mat4_mul(projection, model);

    printf("Transformed cube vertices:\n");
    for (int i = 0; i < 8; ++i) {
        vec3 v = cube[i];
        vec3 result = mat4_apply_to_vec3(mvp, v);

        printf("Vertex %d: (%.2f, %.2f, %.2f)\n", i, result.x, result.y, result.z);
    }

    return 0;
}