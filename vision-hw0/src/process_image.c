#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include "image.h"

int serialize_index(image im, int x, int y, int c) {
    int w = im.w;
    int h = im.h;
    return c*h*w + y*w + x;
}

float get_pixel(image im, int x, int y, int c)
{
    // TODO Fill this in
    int idx = serialize_index(im, x, y, c);
    return im.data[idx];
}

void set_pixel(image im, int x, int y, int c, float v)
{
    // TODO Fill this in
    int idx = serialize_index(im, x, y, c);
    im.data[idx] = v;
}

image copy_image(image im)
{
    image copy = make_image(im.w, im.h, im.c);
    // TODO Fill this in
    memcpy(copy.data, im.data, im.w * im.h * im.c * sizeof(float));
    return copy;
}

image rgb_to_grayscale(image im)
{
    assert(im.c == 3);
    image gray = make_image(im.w, im.h, 1);
    // TODO Fill this in
    const float w_r = 0.299;
    const float w_g = 0.587;
    const float w_b = 0.114;

    for (int y = 0; y < im.h; y++) {
        for (int x = 0; x < im.w; x++) {
            int idx_gray = serialize_index(gray, x, y, 0);
            int idx_r = serialize_index(im, x, y, 0);
            int idx_g = serialize_index(im, x, y, 1);
            int idx_b = serialize_index(im, x, y, 2);
            gray.data[idx_gray] = w_r * im.data[idx_r] + w_g * im.data[idx_g] + w_b * im.data[idx_b];
        }
    }
    return gray;
}

void shift_image(image im, int c, float v)
{
    // TODO Fill this in
}

void clamp_image(image im)
{
    // TODO Fill this in
}


// These might be handy
float three_way_max(float a, float b, float c)
{
    return (a > b) ? ( (a > c) ? a : c) : ( (b > c) ? b : c) ;
}

float three_way_min(float a, float b, float c)
{
    return (a < b) ? ( (a < c) ? a : c) : ( (b < c) ? b : c) ;
}

void rgb_to_hsv(image im)
{
    // TODO Fill this in
}

void hsv_to_rgb(image im)
{
    // TODO Fill this in
}
