#ifndef MULTITHREADING_H
#define MULTITHREADING_H

#include <stddef.h>
#include <stdint.h>
#include <pthread.h>

#define num_pix(img) ((img)->w * (img)->h)
#define CLAMP(value, min_val, max_val) ((value) < (min_val) ? (min_val) : ((value) > (max_val) ? (max_val) : (value)))



/**
* struct pixel_s - RGB pixel
*
* @r: Red component
* @g: Green component
* @b: Blue component
*/
typedef struct pixel_s
{
	uint8_t r;
	uint8_t g;
	uint8_t b;
} pixel_t;

/**
* struct img_s - Image
*
* @w:      Image width
* @h:      Image height
* @pixels: Array of pixels
*/
typedef struct img_s
{
	size_t w;
	size_t h;
	pixel_t *pixels;
} img_t;

/**
* struct kernel_s - Convolution kernel
*
* @size:   Size of the matrix (both width and height)
* @matrix: Kernel matrix
*/
typedef struct kernel_s
{
	size_t size;
	float **matrix;
} kernel_t;

/**
* struct blur_portion_s - Information needed to blur a portion of an image
*
* @img:      Source image
* @img_blur: Destination image
* @x:        X position of the portion in the image
* @y:        Y position of the portion in the image
* @w:        Width of the portion
* @h:        Height of the portion
* @kernel:   Convolution kernel to use
*/
typedef struct blur_portion_s
{
	img_t const *img;
	img_t *img_blur;
	size_t x;
	size_t y;
	size_t w;
	size_t h;
	kernel_t const *kernel;
} blur_portion_t;

/**
 * thread_entry - Entry point for the thread
 * @arg: Argument passed to the thread
 *
 * Return: NULL
 */
void *thread_entry(void *arg);

/**
 * tprintf - Print a formatted string with the thread ID
 * @format: Format string
 *
 * Return: The number of characters printed (same as `printf`)
 */
int tprintf(char const *format, ...);

/**
 * apply_kernel - Apply the convolution kernel to a pixel in the image
 * @img: Source image
 * @kernel: Convolution kernel
 * @x: X coordinate of the pixel to blur
 * @y: Y coordinate of the pixel to blur
 *
 * Return: The blurred pixel
 */
/* pixel_t apply_kernel(img_t const *img, kernel_t const *kernel, size_t x, size_t y);*/
pixel_t apply_kernel(const img_t *img, const kernel_t *kernel, size_t x, size_t y);

/**
 * blur_portion - Blur a portion of an image
 * @portion: Information needed to blur a portion of an image
 */
void blur_portion(blur_portion_t const *portion);

/**
 * blur_image - Blur the entire image using Gaussian Blur with multithreading
 * @img_blur: Destination image
 * @img: Source image
 * @kernel: Convolution kernel
 */
void blur_image(img_t *img_blur, img_t const *img, kernel_t const *kernel);

#endif /* MULTITHREADING_H */
