
struct vec3;
struct mat4;
struct canvas_t;

vec3 project_vertex(vec3 v, mat4 local, mat4 world, mat4 camera, mat4 proj);
int clip_to_circular_viewport(canvas_t* canvas, float x, float y);
vec3 orthographicProject(vec3 p);
void render_wireframe(canvas_t* canvas, vec3* vertices, int vertex_count, int (*edges)[2], int edge_count,
                      mat4 local, mat4 world, mat4 camera, mat4 proj);