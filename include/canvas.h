#ifndef CANVAS_H
#define CANVAS_H

typedef struct {
    int width;
    int height;
    float** pixels;
} canvas_t;

extern int SCREEN_WIDTH;
extern int SCREEN_HEIGHT;

canvas_t* create_canvas(int width, int height);
//function to craete and allocate memory for canvas

void clear_canvas(canvas_t* canvas);
//to free the memory used for canvas

void set_pixel_f(canvas_t* canvas, float x, float y, float intensity);
//colouring up the pixels

void draw_line_f(canvas_t* canvas, float x0, float y0, float x1, float y1, float thickness, float brightness);
//to draw a line between starting and end point cordinates

void save_canvas_ppm(canvas_t* canvas, const char* filename);
//to save the canvas image

#endif
