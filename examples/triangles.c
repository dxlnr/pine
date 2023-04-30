/* Drawing Triangles */
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <errno.h>

#define PINE_IMPLEMENTATION
#include "../pine.h"

#define WIDTH 1280
#define HEIGHT 960 

static uint32_t pixels[HEIGHT*WIDTH];

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

bool draw_lines_example()
{
    // Set background color.
    fill(pixels, HEIGHT, WIDTH, 0x383838);
    // Draw a triangle.
    fill_triangle(pixels, HEIGHT, WIDTH, 200, 150, 400, 100, 320, 320, 0x000000);
    // Draw a triangle.
    fill_triangle(pixels, HEIGHT, WIDTH, 400, 350, 650, 150, 800, 300, 0x000000);
    // Draw a triangle.
    fill_triangle(pixels, HEIGHT, WIDTH, 800, 350, 900, 200, 1000, 350, 0x000000);
    // Draw a triangle.
    fill_triangle(pixels, HEIGHT, WIDTH, 100, 550, 200, 500, 300, 550, 0x000000);
    // Draw a triangle.
    fill_triangle(pixels, HEIGHT, WIDTH, 100, 900, 150, 580, 60, 600, 0x000000);
    // Draw a triangle.
    fill_triangle(pixels, HEIGHT, WIDTH, 400, 400, 500, 650, 400, 700, 0x000000);

    const char *fpath = "triangles.ppm";
    Errno e = save_to_ppm_file(pixels, HEIGHT, WIDTH, fpath);
    if (e) {
        fprintf(stderr, "ERROR: unable to save file %s: %d\n", fpath, e);
        return 0;
    }
    return 1;
}

int main() {
    
    if (!draw_lines_example()) return -1;
    return 0;
}
