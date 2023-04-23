#include <stddef.h>
#include <stdio.h>
#include <stdint.h>
#include <errno.h>

#include "pine.h"

#define WIDTH 1280
#define HEIGHT 960

int min(int a, int b) { return (a > b) ? b : a; }
int max(int a, int b) { return (a > b) ? a : b; }

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
    // Draw a line.
    draw_line(pixels, HEIGHT, WIDTH, 0, 0, 100, 100, 0xFFFFFF);
    // Draw a line.
    draw_line(pixels, HEIGHT, WIDTH, 100, 100, 200, 0, 0xFFFFFF);
    // Draw a line.
    draw_line(pixels, HEIGHT, WIDTH, 200, 200, 100, 100, 0xFFFFFF);
    // Draw a line.
    draw_line(pixels, HEIGHT, WIDTH, 100, 100, 0, 200, 0xFFFFFF);
    // Draw a line.
    draw_line(pixels, HEIGHT, WIDTH, 100, 100, 200, 100, 0xFFFFFF);
    // Draw a line.
    draw_line(pixels, HEIGHT, WIDTH, 0, 100, 100, 100, 0xFFFFFF);
    // Draw a line.
    draw_line(pixels, HEIGHT, WIDTH, 100, 0, 100, 100, 0xFFFFFF);
    // Draw a line.
    draw_line(pixels, HEIGHT, WIDTH, 100, 100, 100, 200, 0xFFFFFF);

    const char *fpath = "canvas.ppm";
    Errno e = save_to_ppm_file(pixels, HEIGHT, WIDTH, fpath);
    if (e) {
        fprintf(stderr, "ERROR: unable to save file %s: %d\n", fpath, e);
        return 1;
    }

    return 0;
}
