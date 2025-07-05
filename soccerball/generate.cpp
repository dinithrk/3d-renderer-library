#include <stdio.h>
#include <math.h>

typedef struct {
    double x, y, z;
} Vec3;

int main() {
    const double phi = (1.0 + sqrt(5.0)) / 2.0;

    Vec3 vertices[60] = {
        // Pattern 1: (0, ±1, ±3φ)
        {0, 1, 3*phi}, {0, -1, 3*phi}, {0, 1, -3*phi}, {0, -1, -3*phi},
        {1, 3*phi, 0}, {-1, 3*phi, 0}, {1, -3*phi, 0}, {-1, -3*phi, 0},
        {3*phi, 0, 1}, {3*phi, 0, -1}, {-3*phi, 0, 1}, {-3*phi, 0, -1},

        // Pattern 2: (±1, ±(2+φ), ±2φ)
        {1, (2+phi), 2*phi}, {-1, (2+phi), 2*phi}, {1, -(2+phi), 2*phi}, {-1, -(2+phi), 2*phi},
        {1, (2+phi), -2*phi}, {-1, (2+phi), -2*phi}, {1, -(2+phi), -2*phi}, {-1, -(2+phi), -2*phi},
        {(2+phi), 2*phi, 1}, {-(2+phi), 2*phi, 1}, {(2+phi), -2*phi, 1}, {-(2+phi), -2*phi, 1},
        {(2+phi), 2*phi, -1}, {-(2+phi), 2*phi, -1}, {(2+phi), -2*phi, -1}, {-(2+phi), -2*phi, -1},
        {2*phi, 1, (2+phi)}, {-2*phi, 1, (2+phi)}, {2*phi, -1, (2+phi)}, {-2*phi, -1, (2+phi)},
        {2*phi, 1, -(2+phi)}, {-2*phi, 1, -(2+phi)}, {2*phi, -1, -(2+phi)}, {-2*phi, -1, -(2+phi)},

        // Pattern 3: (±φ, ±2, ±(2φ+1))
        {phi, 2, 2*phi+1}, {-phi, 2, 2*phi+1}, {phi, -2, 2*phi+1}, {-phi, -2, 2*phi+1},
        {phi, 2, -(2*phi+1)}, {-phi, 2, -(2*phi+1)}, {phi, -2, -(2*phi+1)}, {-phi, -2, -(2*phi+1)},
        {2, 2*phi+1, phi}, {-2, 2*phi+1, phi}, {2, -(2*phi+1), phi}, {-2, -(2*phi+1), phi},
        {2, 2*phi+1, -phi}, {-2, 2*phi+1, -phi}, {2, -(2*phi+1), -phi}, {-2, -(2*phi+1), -phi},
        {2*phi+1, phi, 2}, {-2*phi+1, phi, 2}, {2*phi+1, -phi, 2}, {-2*phi+1, -phi, 2},
        {2*phi+1, phi, -2}, {-2*phi+1, phi, -2}, {2*phi+1, -phi, -2}, {-2*phi+1, -phi, -2},

        // Pattern 4: (±φ², ±φ, ±(2φ−1))
        {phi*phi, phi, 2*phi - 1}, {-phi*phi, phi, 2*phi - 1},
        {phi*phi, -phi, 2*phi - 1}, {-phi*phi, -phi, 2*phi - 1},
        {phi*phi, phi, -(2*phi - 1)}, {-phi*phi, phi, -(2*phi - 1)},
        {phi*phi, -phi, -(2*phi - 1)}, {-phi*phi, -phi, -(2*phi - 1)}
    };

    printf("Truncated Icosahedron Vertices (60 total):\n\n");
    for (int i = 0; i < 60; i++) {
        printf("(%f, %f, %f)\n", vertices[i].x, vertices[i].y, vertices[i].z);
    }

    return 0;
}
