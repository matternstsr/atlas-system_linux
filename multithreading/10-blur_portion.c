#include "multithreading.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

/**
 * apply_gaussian_blur - Apply a Gaussian blur to a rect portion of an image.
 * @img: Ptr to the original image struct cont the pixel data to be blurred.
 * @img_blur: Ptr to the image struct where the blurred res will be stored.
 * @kernel: Pointer to the Gaussian kernel used for the blurring operation.
 * @x_start: The x-cord of top-left corner of portion of image to be blurred.
 * @y_start: The y-cord of top-left corner of portion of image to be blurred.
 * @width: The width of the portion of the image to be blurred.
 * @height: The height of the portion of the image to be blurred.
 *
 * The func applies a Gaussian blur to a specified rectangular region of image
 * defined by the coordinates (x_start, y_start) & dimensions (width, height).
 * The blurring is performed using the specified kernel, and resulting blurred
 * image is stored in the `img_blur` structure.
 *
 * The Gaussian blur is computed by averaging pixel values weighted by  kernel
 * values, ensuring that pixels near center of kernel have a higher influence
 * on the final result than those further away.
 */
static void apply_gaussian_blur(img_t const *img, img_t *img_blur, kernel_t const *kernel,
    size_t x_start, size_t y_start, size_t width, size_t height)
{
    size_t ki, kj;
    size_t kernel_half_size = kernel->size / 2;
    pixel_t *pixels = img->pixels;
    pixel_t *pixels_blur = img_blur->pixels;

    for (size_t y = y_start; y < y_start + height; y++)
	{
        for (size_t x = x_start; x < x_start + width; x++)
		{
            float r = 0, g = 0, b = 0;
            float weight_sum = 0.0;

            for (ki = 0; ki < kernel->size; ki++)
			{
                for (kj = 0; kj < kernel->size; kj++)
				{
                    size_t pixel_x = x + ki - kernel_half_size;
                    size_t pixel_y = y + kj - kernel_half_size;

                    if (pixel_x < img->w && pixel_y < img->h)
					{
                        size_t pixel_index = pixel_y * img->w + pixel_x;
                        float weight = kernel->matrix[ki][kj];
                        r += pixels[pixel_index].r * weight;
                        g += pixels[pixel_index].g * weight;
                        b += pixels[pixel_index].b * weight;
                        weight_sum += weight;
                    }
                }
            }

            size_t pixel_index = y * img->w + x;
            pixels_blur[pixel_index].r = (char)(r / weight_sum);
            pixels_blur[pixel_index].g = (char)(g / weight_sum);
            pixels_blur[pixel_index].b = (char)(b / weight_sum);
        }
    }
}

/**
 * blur_portion - Apply a Gaussian blur to a specified portion of an image.
 * @portion: A pointer to a `blur_portion_t` structure containing details about
 * the portion of image to be blurred.
 *
 * This includes the original image,
 * the image to store the blurred result, the convolution kernel, and the
 * coordinates and dimensions of the portion to be blurred.
 *
 * The function applies a Gaussian blur to the specified rectangular area of
 * image using the provided kernel. The resulting blurred image is stored in 
 * `img_blur` field of the `portion` structure.
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
