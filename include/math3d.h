typedef struct {
    union {
        struct { float x, y, z; };
        struct { float r, theta, phi; };
    };
    int is_cartesian; // 1 if using x, y, z; 0 if using r, theta, phi
} vec3;


typedef struct {
    float m[16]; // column-major layout
} mat4;


// Function declarations
vec3 vec3_from_spherical(float r, float theta, float phi);
vec3 vec3_normalize_fast(vec3 v);
vec3 vec3_slerp(vec3 a, vec3 b, float t);
vec3 mat4_apply_to_vec3(mat4 m, vec3 v);

mat4 mat4_identity();
mat4 mat4_translate(float tx, float ty, float tz);
mat4 mat4_scale(float sx, float sy, float sz);
mat4 mat4_rotate_xyz(float rx, float ry, float rz);
mat4 mat4_frustum_asymmetric(float left, float right, float bottom, float top, float near, float far);
mat4 mat4_mul(mat4 a, mat4 b);