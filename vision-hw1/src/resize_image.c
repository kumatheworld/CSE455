#include <math.h>
#include "image.h"

float get_coord_for_interp(int z, int l_old, int l_new) {
    return (z + .5) * l_old / l_new - .5;
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
    return make_image(1,1,1);
}

float bilinear_interpolate(image im, float x, float y, int c)
{
    // TODO
    return 0;
}

image bilinear_resize(image im, int w, int h)
{
    // TODO
    return make_image(1,1,1);
}

