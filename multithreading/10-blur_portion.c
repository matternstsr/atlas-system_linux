#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "multithreading.h"

/**
 * apply_kernel - Apply a convolution kernel (2D filter) to a pixel.
 * @img: Source image.
 * @kernel: Convolution kernel.
 * @x: X coordinate of the pixel.
 * @y: Y coordinate of the pixel.
 *
 * Applies the kernel to the selected pixel and returns the blurred result.
 *
 * Return: The blurred pixel.
 */
pixel_t apply_kernel(const img_t *img, const kernel_t *kernel, size_t x, size_t y)
{
    size_t i, j;
    float sum_r = 0, sum_g = 0, sum_b = 0;
    size_t half_size = kernel->size / 2;
    pixel_t blurred_pixel;

    for (i = 0; i < kernel->size; i++)
    {
        for (j = 0; j < kernel->size; j++)
        {
            int img_x = (int)x + (int)i - (int)half_size;
            int img_y = (int)y + (int)j - (int)half_size;

            if (img_x < 0 || img_x >= (int)img->w || img_y < 0 || img_y >= (int)img->h)
            {
                continue; /* Skip out-of-bounds pixels */
            }

            size_t idx = img_y * img->w + img_x;
            float weight = kernel->matrix[i][j];
            sum_r += img->pixels[idx].r * weight;
            sum_g += img->pixels[idx].g * weight;
            sum_b += img->pixels[idx].b * weight;
        }
    }

    /* Normalize and clamp values */
    blurred_pixel.r = (uint8_t)fminf(fmaxf(sum_r, 0.0f), 255.0f);
    blurred_pixel.g = (uint8_t)fminf(fmaxf(sum_g, 0.0f), 255.0f);
    blurred_pixel.b = (uint8_t)fminf(fmaxf(sum_b, 0.0f), 255.0f);

    return blurred_pixel;
}
/**
 * blur_portion - Blur a portion of an image.
 * @portion: Data structure with blurring parameters.
 *
 * Applies the blurring operation to the specified portion of the image.
 */
void blur_portion(const blur_portion_t *portion)
{
    size_t x, y;

    /* Ensure the portion and images are valid */
    if (!portion || !portion->img || !portion->img_blur || !portion->kernel)
    {
        return;
    }

    /* Loop through each pixel in the portion */
    for (y = portion->y; y < portion->y + portion->h; y++)
    {
        for (x = portion->x; x < portion->x + portion->w; x++)
        {
            /* Check if the pixel is within the image bounds */
            if (x >= portion->img->w || y >= portion->img->h)
            {
                continue;
            }

            /* Apply the Gaussian kernel to the pixel */
            pixel_t blurred_pixel = apply_kernel(portion->img, portion->kernel, x, y);
            size_t idx = y * portion->img_blur->w + x;

            /* Ensure the index is within bounds for the blurred image */
            if (idx < num_pixels(portion->img_blur))
            {
                portion->img_blur->pixels[idx] = blurred_pixel;
            }
        }
    }
}
