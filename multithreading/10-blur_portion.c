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
 */
static void apply_gaussian_blur(img_t const *img, img_t *img_blur, kernel_t const *kernel,
    size_t x_start, size_t y_start, size_t width, size_t height)
{
    size_t ki, kj;
    size_t kernel_half_size = kernel->size / 2;
    pixel_t *pixels = img->pixels;
    pixel_t *pixels_blur = img_blur->pixels;

    for (size_t y = y_start; y < y_start + height; y++)
	{ /* Looping Over Pixels: */
        for (size_t x = x_start; x < x_start + width; x++)
		{
            float r = 0, g = 0, b = 0;
            float weight_sum = 0.0;

            for (ki = 0; ki < kernel->size; ki++)
			{
                for (kj = 0; kj < kernel->size; kj++)
				{ /* Applying the Kernel: */
                    size_t pixel_x = x + ki - kernel_half_size;
                    size_t pixel_y = y + kj - kernel_half_size;
					/* Boundary Check for Pixel Indices: */
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
			/* Updating the Blurred Image */
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

typedef struct {
    img_t const *img;
    img_t *img_blur;
    kernel_t const *kernel;
    size_t x_start;
    size_t y_start;
    size_t width;
    size_t height;
} blur_task_t;

void *thread_apply_gaussian_blur(void *arg) {
    blur_task_t *task = (blur_task_t *)arg;
    apply_gaussian_blur(
        task->img,
        task->img_blur,
        task->kernel,
        task->x_start,
        task->y_start,
        task->width,
        task->height
    );
    return NULL;
}

void blur_image(img_t *img_blur, img_t const *img, kernel_t const *kernel) {
    /* Define number of threads and tasks */
    int num_threads = 4; /* Adjust as needed */
    pthread_t threads[num_threads];
    blur_task_t tasks[num_threads];

    /* Divide the image into portions for each thread */
    size_t portion_height = img->h / num_threads;
    for (int i = 0; i < num_threads; i++) {
        tasks[i].img = img;
        tasks[i].img_blur = img_blur;
        tasks[i].kernel = kernel;
        tasks[i].x_start = 0;
        tasks[i].y_start = i * portion_height;
        tasks[i].width = img->w;
        tasks[i].height = (i == num_threads - 1) ? (img->h - (portion_height * (num_threads - 1))) : portion_height;

        pthread_create(&threads[i], NULL, thread_apply_gaussian_blur, &tasks[i]);
    }

    /* Wait for all threads to finish */
    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }
}