#include "_getline.h"

// Define your own malloc function
void *my_malloc(size_t size) {
    // Call the system malloc to allocate memory
    void *ptr = malloc(size);
    if (ptr != NULL) {
        // Initialize the allocated memory to 0xFF
        memset(ptr, 0xFF, size);
    }
    return ptr;
}

// Define your own free function
void my_free(void *ptr) {
    // Call the system free to deallocate memory
    free(ptr);
}