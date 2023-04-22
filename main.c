#include <stddef.h>
#include <stdio.h>
#include <stdint.h>
#include <errno.h>
#include <math.h>

#define WIDTH 960
#define HEIGHT 640

int min(int a, int b) { return (a > b) ? b : a; }
int max(int a, int b) { return (a > b) ? a : b; }

void fill(uint32_t *pixels, size_t height, size_t width, uint32_t color)
{
    for (size_t i = 0; i < width*height; ++i) {
        pixels[i] = color;
    }
}

/**
 * Writes a rectangle to memory.
 */
void fill_rect(uint32_t *p, size_t p_h, size_t p_w, 
               int r_x, 
               int r_y, 
               size_t r_h, 
               size_t r_w, 
               uint32_t color
) {
    for (size_t i = 0; i < r_h; ++i) {
        int y = r_y + i;
        if (0 <= y && y < p_h) {
            for (size_t j = 0; j < r_w; ++j) {
                int x = r_x + j;
                if (0 <= x && x < p_w) {
                    p[r_y*p_w + r_x + j + i*p_w] = color;
                }
            }
        }
    }
}

/** 
 * Renders a triangle.
 */
void fill_triangle(uint32_t *p, size_t p_h, size_t p_w,
                   int x1, int y1,
                   int x2, int y2,
                   int x3, int y3,
                   uint32_t color) 
{
    double dpw = 1 / (double) p_w;
    double dph = 1 / (double) p_h;

    int y_min_12 = min(y1, y2);
    int x_min_12 = min(x1, x2);
    int y_max_12 = max(y1, y2);
    int x_max_12 = max(x1, x2);

    /* double x12 = x_max_12 - x_min_12; */
    /* double y12 = y_max_12 - y_min_12; */
    double x12 = (double) x2 - (double) x1;
    double y12 = y2 - y1; 
    double alpha = atan(x12/y12);
    double dx12 = 0.0;

    for (size_t i = 0; i < y12; ++i) {
        int dy12 = y_min_12 + i;
        if (0 <= y12 && y12 < dy12) {
            double dx12 = dx12 + (1 * tan(alpha));
            p[y_min_12*p_w + x_min_12 + (int) round(dx12) + i*p_w] = color;
        }
    }
}

typedef int Errno;
#define return_defer(value) do { res = (value); goto defer; } while (0)

int save_to_ppm_file(uint32_t *pixels, size_t height, size_t width, const char *path)
{
    int res = 0;
    FILE *f = NULL;

    {
        f = fopen(path, "wb");
        if (f == NULL) return_defer(errno);

        fprintf(f, "P6\n%zu %zu 255\n", width, height); 
        if (ferror(f)) return_defer(errno);

        for (size_t i = 0; i < width*height; ++i) {
            uint32_t p = pixels[i];
            uint8_t bytes[3] = {
                (p>>(8*0))&0xFF,
                (p>>(8*1))&0xFF,
                (p>>(8*2))&0xFF,
            };
            fwrite(bytes, sizeof(bytes), 1, f);
            if (ferror(f)) return_defer(errno);
        }
    }
defer:
    if (f) fclose(f);
    return res;
}

static uint32_t pixels[HEIGHT*WIDTH];

int main() {
    // Set background color.
    fill(pixels, HEIGHT, WIDTH, 0x383838);
    // Draw a rectangle.
    fill_rect(pixels, HEIGHT, WIDTH, 150, 200, 60, 100, 0xFFFFFF);
    // Draw a triangle.
    fill_triangle(pixels, HEIGHT, WIDTH, 300, 300, 380, 400, 460, 500, 0xFFFFFF);

    const char *fpath = "canvas.ppm";
    Errno e = save_to_ppm_file(pixels, HEIGHT, WIDTH, fpath);
    if (e) {
        fprintf(stderr, "ERROR: unable to save file %s: %d\n", fpath, e);
        return 1;
    }

    return 0;
}
