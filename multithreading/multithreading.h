#ifndef MULTITHREADING_H
#define MULTITHREADING_H

#include <stddef.h>
#include <stdint.h>  /* For uintptr_t */
#include <pthread.h>
#include "list.h"

#define CLAMP(value, min, max) ((value) < (min) ? (min) : ((value) > (max) ? (max) : (value)))


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

typedef void *(*task_entry_t)(void *);

/**
 * enum task_status_e - Task statuses
 *
 * @PENDING: Task is pending
 * @STARTED: Task has been started
 * @SUCCESS: Task has completed successfully
 * @FAILURE: Task has completed with issues
 */
typedef enum task_status_e
{
    PENDING,
    STARTED,
    SUCCESS,
    FAILURE
} task_status_t;

/**
 * struct task_s - Executable task structure
 *
 * @entry:  Pointer to a function to serve as the task entry
 * @param:  Address to a custom content to be passed to the entry function
 * @status: Task status, default to PENDING
 * @result: Stores the return value of the entry function
 * @lock:   Task mutex
 */
typedef struct task_s
{
    task_entry_t    entry;
    void        *param;
    task_status_t   status;
    void        *result;
    pthread_mutex_t lock;
	int id;
} task_t;



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
/* pixel_t apply_kernel(const img_t *img, const kernel_t *kernel, size_t x, size_t y); */

/**
 * blur_portion - Blur a portion of an image
 * @portion: Information needed to blur a portion of an image
 */
/* void blur_portion(blur_portion_t const *portion); */

/**
 * blur_image - Blur the entire image using Gaussian Blur with multithreading
 * @img_blur: Destination image
 * @img: Source image
 * @kernel: Convolution kernel
 */
void blur_image(img_t *img_blur, img_t const *img, kernel_t const *kernel);

/* static void apply_gaussian_blur(img_t const *img, img_t *img_blur, kernel_t const *kernel, size_t x_start, size_t y_start, size_t width, size_t height); */
void blur_portion(blur_portion_t const *portion);

/**
 * prime_factors - Factors a given number into a list containing prime factors
 *
 * @s: string of the number to factor
 * Return: list of prime factors
 **/
list_t *prime_factors(char const *s);

/**
* create_task - Create a new task with a specified entry function and parameter.
* @entry: Function pointer to the task entry function.
* @param: Parameter for the entry function.
*
* Return: Pointer to the newly created task structure.
*/
task_t *create_task(task_entry_t entry, void *param);

/**
* destroy_task - Free resources associated with a task.
* @task: Pointer to the task to destroy.
*
* Cleans up and deallocates the memory for the specified task.
*/
void destroy_task(task_t *task);

/**
* exec_tasks - Execute a list of tasks in a thread-safe manner.
* @tasks: List of tasks to execute.
*
* Each task is processed by multiple threads, ensuring thread safety.
*
* Return: Always returns NULL.
*/
void *exec_tasks(const list_t *tasks);

/**
 * is_valid_pixel - checks if the given pixel coordinates are within the image bounds.
 *
 * @img: Pointer to the image structure.
 * @x: x-coordinate of the pixel.
 * @y: y-coordinate of the pixel.
 * Return: 1 if the pixel is within bounds, 0 otherwise.
 */
/* static int is_valid_pixel(img_t const *img, size_t x, size_t y); */


#endif /* MULTITHREADING_H */
