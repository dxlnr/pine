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
    
    /* // Set manually this has to be ordered automatically later. */
    int xt = x2;
    int xm = x1;
    int xd = x3;
    int yt = y2;
    int ym = y1;
    int yd = y3;

    int xs;
    int xe;
    int ys;
    int ye;

    int xs23;
    int xe23;
    int ys23;
    int ye23;

    // Top triangle

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
        /* int y = ys; */
        int x = xe;

        printf("x = %d, y = %d, d = %d\n", x, ye, d);

        int c1;
        /* for (int y = ye; y < ys; ++y) { */
        /*     c1 = 1; */
        /*     while (d <= 0) { */
        /*         d = d + 2 * dy; */
        /*         c1 = c1 + 1; */
        /*     } */
        /*     d = d + (2* (dy - dx)); */
        /*     x = x - c1; */
        /*     printf("x = %d, y = %d, d = %d\n", x, ye, d); */
        /*     p[y*w + x] = color; */
        /* } */

        if (y2 > y3) {
            xs23 = x3;
            xe23 = x2;
            ys23 = y3;
            ye23 = y2;
        } else {
            xs23 = x2;
            xe23 = x3;
            ys23 = y2;
            ye23 = y3;
        }
        //  
        int dx23 = xe23 - xs23;
        int dy23 = ye23 - ys23;

        printf("test x = %d\n", x);
        
        int xi23 = 1;
        if (dx23 < 0) {
            xi23 = -1;
            dx23 = -dx23;
        }
        int d23 = (2 * dx23) - dy23;
        int x23 = xs23;

        printf("test x = %d\n", x);
        printf("x = %d, ys = %d, ye = %d\n", x, ys, ye);
        for (int y = ye; y < ys; ++y) {
            c1 = 1;
            while (d <= 0) {
                d = d + 2 * dy;
                c1 = c1 + 1;
            }
            d = d + (2* (dy - dx));
            x = x - c1;
            printf("(1) x = %d, y = %d, d = %d\n", x, ye, d);
            /* p[y*w + x] = color; */
            
            printf("(2) x23 = %d, y = %d, d23 = %d\n", x23, ye, d23);
            /* p[y*w + x23] = color; */

            for (int xx = x; xx < x23; ++xx) {
                p[y*w + xx] = color;
            }
            if (d23 > 0) {
                x23 = x23 + xi23;
                d23 = d23 + (2 * (dx23 - dy23));
            } else {
                d23 = d23 + 2 * dx23;
            }
        }

    } else {
        /* if (y1 > y2) { */
        /*     xs = x2; */
        /*     xe = x1; */
        /*     ys = y2; */
        /*     ye = y1; */
        /* } else { */
        /*     xs = x1; */
        /*     xe = x2; */
        /*     ys = y1; */
        /*     ye = y2; */
        /* } */
        /* // */  
        /* int dx = xe - xs; */
        /* int dy = ye - ys; */

        /* int xi = 1; */
        /* if (dx < 0) { */
        /*     xi = -1; */
        /*     dx = -dx; */
        /* } */
        /* int d = (2 * dx) - dy; */
        /* int x = xs; */

        /* for (int y = ys; y < ye; ++y) { */
        /*     p[y*w + x] = color; */
        /*     if (d > 0) { */
        /*         x = x + xi; */
        /*         d = d + (2 * (dx - dy)); */
        /*     } else { */
        /*         d = d + 2 * dx; */
        /*     } */
        /* } */
    /* } */
    /* for (int y = yt; y < ym; ++y) { */

    }

    int xs13;
    int xe13;
    int ys13;
    int ye13;

    // Low triangle.
    if (x1 > x3) {
        xs13 = x3;
        xe13 = x1;
        ys13 = y3;
        ye13 = y1;
    } else {
        xs13 = x1;
        xe13 = x3;
        ys13 = y1;
        ye13 = y3;
    }
    //  
    int dx13 = xe13 - xs13;
    int dy13 = ye13 - ys13;

    int yi13 = 1;
    if (dy13 < 0) {
        yi13 = -1;
        dy13 = -dy13;
    }
    int d13 = (2 * dy13) - dx13;
    /* int y = ys; */
    int xl = xe;

    /* printf("x = %d, y = %d, d = %d\n", x, ye, d); */

    int c2;
    /* for (int y = ye; y < ys; ++y) { */
    /*     c1 = 1; */
    /*     while (d <= 0) { */
    /*         d = d + 2 * dy; */
    /*         c1 = c1 + 1; */
    /*     } */
    /*     d = d + (2* (dy - dx)); */
    /*     x = x - c1; */
    /*     printf("x = %d, y = %d, d = %d\n", x, ye, d); */
    /*     p[y*w + x] = color; */
    /* } */

    /* printf("test x = %d\n", x); */
    /* printf("x = %d, ys = %d, ye = %d\n", x, ys, ye); */
    for (int y = y3; y < ye; ++y) {
        for (int xx = x13; xx < x23; ++xx) {
            p[y*w + xx] = color;
        }
        if (d13 > 0) {
            x13 = x13 + xi13;
            d13 = d13 + (2 * (dx13 - dy13));
        } else {
            d13 = d13 + 2 * dx13;
        }
        if (d23 > 0) {
            x23 = x23 + xi23;
            d23 = d23 + (2 * (dx23 - dy23));
        } else {
            d23 = d23 + 2 * dx23;
        }
    }
}

#endif
