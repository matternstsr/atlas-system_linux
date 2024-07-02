#include "2-hreadelf.h"

/**
 * switch_2_byte - switches endianness on 2 byte unsigned int
 * @n: the unsigned int whose bytes to switch
 * Return: unsigned int with switched bytes
 */
unsigned short switch_2_byte(unsigned short n)
{
	return (((n >> 8)  & 0x00ff) |
			((n << 8)  & 0xff00));
}