#include "simplecrc_test.h"


/* main.c this is a test for the CRC_hash_param() */
int main(int argc, char *argv[]) {

    unsigned int array[] = {
        0x80000000, 
        0x80001000, 
        0x80002000, 
        0x80003000, 
        0x80004000
    };
    
    int size = sizeof(array) / sizeof(array[0]);

    // Declare a new array to store the hashed values
    long hashedValues[size];

    for(int i = 0; i < size; i++) {
        printf("input: %x\n", array[i]);
        hashedValues[i] = CRC_hash_param(processHexValue(array[i]), 27, 0x865, 12);
        printf("testing crc param hashed: %lx\n", hashedValues[i]);
    }
    // unsigned long CRC_hash_param(unsigned long input, int len_input, unsigned long CRC_POLY, int crc_bits)

}