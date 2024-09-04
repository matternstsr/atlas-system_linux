#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "multithreading.h"

typedef struct {
    uint8_t r, g, b;
} pixel_t;

typedef struct {
    size_t w, h;
    pixel_t *pixels;
} img_t;

typedef struct {
    size_t size;
    float **matrix;
} kernel_t;

/**
 * apply_kernel - Apply a convolution kernel to a pixel.
 * @img: Source image.
 * @kernel: Convolution kernel.
 * @x: X coordinate of the pixel.
 * @y: Y coordinate of the pixel.
 *
 * Return: The blurred pixel.
 */
pixel_t apply_kernel(const img_t *img, const kernel_t *kernel, size_t x, size_t y) {
    size_t i, j;
    float sum_r = 0, sum_g = 0, sum_b = 0;
    size_t half_size = kernel->size / 2;
    pixel_t blurred_pixel;

    for (i = 0; i < kernel->size; i++) {
        for (j = 0; j < kernel->size; j++) {
            int img_x = (int)x + i - half_size;
            int img_y = (int)y + j - half_size;

            /* Skip out-of-bounds pixels */
            if (img_x < 0 || img_x >= (int)img->w || img_y < 0 || img_y >= (int)img->h) {
                continue;
            }

            size_t idx = img_y * img->w + img_x;
            float weight = kernel->matrix[i][j];
            sum_r += img->pixels[idx].r * weight;
            sum_g += img->pixels[idx].g * weight;
            sum_b += img->pixels[idx].b * weight;
        }
    }

    /* Clamp values */
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

	/* Ensure the portion and images are real */
	if (!portion || !portion->img || !portion->img_blur || !portion->kernel)
	{
		return;
	}

	/* Loop through each pixel in the portion */
	for (y = portion->y; y < portion->y + portion->h; y++)
	{
		for (x = portion->x; x < portion->x + portion->w; x++)
		{
			/* Apply the Gaussian kernel to the pixel ???? */
			pixel_t blurred_pixel = apply_kernel(portion->img, portion->kernel, x, y);
			size_t idx = y * portion->img_blur->w + x;
			portion->img_blur->pixels[idx] = blurred_pixel;
		}
	}
}
