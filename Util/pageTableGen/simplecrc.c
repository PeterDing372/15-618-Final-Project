#include <string.h>
#include "simplecrc.h"


long processHexValue(unsigned int value) {
    // Mask out the lower 12 bits and zero-extend to 27 bits
    value = (value & 0xFFFFF000) >> 12;

    // Return the processed value
    return value;
}

unsigned int createBitmask(int n) {
    if (n >= sizeof(unsigned int) * 8) {
        // If n is greater than or equal to the number of bits in an unsigned int,
        // return a bitmask with all bits set.
        return ~0;
    }
    return (1U << n) - 1;
}

/** 
    @brief this is parameterized crc hash function unit for verifying hardware crc
    @param len_input: length of the input 
    @param crc_bits: number of crc bits to be calculated 
    @param CRC_POLY: the generator polynomial
    @return 44-bit physical page number

**/
uint64_t CRC_hash_param(unsigned long input, int len_input, unsigned long CRC_POLY, int crc_bits)
{
    // printf("\n\n --------------start hashing crc --------------------\n");
    // printf("input:%lx\nlength: %d\n", input, len_input);
    // decimalToBinary((unsigned long)input);
    // printf("generator poly: %lx\n", CRC_POLY);
    // decimalToBinary((unsigned long)CRC_POLY);
    CRC_POLY <<= (len_input - crc_bits - 1);
    unsigned long remainder = input;
    int count = 0;
    
    for (int j = len_input-1; j >= crc_bits; j--)
    {
        count++;
        // printf("remainder (pre): %lx\n", remainder);
        // decimalToBinary((unsigned long)remainder);
        // int bit = (d & 1 << i) != 0
        if (remainder & (0x1L << (len_input - 1))){
            
            // printf("xoring here:\n");
            // printf("    j:%d\n    remainder: %lx\n", j, remainder);
            // decimalToBinary((unsigned long)remainder);
            // printf("the generator\n");
            // decimalToBinary((unsigned long)CRC_POLY);
            
            remainder ^= CRC_POLY;
            // printf("    remainder (xored): %lx\n    count: %d\n", remainder, count);
            // decimalToBinary((unsigned long)remainder);
        }
            
        remainder <<= 1;
        // printf("remainder (post): %lx\n", remainder);
        // decimalToBinary((unsigned long)remainder);
    }
    uint mask = createBitmask(crc_bits);
    return  (remainder >> count) & mask;
}



/* CRC Hash Function */
unsigned long CRC6_POLY = (0x25) << 2;
/* length: length of the input */
unsigned long getCRC(unsigned long input, int length)
{
    // printf("input:%lx\nlength: %d\n", input, length);
    // decimalToBinary((unsigned long)input);
    // printf("generator poly: %lx\n", CRC6_POLY);
    // decimalToBinary((unsigned long)CRC6_POLY);
    // unsigned long remainder = 0;
    unsigned long bitMask = 0xFFFF;
    unsigned long crc = input;
    int j, count = 0;
    
    for (j = length-1; j >= 5; j--)
    {
        count++;
        // printf("crc (pre): %lx\n", crc);
        // decimalToBinary((unsigned long)crc);
        // int bit = (d & 1 << i) != 0
        if (crc & (0x1 << 7)){
            
            // printf("xoring here:\n");
            // printf("    j:%d\n    crc: %lx\n", j, crc);
            // decimalToBinary((unsigned long)crc);
            // printf("the generator\n");
            // decimalToBinary((unsigned long)CRC6_POLY);
            
            crc ^= CRC6_POLY;
            // printf("    crc (xored): %lx\n    count: %d\n", crc, count);
            // decimalToBinary((unsigned long)crc);
        }
            
        crc <<= 1;
        // printf("crc (post): %lx\n", crc);
        // decimalToBinary((unsigned long)crc);
    }
    return  crc >> count;
}



void decimalToBinary(int num) {   
    // printf("in binary: ");
    if (num == 0) {
        // printf("0");
        return;
    }
   
   // Stores binary representation of number.
   int binaryNum[32]; // Assuming 32 bit integer.
   int i=0;
   
   for ( ;num > 0; ){
      binaryNum[i++] = num % 2;
      num /= 2;
   }
   
   // Printing array in reverse order.
   for (int j = i-1; j >= 0; j--)
      printf("%d", binaryNum[j]);
    // printf("\n");
}

