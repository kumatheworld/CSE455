#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include "image.h"
#include "matrix.h"
#include <time.h>

// Frees an array of descriptors.
// descriptor *d: the array.
// int n: number of elements in array.
void free_descriptors(descriptor *d, int n)
{
    int i;
    for(i = 0; i < n; ++i){
        free(d[i].data);
    }
    free(d);
}

// Create a feature descriptor for an index in an image.
// image im: source image.
// int i: index in image for the pixel we want to describe.
// returns: descriptor for that index.
descriptor describe_index(image im, int i)
{
    int w = 5;
    descriptor d;
    d.p.x = i%im.w;
    d.p.y = i/im.w;
    d.data = calloc(w*w*im.c, sizeof(float));
    d.n = w*w*im.c;
    int c, dx, dy;
    int count = 0;
    // If you want you can experiment with other descriptors
    // This subtracts the central value from neighbors
    // to compensate some for exposure/lighting changes.
    for(c = 0; c < im.c; ++c){
        float cval = im.data[c*im.w*im.h + i];
        for(dx = -w/2; dx < (w+1)/2; ++dx){
            for(dy = -w/2; dy < (w+1)/2; ++dy){
                float val = get_pixel(im, i%im.w+dx, i/im.w+dy, c);
                d.data[count++] = cval - val;
            }
        }
    }
    return d;
}

// Marks the spot of a point in an image.
// image im: image to mark.
// ponit p: spot to mark in the image.
void mark_spot(image im, point p)
{
    int x = p.x;
    int y = p.y;
    int i;
    for(i = -9; i < 10; ++i){
        set_pixel(im, x+i, y, 0, 1);
        set_pixel(im, x, y+i, 0, 1);
        set_pixel(im, x+i, y, 1, 0);
        set_pixel(im, x, y+i, 1, 0);
        set_pixel(im, x+i, y, 2, 1);
        set_pixel(im, x, y+i, 2, 1);
    }
}

// Marks corners denoted by an array of descriptors.
// image im: image to mark.
// descriptor *d: corners in the image.
// int n: number of descriptors to mark.
void mark_corners(image im, descriptor *d, int n)
{
    int i;
    for(i = 0; i < n; ++i){
        mark_spot(im, d[i].p);
    }
}

// Creates a 1d Gaussian filter.
// float sigma: standard deviation of Gaussian.
// returns: single row image of the filter.
image make_1d_gaussian(float sigma, int x)
{
    // TODO: optional, make separable 1d Gaussian.
    const int l = (6 * sigma + 1) / 2;
    const int w = 2 * l + 1;
    const float neg_inv_2sigma2 = -1 / (2 * sigma * sigma);

    image filter;
    if (x) {
        filter = make_image(1, w, 1);
    } else {
        filter = make_image(w, 1, 1);
    }
    int idx = 0;
    for (int x = -l; x <= l; x++) {
        int x2 = x * x;
        filter.data[idx++] = exp(x2 * neg_inv_2sigma2);
    }

    l1_normalize(filter);
    return filter;
}

// Smooths an image using separable Gaussian filter.
// image im: image to smooth.
// float sigma: std dev. for Gaussian.
// returns: smoothed image.
image smooth_image(image im, float sigma)
{
    if (0) {
        image g = make_gaussian_filter(sigma);
        image s = convolve_image(im, g, 1);
        free_image(g);
        return s;
    } else {
        // TODO: optional, use two convolutions with 1d gaussian filter.
        // If you implement, disable the above if check.
        image g1 = make_1d_gaussian(sigma, 1);
        image s1 = convolve_image(im, g1, 1);
        free_image(g1);
        image g2 = make_1d_gaussian(sigma, 0);
        image s2 = convolve_image(s1, g2, 1);
        free_image(g2);
        free_image(s1);
        return s2;
    }
}

// Calculate the structure matrix of an image.
// image im: the input image.
// float sigma: std dev. to use for weighted sum.
// returns: structure matrix. 1st channel is Ix^2, 2nd channel is Iy^2,
//          third channel is IxIy.
image structure_matrix(image im, float sigma)
{
    // TODO: calculate structure matrix for im.
    image Ix = apply_sobel(im, 1);
    image Iy = apply_sobel(im, 0);

    const im_size = im.w * im.h;
    image I2 = make_image(im.w, im.h, 3);
    for (int idx = 0; idx < im_size; idx++) {
        float x = Ix.data[idx];
        float y = Iy.data[idx];
        I2.data[idx] = x * x;
        I2.data[idx + im_size] = y * y;
        I2.data[idx + 2 * im_size] = x * y;
    }
    free_image(Ix);
    free_image(Iy);

    image S = smooth_image(I2, sigma);
    free_image(I2);

    return S;
}

// Estimate the cornerness of each pixel given a structure matrix S.
// image S: structure matrix for an image.
// returns: a response map of cornerness calculations.
image cornerness_response(image S)
{
    image R = make_image(S.w, S.h, 1);
    // TODO: fill in R, "cornerness" for each pixel using the structure matrix.
    // We'll use formulation det(S) - alpha * trace(S)^2, alpha = .06.
    const int im_size = S.w * S.h;
    const float alpha = .06;
    for (int idx = 0; idx < im_size; idx++) {
        float xx = S.data[idx];
        float yy = S.data[idx + im_size];
        float xy = S.data[idx + 2 * im_size];

        float det = xx * yy - xy * xy;
        float tr = xx + yy;
        R.data[idx] = det - alpha * tr * tr;
    }
    return R;
}

// Perform non-max supression on an image of feature responses.
// image im: 1-channel image of feature responses.
// int w: distance to look for larger responses.
// returns: image with only local-maxima responses within w pixels.
image nms_image(image im, int w)
{
    image r = copy_image(im);
    // TODO: perform NMS on the response map.
    // for every pixel in the image:
    //     for neighbors within w:
    //         if neighbor response greater than pixel response:
    //             set response to be very low (I use -999999 [why not 0??])
    const float very_low = -999999;
    for (int y = 0; y < im.h; y++) {
        for (int x = 0; x < im.w; x++) {
            float maximum = __FLT_MIN__;
            for (int y_ = y - w; y_ <= y + w; y_++) {
                for (int x_ = x - w; x_ <= x + w; x_++) {
                    float val = get_pixel(im, x_, y_, 0);
                    if (maximum < val) {
                        maximum = val;
                    }
                }
            }
            if (get_pixel(im, x, y, 0) < maximum) {
                set_pixel(r, x, y, 0, very_low);
            }
        }
    }
    return r;
}

// Perform harris corner detection and extract features from the corners.
// image im: input image.
// float sigma: std. dev for harris.
// float thresh: threshold for cornerness.
// int nms: distance to look for local-maxes in response map.
// int *n: pointer to number of corners detected, should fill in.
// returns: array of descriptors of the corners in the image.
descriptor *harris_corner_detector(image im, float sigma, float thresh, int nms, int *n)
{
    // Calculate structure matrix
    image S = structure_matrix(im, sigma);

    // Estimate cornerness
    image R = cornerness_response(S);

    // Run NMS on the responses
    image Rnms = nms_image(R, nms);


    //TODO: count number of responses over threshold
    int count = 0; // change this
    for (int idx = 0; idx < im.w * im.h; idx++) {
        if (Rnms.data[idx] > thresh) {
            count++;
        }
    }


    *n = count; // <- set *n equal to number of corners in image.
    descriptor *d = calloc(count, sizeof(descriptor));
    //TODO: fill in array *d with descriptors of corners, use describe_index.
    count = 0;
    for (int idx = 0; idx < im.w * im.h; idx++) {
        if (Rnms.data[idx] > thresh) {
            d[count++] = describe_index(R, idx);
        }
    }

    free_image(S);
    free_image(R);
    free_image(Rnms);
    return d;
}

// Find and draw corners on an image.
// image im: input image.
// float sigma: std. dev for harris.
// float thresh: threshold for cornerness.
// int nms: distance to look for local-maxes in response map.
void detect_and_draw_corners(image im, float sigma, float thresh, int nms)
{
    int n = 0;
    descriptor *d = harris_corner_detector(im, sigma, thresh, nms, &n);
    mark_corners(im, d, n);
}
