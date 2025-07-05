#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "canvas.h"

//creating the desired canvas
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

//function to free the memory of canvas
void clear_canvas(canvas_t* canvas) {
    for (int y = 0; y < canvas->height; y++) {
        free(canvas->pixels[y]);
    }
    free(canvas->pixels);
    free(canvas);
}


//function to colourup the pixels 
void set_pixel_f(canvas_t* canvas, float x, float y, float intensity) {
    int x0 = (int)floorf(x);
    int y0 = (int)floorf(y);
    float dx = x - x0;
    float dy = y - y0;

    float smoothness = 0.15;
    //smoothness factor

    //bound checking and pixel colouring
    if(x0 >= 0 && y0 >= 0){

        if (x0 < (canvas->width) && y0 < (canvas->height) ){

            canvas->pixels[y0][x0] += (intensity * (1 - dx) * (1 - dy) + smoothness );
        }
        if (x0+1 < (canvas->width) && y0 < (canvas->height) ){

            canvas->pixels[y0][x0+1] += (intensity * dx * (1 - dy) + smoothness);
        }
        if (x0 < (canvas->width) && y0+1 < (canvas->height) ){

            canvas->pixels[y0+1][x0] += (intensity * (1 - dx) * dy + smoothness);
        }
        if (x0+1 < (canvas->width) && y0+1 < (canvas->height) ){

            canvas->pixels[y0+1][x0+1] += (intensity * dx * dy + smoothness);
        }
        
        
    }

}


//function to draw lines by using a starting and end point cordinates
void draw_line_f(canvas_t* canvas, float x0, float y0, float x1, float y1, float thickness) {
    float x_length = x1 - x0;
    float y_length = y1 - y0;

    float steps = fmaxf(fabsf(x_length), fabsf(y_length));
    //step count determination according to length by x, y directions

    float xstep = x_length / steps;
    float ystep = y_length / steps;

    for (int i = 0; i <= steps; i++) {
        float x = x0 + i * xstep;
        float y = y0 + i * ystep;
        for (float dy = -thickness / 2; dy <= thickness / 2; dy++) {
            for (float dx = -thickness / 2; dx <= thickness / 2; dx++) {
                if (dx * dx + dy * dy <= (thickness / 2) * (thickness / 2)) {
                    set_pixel_f(canvas, x + dx, y + dy, 0.75f);
                    //intensity 0.75 for smoothness
                }
            }
        }
    }
}

//saving the canvas as a ppm file which will be converted to a jpg later
void save_canvas_ppm(canvas_t* canvas, const char* filename) {

    //openning file to write
    FILE* fp = fopen(filename, "w");


    fprintf(fp, "P3\n%d %d\n255\n", canvas->width, canvas->height);
    //readable format


    for (int y = 0; y < canvas->height; y++) {
        for (int x = 0; x < canvas->width; x++) {

            int value = (int)(canvas->pixels[y][x] * 255.0f);
            if (value > 255) value = 255;
            //cap the values at 255 = white


            fprintf(fp, "%d %d %d ", value, value, value);
            //rgb values are same since its greyscale
        }
        fprintf(fp, "\n");
        //new line after each row of pixels
    }
    fclose(fp);
    //closing the opened file
}
