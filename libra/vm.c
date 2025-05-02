#include <stdio.h>
#include <stdlib.h>
#include "vm.h"

void libra_vm_iniciar(LibraVM* vm, const size_t tam_pilha) {
    vm->tam_pilha = tam_pilha;
    vm->sp = 0;
    vm->pc = 0;
    vm->pilha = (int*)libra_alocar(tam_pilha * sizeof(int));
    vm->codigo = NULL;
    vm->tam_cod = 0;
}

void libra_vm_carregar_prog(LibraVM* vm, int* codigo, const size_t tam_cod) {
    vm->codigo = codigo;
    vm->tam_cod = tam_cod;
    vm->pc = 0;
}

void libra_vm_limpar(LibraVM* vm) {
    libra_liberar(vm->pilha);
}

int libra_vm_proximo_byte(LibraVM* vm) {
    if (vm->pc >= vm->tam_cod) {
        return -1;
    }
    return vm->codigo[vm->pc++];
}

int libra_vm_topo_pilha(LibraVM* vm) {
    if (vm->sp == 0) {
        libra_erro("Pilha vazia\n");
    }
    return vm->pilha[vm->sp - 1];
}

int libra_vm_pilha_cheia(LibraVM* vm) {
    return vm->sp == vm->tam_pilha;
}

int libra_vm_pilha_vazia(LibraVM* vm) {
    return vm->sp == 0;
}

void libra_vm_empilhar(LibraVM* vm, int valor) {
    if (libra_vm_pilha_cheia(vm)) {
        libra_erro("Pilha cheia\n");
    }
    vm->pilha[vm->sp++] = valor;
}

int libra_vm_desempilhar(LibraVM* vm) {
    if (libra_vm_pilha_vazia(vm)) {
        libra_erro("Pilha vazia\n");
    }
    return vm->pilha[--vm->sp];
}

void libra_vm_executar(LibraVM* vm) {
    int instrucao;
    int a, b;

    while ((instrucao = libra_vm_proximo_byte(vm)) != -1) {
        switch (instrucao) {
            case OP_PARAR:
                return;

            case OP_EMPILHAR:
                a = libra_vm_proximo_byte(vm);
                libra_vm_empilhar(vm, a);
                break;

            case OP_DESEMPILHAR:
                a = libra_vm_desempilhar(vm);
                break;

            case OP_SOMAR:
                if (libra_vm_pilha_vazia(vm)) {
                    libra_erro("Pilha vazia para soma\n");
                }
                b = libra_vm_desempilhar(vm);
                a = libra_vm_desempilhar(vm);
                libra_vm_empilhar(vm, a + b);
                break;

            case OP_SUBTRAIR:
                if (libra_vm_pilha_vazia(vm)) {
                    libra_erro("Pilha vazia para subtração\n");
                }
                b = libra_vm_desempilhar(vm);
                a = libra_vm_desempilhar(vm);
                libra_vm_empilhar(vm, a - b);
                break;

            case OP_MULTIPLICAR:
                if (libra_vm_pilha_vazia(vm)) {
                    libra_erro("Pilha vazia para multiplicação\n");
                }
                b = libra_vm_desempilhar(vm);
                a = libra_vm_desempilhar(vm);
                libra_vm_empilhar(vm, a * b);
                break;

            case OP_EXIBIR:
                a = libra_vm_topo_pilha(vm);
                printf("%d\n", a);
                break;

            default:
                libra_erro("Instrução desconhecida");
                return;
        }
    }
}