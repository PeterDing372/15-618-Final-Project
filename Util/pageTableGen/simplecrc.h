#ifndef SIMPLECRC_H
#define SIMPLECRC_H
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
unsigned int CRC_hash(char *str);
unsigned long getCRC(unsigned long input, int length);
void decimalToBinary(int num);
uint64_t CRC_hash_param(unsigned long input, int len_input, 
                            unsigned long CRC_POLY, int crc_bits);
long processHexValue(unsigned int value);

#endif