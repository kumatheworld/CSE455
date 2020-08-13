#include <math.h>
#include "image.h"

float get_coord_for_interp(int z, int l_old, int l_new)
{
    return (z + .5) * l_old / l_new - .5;
}

image resize(float (*interpolate)(image im, float x, float y, int c), image im, int w, int h)
{
    // TODO
    image im_new = make_image(w, h, im.c);
    int idx = 0;
    for (int c = 0; c < im.c; c++) {
        for (int y = 0; y < h; y++) {
            float y_ = get_coord_for_interp(y, im.h, h);
            for (int x = 0; x < w; x++) {
                float x_ = get_coord_for_interp(x, im.w, w);
                im_new.data[idx++] = interpolate(im, x_, y_, c);
            }
        }
    }
    return im_new;
}

float nn_interpolate(image im, float x, float y, int c)
{
    // TODO Fill in
    int xi = round(x);
    int yi = round(y);
    return get_pixel(im, xi, yi, c);
}

image nn_resize(image im, int w, int h)
{
    // TODO Fill in (also fix that first line)
    return resize(*nn_interpolate, im, w, h);
}

float bilinear_interpolate(image im, float x, float y, int c)
{
    // TODO
    if (x < 0) {
        x = -1;
    }
    if (y < 0) {
        y = -1;
    }

    int xi = (int)x;
    int yi = (int)y;
    float tl = get_pixel(im, xi, yi, c);
    float tr = get_pixel(im, xi + 1, yi, c);
    float bl = get_pixel(im, xi, yi + 1, c);
    float br = get_pixel(im, xi + 1, yi + 1, c);

    float x_frac = x - xi;
    float y_frac = y - yi;
    float top = (1 - x_frac) * tl + x_frac * tr;
    float bottom = (1 - x_frac) * bl + x_frac * br;
    float middle = (1 - y_frac) * top + y_frac * bottom;

    return middle;
}

image bilinear_resize(image im, int w, int h)
{
    // TODO
    return resize(*bilinear_interpolate, im, w, h);
}

