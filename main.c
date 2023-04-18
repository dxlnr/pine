#include <stddef.h>
#include <stdio.h>
#include <stdint.h>
#include <errno.h>

#define WIDTH 960
#define HEIGHT 640

void fill(uint32_t *pixels, size_t height, size_t width, uint32_t color)
{
    for (size_t i = 0; i < width*height; ++i) {
        pixels[i] = color;
    }
}

/**
 * Writes a rectangle to memory.
 */
void fill_rect(
    uint32_t *p, 
    size_t p_h, 
    size_t p_w, 
    int r_x, 
    int r_y, 
    size_t r_h, 
    size_t r_w, 
    uint32_t color
) {
    fill(p, p_h, p_w, 0xFFFFFFFF);

    for (size_t i = 0; i < r_h; ++i) {
        for (size_t j = 0; j < r_w; ++j) {
            p[r_y*p_w + r_x + j + i*p_w] = color;
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
    /* fill(pixels, HEIGHT, WIDTH, 0xFFFFFFFF); */
    fill_rect(pixels, HEIGHT, WIDTH, 150, 200, 60, 100, 0xCC0000);

    const char *fpath = "canvas.ppm";
    Errno e = save_to_ppm_file(pixels, HEIGHT, WIDTH, fpath);
    if (e) {
        fprintf(stderr, "ERROR: unable to save file %s: %d\n", fpath, e);
        return 1;
    }

    return 0;
}
