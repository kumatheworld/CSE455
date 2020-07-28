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
    return make_image(1,1,1);
}

image add_image(image a, image b)
{
    // TODO
    return make_image(1,1,1);
}

image sub_image(image a, image b)
{
    // TODO
    return make_image(1,1,1);
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
}

image *sobel_image(image im)
{
    // TODO
    return calloc(2, sizeof(image));
}

image colorize_sobel(image im)
{
    // TODO
    return make_image(1,1,1);
}
