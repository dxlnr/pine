#include <stddef.h>
#include <stdio.h>
#include <stdint.h>
#include <errno.h>
#include <string.h>

#define PINE
#include "pine.c"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#define WIDTH 1280
#define HEIGHT 960

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

int main(int argc, char **argv) {
    char fpath[11];
    const char *ff;
    if (argc >= 2) {
        ff = argv[1];
    } else {
        ff = "ppm";
        /* printf("No file format specified. \'ppm\' will be used."); */
    }
    // Construct file path.
    strcpy( fpath, "canvas.");
    strcat( fpath, ff );

    // Set background color.
    fill(pixels, HEIGHT, WIDTH, 0xFFFFFFFF);
    // Draw a triangle.
    /* draw_triangle(pixels, HEIGHT, WIDTH, 200, 150, 400, 100, 320, 320, 0x000000); */
    fill_triangle(pixels, HEIGHT, WIDTH, 200, 150, 400, 100, 320, 320, 0x000000);
    /* fill_triangle_test(pixels, HEIGHT, WIDTH, 200, 150, 400, 100, 320, 320, 0x000000); */
    
    if (strcmp(ff, "png") == 0) {
        if (!stbi_write_png(fpath, WIDTH, HEIGHT, 4, pixels, sizeof(uint32_t)*WIDTH)) {
            fprintf(stderr, "ERROR: unable to save file %s\n", fpath);
            return 1;
        }
    } else {
        Errno e = save_to_ppm_file(pixels, HEIGHT, WIDTH, fpath);
        if (e) {
            fprintf(stderr, "ERROR: unable to save file %s: %d\n", fpath, e);
            return 1;
        }
    }
    return 0;
}
