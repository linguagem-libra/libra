#include <stdio.h>
#include "compilador.h"
#include "memoria.h"
#include "vm.h"
#include "libra.h"

static Nodo prox(Compilador* comp)
{
    return comp->nodos[comp->pos];
}

static Nodo passar(Compilador* comp)
{
    return comp->nodos[comp->pos++];
}

int* libra_compilar_decl_var(Compilador* comp, DeclVar decl)
{
    
}

int* libra_compilar_expr(Compilador* comp, Expr* expr, size_t* tam_inst)
{
    int* bytes = NULL;
    switch (expr->tipo)
    {
    case EXPR_LIT:
        *tam_inst = 2 * sizeof(int); // empilhar x
        bytes = libra_alocar(*tam_inst);
        bytes[0] = OP_EMPILHAR;
        bytes[1] = libra_avaliar_expr(expr).i32;
        passar(comp);
        break;
    case EXPR_BIN:
        *tam_inst = 5 * sizeof(int); // empilhar a, empilhar b, operar
        bytes = libra_alocar(*tam_inst);
        bytes[0] = OP_EMPILHAR;
        bytes[1] = libra_avaliar_expr(expr->bin.esq).i32;
        bytes[2] = OP_EMPILHAR;
        bytes[3] = libra_avaliar_expr(expr->bin.dir).i32;
        switch (expr->bin.op)
        {
        case TOKEN_OP_SOMA: bytes[4] = OP_SOMAR; break;
        case TOKEN_OP_SUB: bytes[4] = OP_SUBTRAIR; break;
        case TOKEN_OP_MUL: bytes[4] = OP_MULTIPLICAR; break;
        }
        passar(comp);
        break;
    }
    return bytes;
}

int* libra_compilar_prox(Compilador* comp, size_t* tam_inst)
{
    Nodo atual = prox(comp);
    // Acabou o programa, gerar instrução de Parar
    if(atual.tipo == NODO_FINAL)
    {
        *tam_inst = 1;
        int* inst_parar = libra_alocar(sizeof(int));
        *inst_parar = OP_PARAR;
        return inst_parar;
    }

    switch (atual.tipo)
    {
    case NODO_EXPR: return libra_compilar_expr(comp, atual.expr, tam_inst);
    }
}