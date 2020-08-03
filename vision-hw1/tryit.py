from uwimg import *

# Create my own Ronbledore
im_high = load_image("data/ron.png")
f_high = make_gaussian_filter(2)
im_high = sub_image(im_high, convolve_image(im_high, f_high, 1))
im_low = load_image("data/dumbledore.png")
f_low = make_gaussian_filter(3)
im_low = convolve_image(im_low, f_low, 1)
im = add_image(im_high, im_low)
clamp_image(im)
save_image(im, "ronbledore")

# Make a colorized representation
im = load_image("data/dog.jpg")
im_sobel = colorize_sobel(im)
f = make_gaussian_filter(1)
im_sobel_smooth = convolve_image(im_sobel, f, 1)
save_image(im_sobel_smooth, "sobel")
