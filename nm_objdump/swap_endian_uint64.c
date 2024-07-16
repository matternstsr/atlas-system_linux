#include "hnm.h"

/**
* swap_endian_uint64 - Swap endianness of a 64-bit unsigned integer
* @num: The number whose endianness to swap
* This function swaps the endianness of a 64-bit unsigned integer.
* Return: The number with swapped endianness
*/
unsigned long swap_endian_uint64(unsigned long num)
{
return (((num >> 56) & 0x00000000000000ff) |
/* Shift right by 56 bits and mask with 0x00000000000000ff */
		((num >> 40) & 0x000000000000ff00) |
		/* Shift right by 40 bits and mask with 0x000000000000ff00 */
		((num >> 24) & 0x0000000000ff0000) |
		/* Shift right by 24 bits and mask with 0x0000000000ff0000 */
		((num >> 8)  & 0x00000000ff000000) |
		/* Shift right by 8 bits and mask with 0x00000000ff000000 */
		((num << 8)  & 0x000000ff00000000) |
		/* Shift left by 8 bits and mask with 0x000000ff00000000 */
		((num << 24) & 0x0000ff0000000000) |
		/* Shift left by 24 bits and mask with 0x0000ff0000000000 */
		((num << 40) & 0x00ff000000000000) |
		/* Shift left by 40 bits and mask with 0x00ff000000000000 */
		((num << 56) & 0xff00000000000000));
		/* Shift left by 56 bits and mask with 0xff00000000000000 */
}