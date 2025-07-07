int main() {
    int w = 256;
    int h = 100;
    canvas_t* canvas = create_canvas(w, h);

    for (int x = 0; x < w; x++) {
        float intensity = (float)x / (w - 1);  // from 0.0 to 1.0
        for (int y = h / 2 - 1; y <= h / 2 + 1; y++) {
            set_pixel_f(canvas, x, y, intensity);
        }
    }

    save_canvas_ppm(canvas, "test_gradient.ppm");
    clear_canvas(canvas);
    printf("Saved test_gradient.ppm. Check visually or open in an image viewer.\n");
    return 0;
}