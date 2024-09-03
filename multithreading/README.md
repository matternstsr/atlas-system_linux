# Multithreading Image Processing

This project demonstrates image processing using multithreading in C.
It includes tasks such as blurring images with Gaussian Blur using threads and managing tasks with a thread pool.
Utility functions for formatted printing with thread IDs are also included for debugging and synchronization.

## Project Structure

- **0-thread_entry.c**: Contains the thread entry function for basic threading examples.
- **1-tprintf.c**: Implements a thread-safe print function (`tprintf`) with thread ID.
- **10-blur_portion.c**: Blurs a specified portion of an image.
- **11-blur_image.c**: Applies Gaussian Blur to the entire image using multiple threads.
- **20-tprintf.c**: Enhanced `tprintf` with mutex synchronization.
- **22-prime_factors.c**: Factorizes numbers into prime factors.
- **list.c**: Implements a linked list used for task management.
- **multithreading.h**: Header file containing data structures and function prototypes.
- **22-prime_factors.h**: Header file for prime factorization functions.

## Data Structures

The project uses several data structures, defined in `multithreading.h`:

- **pixel_t**: Represents an RGB pixel.
- **img_t**: Represents an image with its width, height, and pixel data.
- **kernel_t**: Represents a convolution kernel for image processing.
- **blur_portion_t**: Contains information needed to blur a portion of an image.
- **task_t**: Represents a task with its entry function, parameters, status, and mutex.

## Building the Project

To build the project, use the provided `Makefile`. The `Makefile` includes targets for compiling and linking the different components. Running `make` will compile all source files and produce the executables.

## Running the Executables

### 0-thread_entry

Demonstrates a basic thread entry function.

### 1-tprintf

Shows how to print formatted strings with thread IDs.

### 10-blur_portion

Blurs a portion of an image. Usage requires image files and kernel setup.

### 11-blur_image

Blurs an entire image using multiple threads.

### 20-tprintf

Enhanced `tprintf` function with thread-safe printing.

### 22-prime_factors

Factorizes a number into prime factors. Requires input numbers for usage.

## Dependencies

- GCC for compilation.
- POSIX threads library (`pthread`) for multithreading.