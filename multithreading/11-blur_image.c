#include "multithreading.h"
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

/* Data structure to pass data to threads */
typedef struct {
    img_t *img_blur;
    img_t const *img;
    kernel_t const *kernel;
    size_t x_start;
    size_t y_start;
    size_t width;
    size_t height;
} thread_data_t;

/* Thread function to blur a portion of the image */
static void* thread_blur(void* arg) {
    thread_data_t *data = (thread_data_t*)arg;
    apply_gaussian_blur(data->img, data->img_blur, data->kernel, data->x_start, data->y_start, data->width, data->height);
    return NULL;
}

/* Implementation of blur_image */
void blur_image(img_t *img_blur, img_t const *img, kernel_t const *kernel) {
    const size_t num_threads = 16;
    pthread_t threads[num_threads];
    thread_data_t thread_data[num_threads];

    size_t portion_width = img->w / num_threads;
    size_t portion_height = img->h;

    for (size_t i = 0; i < num_threads; i++) {
        thread_data[i].img_blur = img_blur;
        thread_data[i].img = img;
        thread_data[i].kernel = kernel;
        thread_data[i].x_start = i * portion_width;
        thread_data[i].y_start = 0;
        thread_data[i].width = (i == num_threads - 1) ? (img->w - i * portion_width) : portion_width;
        thread_data[i].height = portion_height;

        pthread_create(&threads[i], NULL, thread_blur, (void*)&thread_data[i]);
    }

    for (size_t i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }
}
