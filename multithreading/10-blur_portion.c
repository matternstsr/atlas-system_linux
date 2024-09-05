#include "multithreading.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

/* Forward declaration of is_valid_pixel */
static int is_valid_pixel(img_t const *img, size_t x, size_t y);

/**
 * apply_gaussian_blur - Apply a Gaussian blur to a rect portion of an image.
 * @img: Ptr to the original image struct containing the pixel data to be blurred.
 * @img_blur: Ptr to the image struct where the blurred result will be stored.
 * @kernel: Pointer to the Gaussian kernel used for the blurring operation.
 * @x_start: The x-coordinate of the top-left corner of the portion of the image to be blurred.
 * @y_start: The y-coordinate of the top-left corner of the portion of the image to be blurred.
 * @width: The width of the portion of the image to be blurred.
 * @height: The height of the portion of the image to be blurred.
 *
 */
static void apply_gaussian_blur(img_t const *img, img_t *img_blur,
	kernel_t const *kernel, size_t x_start, size_t y_start, size_t width,
	size_t height)
{
    size_t ki, kj;
    size_t kernel_half_size;
    pixel_t *pixels;
    pixel_t *pixels_blur;
    size_t pixel_x, pixel_y, pixel_index;
    float r, g, b, weight_sum, weight;

    kernel_half_size = kernel->size / 2;
    pixels = img->pixels;
    pixels_blur = img_blur->pixels;

    for (size_t y = y_start; y < y_start + height; y++)
    { /* Looping Over Pixels: */
        for (size_t x = x_start; x < x_start + width; x++)
        {
            r = 0;
            g = 0;
            b = 0;
            weight_sum = 0.0;

            for (ki = 0; ki < kernel->size; ki++)
            {
                for (kj = 0; kj < kernel->size; kj++)
                { /* Applying the Kernel: */
                    pixel_x = x + ki - kernel_half_size;
                    pixel_y = y + kj - kernel_half_size;

                    if (is_valid_pixel(img, pixel_x, pixel_y))
                    {
                        pixel_index = pixel_y * img->w + pixel_x;
                        weight = kernel->matrix[ki][kj];
                        r += pixels[pixel_index].r * weight;
                        g += pixels[pixel_index].g * weight;
                        b += pixels[pixel_index].b * weight;
                        weight_sum += weight;
                    }
                }
            }
            /* Updating the Blurred Image */
            pixel_index = y * img->w + x;
            pixels_blur[pixel_index].r = (char)(r / weight_sum);
            pixels_blur[pixel_index].g = (char)(g / weight_sum);
            pixels_blur[pixel_index].b = (char)(b / weight_sum);
        }
    }
}

/**
 * is_valid_pixel - checks if the given pixel coordinates are within the image bounds.
 *
 * @img: Pointer to the image structure.
 * @x: x-coordinate of the pixel.
 * @y: y-coordinate of the pixel.
 * Return: 1 if the pixel is within bounds, 0 otherwise.
 */
static int is_valid_pixel(img_t const *img, size_t x, size_t y)
{
    if (x < img->w && y < img->h)
    {
        return 1;
    }
    return 0;
}

/**
 * blur_portion - Apply a Gaussian blur to a specified portion of an image.
 * @portion: A pointer to a `blur_portion_t` structure containing details about
 * the portion of the image to be blurred.
 *
 */
void blur_portion(blur_portion_t const *portion)
{
    apply_gaussian_blur(
        portion->img,
        portion->img_blur,
        portion->kernel,
        portion->x,
        portion->y,
        portion->w,
        portion->h
    );
}
