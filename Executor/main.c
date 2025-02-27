#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "executor.h"
#include "instruction.h"

void getRealAddress(void* value, size_t size) {
    uint8_t* bytes = (uint8_t*) value;
    for (size_t i = 0; i < size / 2; i++) {
        uint8_t aux = bytes[i];
        bytes[i] = bytes[size - i - 1];
        bytes[size - i - 1] = aux;
    }
}

int main(int argc, char** argv) {
    if (argc < 2) {
        printf("Uso: %s <arquivo.mem>\n", argv[0]);
        return 1;
    }

    FILE *file = fopen(argv[1], "rb");
    if (file == NULL) {
        printf("Erro: Não foi possível abrir o arquivo %s!\n", argv[1]);
        return 1;
    }

    CPU* executor = createMemory();
    fread(executor->memory, sizeof(uint8_t), TOTAL_SIZE, file);
    fclose(file);

    uint32_t file_validation = *((uint32_t*)(executor->memory));
    getRealAddress(&file_validation, sizeof(uint32_t));

    if (file_validation != SIGNATURE_FILE) {
        printf("Erro: Arquivo não compatível. Extensão aceita: .mem!\n");
        freeMemory(executor);
        return 1;
    }

    printf("Arquivo carregado com sucesso! SIGNATURE: 0x%x\n\n", file_validation);

    while (executor->memory[executor->PC] != HLT && executor->PC < TOTAL_SIZE) {
        printf("\nAC: %02X  PC: %02X  FZ: %d  FN: %d  INSTRUÇÃO: %02X  CONTEÚDO: %02X\n",
               executor->AC, executor->PC, flagZero(executor->AC), flagNeg(executor->AC),
               executor->memory[executor->PC], executor->memory[executor->PC + 2]);

        switch (executor->memory[executor->PC]) {
            case STA: instructionSTA(executor); break;
            case LDA: instructionLDA(executor); break;
            case ADD: instructionADD(executor); break;
            case OR:  instructionOR(executor);  break;
            case AND: instructionAND(executor); break;
            case NOT: instructionNOT(executor); break;
            case JMP: instructionJMP(executor); break;
            case JN:  instructionJN(executor);  break;
            case JZ:  instructionJZ(executor);  break;
            default:
                printf("Instrucao desconhecida: %02X. Pulando...\n", executor->memory[executor->PC]);
                executor->PC += 2;
                break;
        }

        printMemory(executor->memory);
        printf("\n");
    }

    printf("\nExecução finalizada.\n");
    freeMemory(executor);
    return 0;
}
