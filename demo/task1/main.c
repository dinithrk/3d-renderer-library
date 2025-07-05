#include <stdio.h>
#include <math.h>
#include "canvas.h"

#define WIDTH 500
#define HEIGHT 500
#define PI 3.14159265f

int main() {
    canvas_t* canvas = create_canvas(WIDTH, HEIGHT);
    float centerX = WIDTH / 2.0f;
    float centerY = HEIGHT / 2.0f;
    float radius = WIDTH * 0.45f;

    for (int angle = 0; angle < 360; angle += 15) {
        float rad = angle * PI / 180.0f;
        float x = centerX + radius * cosf(rad);
        float y = centerY + radius * sinf(rad);
        draw_line_f(canvas, centerX, centerY, x, y, 1.5f);
    }

    save_canvas_ppm(canvas, "clock_lines.ppm");
    destroy_canvas(canvas);
    printf("Saved output to clock_lines.ppm\n");
    return 0;
}
