#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include "multithreading.h"

#define MAX_THREADS 16

/**
* struct thread_data_s - Data structure for thread arguments.
* @img_blur: Destination image.
* @img: Source image.
* @kernel: Convolution kernel.
* @x_start: Starting x coordinate of the portion.
* @x_end: Ending x coordinate of the portion.
* @y_start: Starting y coordinate of the portion.
* @y_end: Ending y coordinate of the portion.
*/
typedef struct thread_data_s
{
	img_t *img_blur;
	const img_t *img;
	const kernel_t *kernel;
	size_t x_start;
	size_t x_end;
	size_t y_start;
	size_t y_end;
} thread_data_t;

/**
* thread_blur_portion - Thread function to blur a portion of the image.
* @arg: Pointer to the thread data structure.
*
* Processes a portion of the image as defined by the thread data.
*
* Return: Always returns NULL.
*/
static void *thread_blur_portion(void *arg)
{
	thread_data_t *data = (thread_data_t *)arg;
	size_t x, y;

	for (y = data->y_start; y < data->y_end; y++)
	{
		for (x = data->x_start; x < data->x_end; x++)
		{
			pixel_t blurred_pixel = apply_kernel(data->img, data->kernel, x, y);
			size_t idx = y * data->img_blur->w + x;
			data->img_blur->pixels[idx] = blurred_pixel;
		}
	}
	
	return NULL;
}

/**
* blur_image - Apply Gaussian Blur to the entire image using multithreading.
* @img_blur: Destination image.
* @img: Source image.
* @kernel: Convolution kernel.
*
* Applies the Gaussian Blur to the whole image and stores the result in img_blur.
*/
void blur_image(img_t *img_blur, const img_t *img, const kernel_t *kernel)
{
	pthread_t threads[MAX_THREADS];
	thread_data_t thread_data[MAX_THREADS];
	size_t num_threads = 4; /* Default number of threads */
	size_t i;
	size_t portion_width, portion_height;
	size_t x_start, x_end, y_start, y_end;
	
	/* Calculate the portion size for each thread */
	portion_width = img->w / num_threads;
	portion_height = img->h / num_threads;

	/* Create threads and assign portions of the image to each thread */
	for (i = 0; i < num_threads; i++)
	{
		x_start = (i % num_threads) * portion_width;
		x_end = (i % num_threads == num_threads - 1) ? img->w : x_start + portion_width;
		y_start = (i / num_threads) * portion_height;
		y_end = (i / num_threads == num_threads / num_threads - 1) ? img->h : y_start + portion_height;

		thread_data[i].img_blur = img_blur;
		thread_data[i].img = img;
		thread_data[i].kernel = kernel;
		thread_data[i].x_start = x_start;
		thread_data[i].x_end = x_end;
		thread_data[i].y_start = y_start;
		thread_data[i].y_end = y_end;

		pthread_create(&threads[i], NULL, thread_blur_portion, &thread_data[i]);
	}

	/* Wait for all threads to complete */
	for (i = 0; i < num_threads; i++)
	{
		pthread_join(threads[i], NULL);
	}
}
