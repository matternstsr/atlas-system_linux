#include "hnm.h"

/**
* swap_endian_uint16 - Switches endianness of a 16-bit unsigned integer.
* @value: The unsigned integer whose bytes to switch.
* This function swaps the endianness of a 16-bit unsigned integer.
* return: Unsigned integer with switched bytes.
*/
uint16_t swap_endian_uint16(uint16_t value)
{
	return (((value >> 8) & 0x00FF) |
	/* Shift right by 8 bits and mask with 0x00FF */
			((value << 8) & 0xFF00));
			/* Shift left by 8 bits and mask with 0xFF00 */
}