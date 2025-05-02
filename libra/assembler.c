#include <stdio.h>
#include "assembler.h"
#include "vm.h"

void libra_asm_compilar_arq(const int *array, size_t size)
{
    FILE *file = fopen("saida.lasm", "w");
    if (file == NULL)
    {
        perror("Erro ao abrir o arquivo");
        return;
    }

    for (size_t i = 0; i < size; i++)
    {
        const char *instructionName = libra_nome_instrucao(array[i]);
        if (instructionName)
        {
            fprintf(file, "%s", instructionName);
            // Se a instrução for EMPILHAR, verificar o próximo valor como argumento
            if (array[i] == OP_EMPILHAR && i + 1 < size)
            {
                fprintf(file, " %d", array[i + 1]);
                i++; // Pular o argumento
            }
            fprintf(file, "\n");
        }
        else
        {
            fprintf(stderr, "Opcode desconhecido: %d\n", array[i]);
        }
    }

    fclose(file);
}