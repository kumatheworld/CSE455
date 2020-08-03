#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include "image.h"
#define TWOPI 6.2831853

void l1_normalize(image im)
{
    // TODO
    const int im_size = im.w * im.h * im.c;
    float sum = 0;
    for (int idx = 0; idx < im_size; idx++) {
        sum += im.data[idx];
    }
    for (int idx = 0; idx < im_size; idx++) {
        im.data[idx] /= sum;
    }
}

image make_box_filter(int w)
{
    // TODO
    image im = make_image(w, w, 1);
    for (int idx = 0; idx < w * w; idx++) {
        im.data[idx] = 1;
    }
    l1_normalize(im);
    return im;
}

void convolve_image_channel(image im, image filter, image im_new, int im_c, int filter_c, int im_new_c)
{
    int idx_im_new = serialize_index(im_new, 0, 0, im_new_c);
    int y_start = -(filter.h / 2);
    int y_end = y_start + im.h;
    for (int y = y_start; y < y_end; y++) {
        int x_start = -(filter.w / 2);
        int x_end = x_start + im.w;
        for (int x = x_start; x < x_end; x++) {
            int idx_filter = serialize_index(filter, 0, 0, filter_c);
            for (int y_ = 0; y_ < filter.h; y_++) {
                for (int x_ = 0; x_ < filter.w; x_++) {
                    float im_data = get_pixel(im, x + x_, y + y_, im_c);
                    im_new.data[idx_im_new] += im_data * filter.data[idx_filter++];
                }
            }
            idx_im_new++;
        }
    }
}

image convolve_image(image im, image filter, int preserve)
{
    // TODO
    assert(filter.c == im.c || filter.c == 1);
    image im_new = make_image(im.w, im.h, preserve ? im.c : 1);
    for (int c = 0; c < im.c; c++) {
        int filter_c = filter.c == im.c ? c : 0;
        int im_new_c = preserve ? c : 0;
        convolve_image_channel(im, filter, im_new, c, filter_c, im_new_c);
    }
    return im_new;
}

image make_filter_from_array(int w, int h, float a[]) {
    image filter = make_image(w, h, 1);
    for (int idx = 0; idx < w * h; idx++) {
        filter.data[idx] = a[idx];
    }
    return filter;
}

image make_highpass_filter()
{
    // TODO
    float a[9] = {0, -1, 0, -1, 4, -1, 0, -1, 0};
    image filter = make_filter_from_array(3, 3, a);
    return filter;
}

image make_sharpen_filter()
{
    // TODO
    image filter = make_highpass_filter();
    filter.data[4] += 1;
    return filter;
}

image make_emboss_filter()
{
    // TODO
    float a[9] = {-2, -1, 0, -1, 1, 1, 0, 1, 2};
    image filter = make_filter_from_array(3, 3, a);
    return filter;
}

// Question 2.2.1: Which of these filters should we use preserve when we run our convolution and which ones should we not? Why?
// Answer: The sharpen filter and the emboss filter are the ones we should we use preserve for because they sum up to 1.

// Question 2.2.2: Do we have to do any post-processing for the above filters? Which ones and why?
// Answer: We need post-processing for the emboss filter because it is something like the edge filters, which need post-processing like non-maximum suppression.

image make_gaussian_filter(float sigma)
{
    // TODO
    const int l = (6 * sigma + 1) / 2;
    const int w = 2 * l + 1;
    const float neg_inv_2sigma2 = -1 / (2 * sigma * sigma);

    image filter = make_image(w, w, 1);
    int idx = 0;
    for (int y = -l; y <= l; y++) {
        int y2 = y * y;
        for (int x = -l; x <= l; x++) {
            int x2 = x * x;
            int r2 = x2 + y2;
            filter.data[idx++] = exp(r2 * neg_inv_2sigma2);
        }
    }

    l1_normalize(filter);
    return filter;
}

image op_image(float (*op)(float, float), image a, image b)
{
    const float w = a.w;
    const float h = a.h;
    const float c = a.c;
    const int same_shape = b.w == w && b.h == h && b.c == c;
    assert(same_shape);

    image im = make_image(w, h, c);
    for (int idx = 0; idx < w * h * c; idx++) {
        im.data[idx] = op(a.data[idx], b.data[idx]);
    }
    return im;
}

float add(float x, float y)
{
    return x + y;
}

image add_image(image a, image b)
{
    // TODO
    return op_image(add, a, b);
}

float sub(float x, float y)
{
    return x - y;
}

image sub_image(image a, image b)
{
    // TODO
    return op_image(sub, a, b);
}

image make_gx_filter()
{
    // TODO
    return make_image(1,1,1);
}

image make_gy_filter()
{
    // TODO
    return make_image(1,1,1);
}

void feature_normalize(image im)
{
    // TODO
    float min = __FLT_MAX__;
    float max = __FLT_MIN__;
    const int im_size = im.w * im.h * im.c;

    for (int idx = 0; idx < im_size; idx++) {
        float z = im.data[idx];
        max = z < max ? max : z;
        min = z > min ? min : z;
    }

    const float scale = min < max ? 1 / (max - min) : 0;
    for (int idx = 0; idx < im_size; idx++) {
        im.data[idx] = scale * (im.data[idx] - min);
    }

    min = __FLT_MAX__;
    max = __FLT_MIN__;
    for (int idx = 0; idx < im_size; idx++) {
        float z = im.data[idx];
        max = z < max ? max : z;
        min = z > min ? min : z;
    }
}

image *sobel_image(image im)
{
    // TODO
    float grad[3] = {-1, 0, 1};
    float smooth[3] = {1, 2, 1};
    image fx1 = make_filter_from_array(3, 1, grad);
    image fx2 = make_filter_from_array(1, 3, smooth);
    image fy1 = make_filter_from_array(3, 1, smooth);
    image fy2 = make_filter_from_array(1, 3, grad);

    image gx = convolve_image(convolve_image(im, fx1, 0), fx2, 0);
    image gy = convolve_image(convolve_image(im, fy1, 0), fy2, 0);

    image *res = calloc(2, sizeof(image));
    res[0] = make_image(im.w, im.h, 1);
    res[1] = make_image(im.w, im.h, 1);
    for (int idx = 0; idx < im.w * im.h; idx++) {
        float x = gx.data[idx];
        float y = gy.data[idx];
        res[0].data[idx] = hypot(x, y);
        res[1].data[idx] = atan2(y, x);
    }

    return res;
}

image colorize_sobel(image im)
{
    // TODO
    image *res = sobel_image(im);
    image mag = res[0];
    image theta = res[1];
    feature_normalize(mag);
    feature_normalize(theta);

    image im_new = make_image(im.w, im.h, 3);
    const int im_size = im.w * im.h;
    for (int idx = 0; idx < im_size; idx ++) {
        im_new.data[idx] = theta.data[idx];
        im_new.data[idx + im_size] = mag.data[idx];
        im_new.data[idx + 2 * im_size] = mag.data[idx];
    }
    clamp_image(im_new);
    hsv_to_rgb(im_new);

    return im_new;
}
