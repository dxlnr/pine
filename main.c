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
 * Renders a rectangle in a specific color.
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
 * Renders a line with specific color.
 */
void fill_line(uint32_t *p, size_t h, size_t w,
                   int x1, int y1,
                   int x2, int y2,
                   uint32_t color) 
{
    double y;
    double dx = x2 - x1;
    double dy = y2 - y1;
    
    if (dx != 0) {
        if (dx > 0) {
            for (int x = x1; x < x2; ++x) {
                y = y1 + dy * ((double) x - x1) / dx;
                p[(int) y*w + x] = color;
            }
        } else {
            for (int x = x2; x < x1; ++x) {
                y = y1 + dy * ((double) x - x2) / dx * (-1);
                p[(int) y*w + x] = color;
            }
        }
    } else {
        if (dy > 0) {
            for (int y = y1; y < y2; ++y) {
                p[y*w + x1] = color;
            }
        } else {
            for (int y = y2; y < y1; ++y) {
                p[y*w + x1] = color;
            }
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
    // Draw a line.
    fill_line(pixels, HEIGHT, WIDTH, 400, 300, 400, 400, 0xFFFFFF);

    const char *fpath = "canvas.ppm";
    Errno e = save_to_ppm_file(pixels, HEIGHT, WIDTH, fpath);
    if (e) {
        fprintf(stderr, "ERROR: unable to save file %s: %d\n", fpath, e);
        return 1;
    }

    return 0;
}
