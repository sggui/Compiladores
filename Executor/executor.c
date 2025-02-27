#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "executor.h"

void printMemory(const uint8_t *memory) {
    for (int i = 0; i < TOTAL_SIZE; i++) {
        if (i % 16 == 0) {
            printf("\n");
        } else {
            printf(" ");
        }
        printf("%02x", memory[i]);
    }
    printf("\n");
}

void freeMemory(CPU *cpu) {
    if (cpu == NULL) {
        return;
    }
    
    free(cpu->memory);
    cpu->memory = NULL;

    free(cpu);
    cpu = NULL;
}

CPU* createMemory() {
    CPU *cpu = (CPU*)malloc(sizeof(CPU));
    if (cpu == NULL) {
        fprintf(stderr, "Erro ao alocar memória para a CPU.\n");
        exit(EXIT_FAILURE);
    }

    cpu->memory = (uint8_t*)malloc(TOTAL_SIZE * sizeof(uint8_t));
    if (cpu->memory == NULL) {
        fprintf(stderr, "Erro ao alocar memória para a memória da CPU.\n");
        free(cpu);
        exit(EXIT_FAILURE);
    }
    
    cpu->AC = 0x00;
    cpu->PC = 0x04;
    cpu->position = 0;

    return cpu;
}
