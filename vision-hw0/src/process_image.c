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
    const int im_size = im.w * im.h;
    for (int idx = 0; idx < im_size; idx++) {
        int idx_r = idx;
        int idx_g = idx + im_size;
        int idx_b = idx + 2 * im_size;
        float R = im.data[idx_r];
        float G = im.data[idx_g];
        float B = im.data[idx_b];
        float H, S, V;

        V = three_way_max(R, G, B);

        float m = three_way_min(R, G, B);
        float C = V - m;
        if (C <= 0) {
            S = 0;
            H = 0;
        } else {
            S = C / V;
            float H_;

            if (V == R) {
                H_ = (G - B) / C;
            } else if (V == G) {
                H_ = (B - R) / C + 2;
            } else  {
                H_ = (R - G) / C + 4;
            }

            if (H_ < 0) {
                H = H_ / 6  + 1;
            } else {
                H = H_ / 6;
            }
        }

        im.data[idx_r] = H;
        im.data[idx_g] = S;
        im.data[idx_b] = V;
    }
}

void hsv_to_rgb(image im)
{
    // TODO Fill this in
    const int im_size = im.w * im.h;
    for (int idx = 0; idx < im_size; idx++) {
        int idx_r = idx;
        int idx_g = idx + im_size;
        int idx_b = idx + 2 * im_size;
        float H = im.data[idx_r];
        float S = im.data[idx_g];
        float V = im.data[idx_b];
        float R, G, B;

        float H_ = 6 * H;
        if (H_ > 5) {
            H_ -= 6;
        }
        float C = S * V;
        float m = V - C;
        if (H_ <= 1) {
            R = V;
            if (H_ < 0) {
                G = m;
                B = G - H_ * C;
            } else {
                B = m;
                G = B + H_ * C;
            }
        } else if (H_ <= 3) {
            H_ -= 2;
            G = V;
            if (H_ < 0) {
                B = m;
                R = B - H_ * C;
            } else {
                R = m;
                B = R + H_ * C;
            }
        } else {
            H_ -= 4;
            B = V;
            if (H_ < 0) {
                R = m;
                G = R - H_ * C;
            } else {
                G = m;
                R = G + H_ * C;
            }
        }

        im.data[idx_r] = R;
        im.data[idx_g] = G;
        im.data[idx_b] = B;
    }
}
