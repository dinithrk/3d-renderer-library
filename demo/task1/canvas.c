#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "canvas.h"

canvas_t* create_canvas(int width, int height) {
    canvas_t* c = (canvas_t*)malloc(sizeof(canvas_t));
    c->width = width;
    c->height = height;
    c->pixels = (float**)malloc(height * sizeof(float*));
    for (int y = 0; y < height; y++) {
        c->pixels[y] = (float*)calloc(width, sizeof(float));
    }
    return c;
}

void destroy_canvas(canvas_t* canvas) {
    for (int y = 0; y < canvas->height; y++) {
        free(canvas->pixels[y]);
    }
    free(canvas->pixels);
    free(canvas);
}

void set_pixel_f(canvas_t* canvas, float x, float y, float intensity) {
    int x0 = (int)floorf(x);
    int y0 = (int)floorf(y);
    float dx = x - x0;
    float dy = y - y0;

    float w00 = (1 - dx) * (1 - dy);
    float w10 = dx * (1 - dy);
    float w01 = (1 - dx) * dy;
    float w11 = dx * dy;

    if (x0 >= 0 && y0 >= 0 && x0 < canvas->width && y0 < canvas->height)
        canvas->pixels[y0][x0] += intensity * w00;
    if (x0+1 >= 0 && y0 >= 0 && x0+1 < canvas->width && y0 < canvas->height)
        canvas->pixels[y0][x0+1] += intensity * w10;
    if (x0 >= 0 && y0+1 >= 0 && x0 < canvas->width && y0+1 < canvas->height)
        canvas->pixels[y0+1][x0] += intensity * w01;
    if (x0+1 >= 0 && y0+1 >= 0 && x0+1 < canvas->width && y0+1 < canvas->height)
        canvas->pixels[y0+1][x0+1] += intensity * w11;
}

void draw_line_f(canvas_t* canvas, float x0, float y0, float x1, float y1, float thickness) {
    float dx = x1 - x0;
    float dy = y1 - y0;
    float steps = fmaxf(fabsf(dx), fabsf(dy));
    float x_inc = dx / steps;
    float y_inc = dy / steps;

    for (int i = 0; i <= steps; i++) {
        float x = x0 + i * x_inc;
        float y = y0 + i * y_inc;
        for (float dy = -thickness / 2; dy <= thickness / 2; dy++) {
            for (float dx = -thickness / 2; dx <= thickness / 2; dx++) {
                if (dx * dx + dy * dy <= (thickness / 2) * (thickness / 2)) {
                    set_pixel_f(canvas, x + dx, y + dy, 1.0f);
                }
            }
        }
    }
}

void save_canvas_ppm(canvas_t* canvas, const char* filename) {
    FILE* fp = fopen(filename, "w");
    fprintf(fp, "P3\n%d %d\n255\n", canvas->width, canvas->height);
    for (int y = 0; y < canvas->height; y++) {
        for (int x = 0; x < canvas->width; x++) {
            int value = (int)(canvas->pixels[y][x] * 255.0f);
            if (value > 255) value = 255;
            fprintf(fp, "%d %d %d ", value, value, value);
        }
        fprintf(fp, "\n");
    }
    fclose(fp);
}
