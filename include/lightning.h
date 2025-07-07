typedef struct {
    int a, b;           // indices of the two vertices forming the edge
    float distance;     // distance from a point (used for sorting)
    float brightness;
} edge_t;

struct vec3;

int compare_edges_by_distance(const void* a, const void* b);
float map_value(float value, float old_min, float old_max, float new_min, float new_max);
void lightning(edge_t* edge_list, int edge_count, vec3* rotated, int (*edges)[2], vec3* reference_point, int referencePoints);