#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "libra.h"

#define BUFFER_SIZE 256
size_t libra_alocacoes;

void arg_compilar(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Erro: Não foi possível abrir o arquivo %s\n", filename);
        exit(EXIT_FAILURE);
    }

    // Ler o conteúdo do arquivo
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    rewind(file);

    char *conteudo = (char *)libra_alocar(file_size + 1);
    if (!conteudo) {
        fprintf(stderr, "Erro: Falha ao alocar memória para o conteúdo do arquivo.\n");
        fclose(file);
        exit(EXIT_FAILURE);
    }

    fread(conteudo, 1, file_size, file);
    conteudo[file_size] = '\0';
    fclose(file);

    // Adicionar ".o" ao final do nome do arquivo
    char output_filename[BUFFER_SIZE];
    snprintf(output_filename, BUFFER_SIZE, "%s.o", filename);

    // Chamar a função libra_compilar
    libra_compilar(conteudo, output_filename);

    libra_liberar(conteudo);
}

void executar_arquivo(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Erro: Não foi possível abrir o arquivo %s\n", filename);
        exit(EXIT_FAILURE);
    }

    // Ler o conteúdo do arquivo
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    rewind(file);

    char *conteudo = (char *)libra_alocar(file_size + 1);
    if (!conteudo) {
        fprintf(stderr, "Erro: Falha ao alocar memória para o conteúdo do arquivo.\n");
        fclose(file);
        exit(EXIT_FAILURE);
    }

    fread(conteudo, 1, file_size, file);
    conteudo[file_size] = '\0';
    fclose(file);

    // Executar o conteúdo do arquivo
    libra_executar(conteudo);

    libra_liberar(conteudo);
}

int main(int argc, char *argv[]) {
    if (argc > 2 && strcmp(argv[1], "-c") == 0) {
        arg_compilar(argv[2]);
        return 0;
    }

    if (argc > 2 && strcmp(argv[1], "-r") == 0) {
        libra_carregar(argv[2]);
        return 0;
    }

    // Verificar se há apenas um argumento
    if (argc == 2) {
        executar_arquivo(argv[1]);
        return 0;
    }

    char input[BUFFER_SIZE];

    printf("Libra 0.1.0\n");

    while (1) {
        printf("> ");
        if (fgets(input, BUFFER_SIZE, stdin) == NULL) {
            break;
        }

        size_t len = strlen(input);
        if (len > 0 && input[len - 1] == '\n') {
            input[len - 1] = '\0';
        }

        if (strcmp(input, "sair") == 0) {
            printf("Encerrando o REPL. Até logo!\n");
            break;
        }

        libra_executar(input);
    }

    return 0;
}