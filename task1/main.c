#include <stdio.h>
#include <math.h>
#include "canvas.h"

#define WIDTH 500
#define HEIGHT 500
#define PI 3.14159265f

int main() {

    float canvas_size = 600;

    canvas_t* canvas = create_canvas(canvas_size, canvas_size);     //600 x 600 canvas

    float center = canvas_size / 2;         //sice x,y cordinates are same in center

    float radius = canvas_size/2 - 20;     //little less than the canvas

    for (int angle = 0; angle < 360; angle += 15) {

        float x = center + radius * cosf(angle * PI / 180);
        float y = center + radius * sinf(angle * PI / 180);
        //ending points of the lines

        draw_line_f(canvas, center, center, x, y, 1.5);
    }

    save_canvas_ppm(canvas, "clock_lines.ppm");
    clear_canvas(canvas);
    //clearing up the canvas to free memory
    printf("Saved output to clock_lines.ppm\n");
    return 0;
}
