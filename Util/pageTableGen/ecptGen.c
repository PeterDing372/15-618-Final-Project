#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "ecptGen.h"
#include "simplecrc.h"

#define LINE_SIZE 16 // 16 hex characters + newline
#define LINE_BYTE (LINE_SIZE/2)
#define FILE_SIZE 8192 // 8kB

typedef struct {
    uint64_t vpn;
    uint64_t addr;
    uint64_t tag;
    uint64_t groupId;
    uint64_t offset;

} pte_info;

int compareByAddr(const void *a, const void *b) {
    const pte_info *pteA = (const pte_info *)a;
    const pte_info *pteB = (const pte_info *)b;

    if (pteA->addr > pteB->addr) return 1;
    if (pteA->addr < pteB->addr) return -1;
    return 0;
}

void splitHexNumber(uint64_t hexNumber, uint64_t output[8]) {
    // Masks for splitting the number into 3-bit groups
    uint64_t masks[8] = {
        0x7,    // Lower 3 bits (bits 0-2)
        0x38,   // Next 3 bits (bits 3-5)
        0x1C0,  // Next 3 bits (bits 6-8)
        0xE00,  // Next 3 bits (bits 9-11)
        0x7000, // Next 3 bits (bits 12-14)
        0x38000, // Next 3 bits (bits 15-17)
        0x1C0000, // Next 3 bits (bits 18-20)
        0xE00000, // Next 3 bits (bits 21-23)
    };

    // Shift amounts for each group
    int shiftAmounts[8] = {0, 3, 6, 9, 12, 15, 18, 21};
    // printf("tag: ");

    // Extracting and storing each group in the output array
    for (int i = 0; i < 8; i++) {
        output[i] = (hexNumber & masks[i]) >> shiftAmounts[i];
        // printf("%ld ", output[i]);

    }
    // printf("\n");

}

uint64_t reverseBytes(uint64_t value) {
    uint64_t reversed = 0;
    reversed |= (value & 0x00000000000000FF) << 56;
    reversed |= (value & 0x000000000000FF00) << 40;
    reversed |= (value & 0x0000000000FF0000) << 24;
    reversed |= (value & 0x00000000FF000000) << 8;
    reversed |= (value & 0x000000FF00000000) >> 8;
    reversed |= (value & 0x0000FF0000000000) >> 24;
    reversed |= (value & 0x00FF000000000000) >> 40;
    reversed |= (value & 0xFF00000000000000) >> 56;
    return reversed;
}

uint64_t createNewPte(uint64_t addr, uint64_t tag) {
    uint64_t new_pte = (tag << 54) | (addr << PTE_PPN_SHIFT) | PTE_V | PTE_U | PTE_R | PTE_W | PTE_X;
    return new_pte;
}


void replaceLine(long val, int pos) {
    FILE *file = fopen("ECPT.hex", "r+");
    if (file == NULL) {
        perror("Error opening file");
        exit(1);
    }

    // Calculate the position in the file
    long filePos = (long)pos * (LINE_SIZE + 1); // +1 for newline character
    fseek(file, filePos, SEEK_SET);

    // Write the value to the file at the specified position
    fprintf(file, "%016lX\n", val);

    fclose(file);
}

void createFile() {
    FILE *file = fopen("ECPT.hex", "w");
    if (file == NULL) {
        perror("Error opening file");
        exit(1);
    }

    char line[LINE_SIZE+1] = "0000000000000000\n";
    int linesNeeded = FILE_SIZE / LINE_BYTE;
    printf("linesNeeded: %d\n", linesNeeded);

    for (int i = 0; i < linesNeeded; i++) {
        fputs(line, file);
    }

    fclose(file);
}

int main() {
    createFile();
    uint64_t array[] = {
        0x80000000, 
        0x80001000, 
        0x80002000, 
        0x80003000, 
        0x80004000
    };
    uint64_t vpn[] = {
        0x80000, 
        0x80001, 
        0x80002, 
        0x80003, 
        0x80004
    };
    uint64_t mask64byte = ~(0xFFFFFFFFFFFFFFFFULL << 6);
    int size = sizeof(array) / sizeof(array[0]);
    pte_info pte_array[size];
    // Declare a new array to store the hashed values
    uint64_t hashedValues[size];


    for(int i = 0; i < size; i++) {
        printf("input: %lx\n", array[i]);
        hashedValues[i] = CRC_hash_param(vpn[i], 27, 0x119, 9);
        printf("testing crc param hashed: %lx\n", hashedValues[i]);
        printf(" vpn: %lx\n", vpn[i]);


        pte_array[i].addr = hashedValues[i];
        pte_array[i].vpn = vpn[i];
        uint64_t tags[8];
        splitHexNumber((vpn[i]>>3), tags);
        int tag_idx = hashedValues[i] & 0x7;
        pte_array[i].tag = tags[tag_idx];
        pte_array[i].groupId = hashedValues[i] & (~0x7);
        pte_array[i].offset = hashedValues[i] & (0x7);
        
    }
    qsort(pte_array, size, sizeof(pte_info), compareByAddr);
    /* Check array sorted */
    for (int i = 0; i < size; i++) {
        printf("pte_array[%d]: addr = %lx, vpn = %lx, tag = %lx id = %lx offset = %lx\n", 
               i, pte_array[i].addr, pte_array[i].vpn, pte_array[i].tag, 
               pte_array[i].groupId, pte_array[i].offset);
    }



    /* Replaces lines */
    /* Since all the address groups into 1 group, complete the group */
    uint64_t start_addr = pte_array[0].groupId; 
    uint64_t curr_addr = start_addr; 

    printf("start_addr: %ld\n",start_addr);
    int hit = 0;
    for(int i = 0; i < 8; i++) {
        hit = 0;
        for (int j = 0; j < size; j++) {
            pte_info curr_info = pte_array[j];
            if (curr_info.offset == i) {
                /* write to entry with current info */
                uint64_t curr_vpn = curr_info.vpn;
                uint64_t tags[8];
                splitHexNumber(curr_vpn>>3, tags);
                uint64_t curr_pte = createNewPte(curr_vpn, tags[curr_info.offset]);
                replaceLine(curr_pte, start_addr+i);
                printf("placing pte: %lx at %ld:%lx tag: %lx\n", 
                        curr_pte, start_addr+i, start_addr+i, tags[curr_info.offset]);
                hit = 1;
                break;
            } 

        }
        if (!hit) {
             /* no match artificially place pte with tag */
            uint64_t curr_vpn = pte_array[0].vpn;
            uint64_t tags[8];
            splitHexNumber(curr_vpn>>3, tags);
            uint64_t curr_pte = createNewPte(curr_vpn, tags[i]);
            replaceLine(curr_pte, start_addr+i);
            printf("placing pte: %lx at %ld:%lx tag: %lx\n",
                    curr_pte, start_addr+i, start_addr+i, tags[i]);
        }
       
        
    }

    printf("File ECPT.hex created with size 8kB.\n");
    return 0;
}
