#include <stdio.h>

#define MAX_ADDRESSES 100  // Maximum number of addresses
#define GROUP_SIZE 8       // Size of each group (8-byte alignment)
#define MAX_GROUPS (MAX_ADDRESSES / GROUP_SIZE + 1) // Maximum number of groups

void groupAddresses(const int *addresses, int numAddresses, int output[MAX_GROUPS][GROUP_SIZE], int *groupSizes) {
    for (int i = 0; i < numAddresses; ++i) {
        int address = addresses[i];
        int groupIndex = address / GROUP_SIZE;

        // Check for out-of-bounds index
        if (groupIndex >= MAX_GROUPS) {
            printf("Address %d is out of bounds and will be ignored.\n", address);
            continue;
        }

        int positionInGroup = groupSizes[groupIndex];
        if (positionInGroup < GROUP_SIZE) {
            output[groupIndex][positionInGroup] = address;
            groupSizes[groupIndex]++;
        } else {
            printf("Group %d is already full and cannot accommodate address %d.\n", groupIndex, address);
        }
    }
}

int main() {
    int addresses[] = {232, 233, 234, 235, 239};
    int numAddresses = sizeof(addresses) / sizeof(addresses[0]);

    int groupedAddresses[MAX_GROUPS][GROUP_SIZE] = {0};
    int groupSizes[MAX_GROUPS] = {0};

    groupAddresses(addresses, numAddresses, groupedAddresses, groupSizes);

    // Print the grouped addresses
    for (int i = 0; i < MAX_GROUPS; ++i) {
        if (groupSizes[i] > 0) {
            printf("Group %d: ", i);
            for (int j = 0; j < groupSizes[i]; ++j) {
                printf("%d ", groupedAddresses[i][j]);
            }
            printf("\n");
        }
    }

    return 0;
}
