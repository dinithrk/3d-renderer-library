#include <math.h>
#include "math3d.h"

vec3 vec3_from_spherical(float r, float theta, float phi) {
    vec3 v;
    v.x = r * sinf(phi) * cosf(theta);
    v.y = r * sinf(phi) * sinf(theta);
    v.z = r * cosf(phi);
    v.is_cartesian = 1;
    return v;
}


float fast_inv_sqrt(float x) {
    float xhalf = 0.5f * x;
    int i = *(int*)&x;
    i = 0x5f3759df - (i >> 1);
    x = *(float*)&i;
    x = x * (1.5f - xhalf * x * x);
    return x;
}

vec3 vec3_normalize_fast(vec3 v) {
    float length_sq = v.x * v.x + v.y * v.y + v.z * v.z;
    float inv_len = fast_inv_sqrt(length_sq);
    v.x *= inv_len;
    v.y *= inv_len;
    v.z *= inv_len;
    return v;
}


vec3 vec3_slerp(vec3 a, vec3 b, float t) {
    float dot = a.x * b.x + a.y * b.y + a.z * b.z;
    if (dot > 0.9995f) { // Linear interpolation for small angles
        vec3 result = {
            .x = a.x + t * (b.x - a.x),
            .y = a.y + t * (b.y - a.y),
            .z = a.z + t * (b.z - a.z)
        };
        return vec3_normalize_fast(result);
    }

    dot = fmaxf(fminf(dot, 1.0f), -1.0f);
    float theta_0 = acosf(dot);
    float theta = theta_0 * t;

    vec3 c = {
        .x = b.x - a.x * dot,
        .y = b.y - a.y * dot,
        .z = b.z - a.z * dot
    };
    c = vec3_normalize_fast(c);

    vec3 result = {
        .x = a.x * cosf(theta) + c.x * sinf(theta),
        .y = a.y * cosf(theta) + c.y * sinf(theta),
        .z = a.z * cosf(theta) + c.z * sinf(theta)
    };
    return result;
}


mat4 mat4_identity() {
    mat4 m = { .m = { 0 } };
    m.m[0] = m.m[5] = m.m[10] = m.m[15] = 1.0f;
    return m;
}


mat4 mat4_mul(mat4 a, mat4 b) {
    mat4 result = {0};
    for (int col = 0; col < 4; ++col) {
        for (int row = 0; row < 4; ++row) {
            for (int i = 0; i < 4; ++i) {
                result.m[col * 4 + row] += a.m[i * 4 + row] * b.m[col * 4 + i];
            }
        }
    }
    return result;
}


mat4 mat4_translate(float tx, float ty, float tz) {
    mat4 m = mat4_identity();
    m.m[12] = tx;
    m.m[13] = ty;
    m.m[14] = tz;
    return m;
}


mat4 mat4_scale(float sx, float sy, float sz) {
    mat4 m = {0};
    m.m[0] = sx;
    m.m[5] = sy;
    m.m[10] = sz;
    m.m[15] = 1.0f;
    return m;
}


mat4 mat4_rotate_xyz(float rx, float ry, float rz) {
    float cx = cosf(rx), sx = sinf(rx);
    float cy = cosf(ry), sy = sinf(ry);
    float cz = cosf(rz), sz = sinf(rz);

    mat4 rx_mat = mat4_identity();
    rx_mat.m[5] = cx; rx_mat.m[6] = sx;
    rx_mat.m[9] = -sx; rx_mat.m[10] = cx;

    mat4 ry_mat = mat4_identity();
    ry_mat.m[0] = cy; ry_mat.m[2] = -sy;
    ry_mat.m[8] = sy; ry_mat.m[10] = cy;

    mat4 rz_mat = mat4_identity();
    rz_mat.m[0] = cz; rz_mat.m[1] = sz;
    rz_mat.m[4] = -sz; rz_mat.m[5] = cz;

    // Combined rotation: Rz * Ry * Rx
    return mat4_mul(rz_mat, mat4_mul(ry_mat, rx_mat));
}


mat4 mat4_frustum_asymmetric(float left, float right, float bottom, float top, float near, float far) {
    mat4 m = {0};

    m.m[0] = (2.0f * near) / (right - left);
    m.m[5] = (2.0f * near) / (top - bottom);
    m.m[8] = (right + left) / (right - left);
    m.m[9] = (top + bottom) / (top - bottom);
    m.m[10] = -(far + near) / (far - near);
    m.m[11] = -1.0f;
    m.m[14] = -(2.0f * far * near) / (far - near);

    return m;
}


vec3 mat4_apply_to_vec3(mat4 m, vec3 v) {
    float x = v.x, y = v.y, z = v.z;

    // Corrected w computation using the last column (not row)
    float w = m.m[3]*x + m.m[7]*y + m.m[11]*z + m.m[15];

    // Avoid division by zero
    if (w == 0.0f) {
        // Return something safe (e.g., center of screen)
        return (vec3){0, 0, 0, 1};  // or some sentinel value
    }

    vec3 result = {
        .x = (m.m[0]*x + m.m[4]*y + m.m[8]*z + m.m[12]) / w,
        .y = (m.m[1]*x + m.m[5]*y + m.m[9]*z + m.m[13]) / w,
        .z = (m.m[2]*x + m.m[6]*y + m.m[10]*z + m.m[14]) / w,
        .is_cartesian = 1
    };
    return result;
}