#include "hnm.h"

/**
 * swap_endian_uint32 - Swaps endianness of a 32-bit unsigned integer.
 * @num: The unsigned integer whose bytes to swap.
 *
 * This function swaps the endianness of a 32-bit unsigned integer.
 *
 * Return: The unsigned integer with swapped bytes.
 */
uint32_t swap_endian_uint32(uint32_t num)
{
    return (((num >> 24) & 0x000000ff) |   /* Shift right by 24 bits and mask with 0x000000ff */
            ((num >> 8)  & 0x0000ff00) |   /* Shift right by 8 bits and mask with 0x0000ff00 */
            ((num << 8)  & 0x00ff0000) |   /* Shift left by 8 bits and mask with 0x00ff0000 */
            ((num << 24) & 0xff000000));   /* Shift left by 24 bits and mask with 0xff000000 */
}