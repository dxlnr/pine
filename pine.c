#ifndef PINE 
#define PINE 

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <errno.h>
#include <math.h>

double rlt(double r) { return 1.0014 + 0.0086 * r - 1.4886 * pow(r, 2) + 0.5344 * pow(r, 3); }

int min(int a, int b) { return (a > b) ? b : a; }
int max(int a, int b) { return (a > b) ? a : b; }

typedef struct {
    uint32_t *pixels;
    size_t width;
    size_t height;
    /* size_t stride; */
} Pine_Canvas;

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
 * renders a triangle with specific color.
 */
void fill_triangle(uint32_t *p, size_t h, size_t w,
               int x1, int y1,
               int x2, int y2,
               int x3, int y3,
               uint32_t color) 
{ 
    int xt, yt;
    int xm, ym;
    int xd, yd;

    bool f1 = false;
    bool f2 = false;
    // define the top point.
    if (y1 < y2 && y1 < y3) {
        yt = y1;
        xt = x1;
        f1 = true;
    } else if (y2 < y1 && y2 < y3) {
        yt = y2;
        xt = x2;
        f2 = true;
    } else {
        yt = y3;
        xt = x3;
    }
    // Define the lowest point.
    if (y1 > y2 && y1 > y3 && !f1) {
        yd = y1;
        xd = x1;
        f1 = true;
    } else if (y2 > y1 && y2 > y3 && !f2 ) {
        yd = y2;
        xd = x2;
        f2 = true;
    } else {
        yd = y3;
        xd = x3;
    }
    /* // Define the middle left point. */
    if (!f1) { 
        xm = x1;
        ym = y1;
    } else if (!f2) {
        xm = x2;
        ym = y2;
    } else {
        xm = x3;
        ym = y3;
    }

    int xstm;
    int xetm;
    int ystm;
    int yetm;
    int xstd;
    int xetd;
    int ystd;
    int yetd;
    int xsmd;
    int xemd;
    int ysmd;
    int yemd;

    // Top triangle
    int ctm;
    int dtm;
    int xtm;
    int dytm;
    int dxtm;
    int xitm;

    int ctd;
    int dtd;
    int xtd;
    int dytd;
    int dxtd;
    int xitd;

    // Line from top to left middle.  
    if (abs(yt - ym) < abs(xt - xm)) {
        if (xm > xt) {
            xstm = xt;
            xetm = xm;
            ystm = yt;
            yetm = ym;
        } else {
            xstm = xm;
            xetm = xt;
            ystm = ym;
            yetm = yt;
        }
        dxtm = xetm - xstm;
        dytm = yetm - ystm;

        if (dytm < 0) { dytm = -dytm; }
        
        dtm = (2 * dytm) - dxtm;
    } else {
        if (ym > yt) {
            xstm = xt;
            xetm = xm;
            ystm = yt;
            yetm = ym;
        } else {
            xstm = xm;
            xetm = xt;
            ystm = ym;
            yetm = yt;
        }
        dxtm = xetm - xstm;
        dytm = yetm - ystm;

        xitm = 1;
        if (dxtm < 0) {
            xitm = -1;
            dxtm = -dxtm;
        }
        dtm = (2 * dxtm) - dytm;
    }
    xtm = xt;

    // Line from top to right middle (down point is used as reference).
    if (abs(yt - yd) < abs(xt - xd)) {
        if (xd > xt) {
            xstd = xt;
            xetd = xd;
            ystd = yt;
            yetd = yd;
        } else {
            xstd = xd;
            xetd = xt;
            ystd = yd;
            yetd = yt;
        }
        dxtd = xetd - xstd;
        dytd = yetd - ystd;

        if (dytd < 0) { dytd = -dytd; }

        dtd = (2 * dytd) - dxtd;
    } else {
        if (yd > yt) {
            xstd = xt;
            xetd = xd;
            ystd = yt;
            yetd = yd;
        } else {
            xstd = xd;
            xetd = xt;
            ystd = yd;
            yetd = yt;
        }
        dxtd = xetd - xstd;
        dytd = yetd - ystd;

        xitd = 1;
        if (dxtd < 0) {
            xitd = -1;
            dxtd = -dxtd;
        }
        dtd = (2 * dxtd) - dytd;
    }
    xtd = xt;

    // Main Loop for filling the top triangle.
    for (int y = yt; y <= ym; ++y) {
        if (xm > xd) {
            for (int x = xtd; x < xtm; ++x) {
                p[y*w + x] = color;
            }
        } else {
            for (int x = xtm; x < xtd; ++x) {
                p[y*w + x] = color;
            }
        }
        // top middle left
        if (abs(yt - ym) < abs(xt - xm)) {
            ctm = 1;
            while (dtm <= 0) {
                dtm = dtm + 2 * dytm;
                ctm = ctm + 1;
            }
            dtm = dtm + (2* (dytm - dxtm));
            if (xm > xd) {
                xtm = xtm + ctm;
            } else {
                xtm = xtm - ctm;
            }
        } else {
            if (dtm > 0) {
                xtm = xtm + xitm;
                dtm = dtm + (2 * (dxtm - dytm));
            } else {
                dtm = dtm + 2 * dxtm;
            }
        }
        // top middle right.
        if (abs(yt - yd) < abs(xt - xd)) {
            ctd = 1;
            while (dtd <= 0) {
                dtd = dtd + 2 * dytd;
                ctd = ctd + 1;
            }
            dtd = dtd + (2* (dytd - dxtd));
            if (xm > xd) {
                xtd = xtd - ctd;
            } else {
                xtd = xtd + ctd;
            }
        } else {
            if (dtd > 0) {
                xtd = xtd + xitd;
                dtd = dtd + (2 * (dxtd - dytd));
            } else {
                dtd = dtd + 2 * dxtd;
            }
        }
    }

    // Low triangle.
    int cmd;
    int dmd;
    int xmd;
    int dymd;
    int dxmd;
    int ximd;

    // Line from bottom to left middle.  
    if (abs(yd - ym) < abs(xd - xm)) {
        if (xm > xd) {
            xsmd = xd;
            xemd = xm;
            ysmd = yd;
            yemd = ym;
        } else {
            xsmd = xm;
            xemd = xd;
            ysmd = ym;
            yemd = yd;
        }
        dxmd = xemd - xsmd;
        dymd = yemd - ysmd;

        if (dymd < 0) { dymd = -dymd; }
        
        dmd = (2 * dymd) - dxmd;
    } else {
        if (ym > yd) {
            xsmd = xd;
            xemd = xm;
            ysmd = yd;
            yemd = ym;
        } else {
            xsmd = xm;
            xemd = xd;
            ysmd = ym;
            yemd = yd;
        }
        dxmd = xemd - xsmd;
        dymd = yemd - ysmd;

        ximd = -1;
        if (dxmd < 0) {
            ximd = 1;
            dxmd = -dxmd;
        }
        dmd = (2 * dxmd) - dymd;
    }
    xmd = xd;
    // Line from bottom to right middle (top point is used as reference).
    if (abs(yt - yd) >= abs(xt - xd)) {
        xitd = 1;
        if (dxtd < 0) {
            xitd = -1;
            dxtd = -dxtd;
        }
    }

    xtd = xd;
    
    // Main Loop for filling the lower triangle.
    for (int y = yd; y > ym; --y) {
        if (xm > xd) {
            for (int x = xtd; x < xmd; ++x) {
                p[y*w + x] = color;
            }
        } else {
            for (int x = xmd; x < xtd; ++x) {
                p[y*w + x] = color;
            }
        }
        // top middle left
        if (abs(ym - yd) < abs(xm - xd)) {
            cmd = 1;
            while (dmd <= 0) {
                dmd = dmd + 2 * dymd;
                cmd = cmd + 1;
            }
            dmd = dmd + (2* (dymd - dxmd));
            if (xm > xd) {
                xmd = xmd + cmd;
            } else {
                xmd = xmd - cmd;
            }
        } else {
            if (dmd > 0) {
                xmd = xmd + ximd;
                dmd = dmd + (2 * (dxmd - dymd));
            } else {
                dmd = dmd + 2 * dxmd;
            }
        }
        // top middle right.
        if (abs(yt - yd) < abs(xt - xd)) {
            ctd = 1;
            while (dtd <= 0) {
                dtd = dtd + 2 * dytd;
                ctd = ctd + 1;
            }
            dtd = dtd + (2* (dytd - dxtd));
            if (xm > xd) {
                xtd = xtd + ctd;
            } else {
                xtd = xtd - ctd;
            }
        } else {
            if (dtd > 0) {
                xtd = xtd + xitd;
                dtd = dtd + (2 * (dxtd - dytd));
            } else {
                dtd = dtd + 2 * dxtd;
            }
        }
    }
}

#endif
