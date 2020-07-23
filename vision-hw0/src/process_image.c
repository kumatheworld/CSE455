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

int valid_idx(int idx, int length) {
    return 0 <= idx && idx < length;
}

int clamp(int x, int left, int right) {
    if (x < left) {
        return left;
    } else if (x >= right) {
        return right - 1;
    } else {
        return x;
    }
}

float get_pixel(image im, int x, int y, int c)
{
    // TODO Fill this in
    x = clamp(x, 0, im.w);
    y = clamp(y, 0, im.h);
    int idx = serialize_index(im, x, y, c);
    return im.data[idx];
}

void set_pixel(image im, int x, int y, int c, float v)
{
    // TODO Fill this in
    if (valid_idx(x, im.w) && valid_idx(y, im.h) && valid_idx(c, im.c)) {
        int idx = serialize_index(im, x, y, c);
        im.data[idx] = v;
    }
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
    const int im_size = im.w * im.h;

    for (int idx = 0; idx < im_size; idx++) {
        int idx_r = idx;
        int idx_g = idx + im_size;
        int idx_b = idx + 2 * im_size;
        gray.data[idx] = w_r * im.data[idx_r] + w_g * im.data[idx_g] + w_b * im.data[idx_b];
    }
    return gray;
}

void shift_image(image im, int c, float v)
{
    // TODO Fill this in
    const int im_size = im.w * im.h;
    const int start = im_size * c;
    const int end = start + im_size;

    for (int idx = start; idx < end; idx++) {
        im.data[idx] += v;
    }
}

float clamp01(float x) {
    if (x < 0) {
        return 0;
    } else if (x > 1) {
        return 1;
    } else {
        return x;
    }
}

void clamp_image(image im)
{
    // TODO Fill this in
    for (int idx = 0; idx < im.w * im.h * im.c; idx++) {
        im.data[idx] = clamp01(im.data[idx]);
    }
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
