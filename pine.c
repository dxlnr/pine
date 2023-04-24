#ifndef PINE 
#define PINE 

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <errno.h>
#include <math.h>


double rlt(double r) { return 1.0014 + 0.0086 * r - 1.4886 * pow(r, 2) + 0.5344 * pow(r, 3); }

int min(int a, int b) { return (a > b) ? b : a; }
int max(int a, int b) { return (a > b) ? a : b; }

/* uint32_t intensify_pixel(double dist) { */
/*     double alpha = 1 - pow((dist * 2 / 3), 2); */
/* } */

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
void draw_line_simple(uint32_t *p, size_t h, size_t w,
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
            for (int x = x1; x > x2; --x) {
                y = y2 - dy * ((double) x - x2) / dx * (-1);
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

/** 
 * Renders a line with specific color.
 */
void draw_line(uint32_t *p, size_t h, size_t w,
               int x1, int y1,
               int x2, int y2,
               uint32_t color) 
{
    int xs;
    int xe;
    int ys;
    int ye;
    if (abs(y2 - y1) < abs(x2 - x1)) {
        if (x1 > x2) {
            xs = x2;
            xe = x1;
            ys = y2;
            ye = y1;
        } else {
            xs = x1;
            xe = x2;
            ys = y1;
            ye = y2;
        }
        //  
        int dx = xe - xs;
        int dy = ye - ys;

        int yi = 1;
        if (dy < 0) {
            yi = -1;
            dy = -dy;
        }
        int d = (2 * dy) - dx;
        int y = ys;

        for (int x = xs; x < xe; ++x) {
            p[y*w + x] = color;
            if (d > 0) {
                y = y + yi;
                d = d + (2 * (dy - dx));
            } else {
                d = d + 2 * dy;
            }
        }
    } else {
        if (y1 > y2) {
            xs = x2;
            xe = x1;
            ys = y2;
            ye = y1;
        } else {
            xs = x1;
            xe = x2;
            ys = y1;
            ye = y2;
        }
        //  
        int dx = xe - xs;
        int dy = ye - ys;

        int xi = 1;
        if (dx < 0) {
            xi = -1;
            dx = -dx;
        }
        int d = (2 * dx) - dy;
        int x = xs;

        for (int y = ys; y < ye; ++y) {
            p[y*w + x] = color;
            if (d > 0) {
                x = x + xi;
                d = d + (2 * (dx - dy));
            } else {
                d = d + 2 * dx;
            }
        }
    }
}

/** 
 * Renders a line with specific color.
 */
void draw_line_aa(uint32_t *p, size_t h, size_t w,
                    int x1, int y1,
                    int x2, int y2,
                    uint32_t color) 
{
}
/** 
 * Renders a triangle with specific color.
 */
void draw_triangle(uint32_t *p, size_t h, size_t w,
               int x1, int y1,
               int x2, int y2,
               int x3, int y3,
               uint32_t color) 
{ 
    draw_line(p, h, w, x1, y1, x2, y2, color); 
    draw_line(p, h, w, x1, y1, x3, y3, color); 
    draw_line(p, h, w, x2, y2, x3, y3, color); 
}

/** 
 * Renders a triangle with specific color.
 */
void fill_triangle(uint32_t *p, size_t h, size_t w,
               int x1, int y1,
               int x2, int y2,
               int x3, int y3,
               uint32_t color) 
{ 
    /* int ty; */
    /* int ys; */

    /* // 21 */
    /* int dx21 = x2 - x1; */
    /* int dy21 = y2 - y1; */

    /* int y21i = 1; */
    /* if (dy21 < 0) { */
    /*     y21i = -1; */
    /*     dy21 = -dy21; */
    /* } */
    /* int d21 = (2 * dy21) - dx21; */
    /* int y21 = y1; */

    /* // 31 */
    /* int dx31 = x3 - x1; */
    /* int dy31 = y3 - y1; */

    /* int y31i = 1; */
    /* if (dy31 < 0) { */
    /*     y31i = -1; */
    /*     dy31 = -dy31; */
    /* } */
    /* int d31 = (2 * dy31) - dx31; */
    /* int y31 = y1; */

    
    /* int xe = x2; */

    /* for (int x = x1; x < x2; ++x) { */
    /*     for (int x21 = x; x21 < x3; ++x21) { */
    /*         p[y21*w + x21] = color; */
    /*     } */
    /*     for (int x31 = x; x31 < x3; ++x31) { */
    /*         p[y31*w + x31] = color; */
    /*     } */
    /*     // y values */
    /*     if (d21 > 0) { */
    /*         y21 = y21 + y21i; */
    /*         d21 = d21 + (2 * (dy21 - dx21)); */
    /*     } else { */
    /*         d21 = d21 + 2 * dy21; */
    /*     } */
    /*     if (d31 > 0) { */
    /*         y31 = y31 + y31i; */
    /*         d31 = d31 + (2 * (dy31 - dx31)); */
    /*     } else { */
    /*         d31 = d31 + 2 * dy31; */
    /*     } */
        
    /*     ty = y31; */

    /*     /1* // x values *1/ */
    /*     /1* if (d32 > 0) { *1/ */
    /*     /1*     xe21 = x + x32i; *1/ */
    /*     /1*     d32 = d32 + (2 * (dx32 - dy32)); *1/ */
    /*     /1* } else { *1/ */
    /*     /1*     d32 = d32 + 2 * dx32; *1/ */
    /*     /1* } *1/ */
    /* } */ 

    /* int tx = x2; */

    /* // t */

    /* // 32 */
    /* int dx23 = x2 - x3; */
    /* int dy23 = y2 - y3; */

    /* int y23i = 1; */
    /* if (dy23 < 0) { */
    /*     y23i = -1; */
    /*     dy23 = -dy23; */
    /* } */
    /* int d23 = (2 * dy23) - dx23; */
    /* int y23 = y3; */

    /* for (int x = x3; x > x2; --x) { */
    /*     for (int x21 = x; x21 < x3; ++x21) { */
    /*         p[y21*w + x21] = color; */
    /*     } */
    /*     for (int x31 = x; x31 < x3; ++x31) { */
    /*         p[y31*w + x31] = color; */
    /*     } */
    /* } */
}
#endif
