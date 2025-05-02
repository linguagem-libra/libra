#include <stdio.h>
#include "arvore.h"
#include "memoria.h"
#include "erro.h"

Expr* libra_expr_lit(LibraValor valor)
{
    Expr* expr = libra_alocar(sizeof(Expr));

    expr->tipo = EXPR_LIT;
    expr->lit.valor = valor;
    return expr;
}

Expr* libra_expr_bin(Expr* esq, TokenTipo op, Expr* dir)
{
    if (!esq || !dir) {
        libra_erro("Operandos inválidos para Expr binária (esq ou dir é NULL)");
    }

    Expr* expr = libra_alocar(sizeof(Expr));

    expr->tipo = EXPR_BIN;
    expr->bin.op = op;
    expr->bin.esq = esq;
    expr->bin.dir = dir;
    return expr;
}

void libra_exibir_expr(Expr expr)
{
    switch (expr.tipo)
    {
    case EXPR_LIT:
        printf("%d", expr.lit.valor.i32);
        break;
    case EXPR_BIN:
        libra_exibir_expr(*expr.bin.esq);
        switch (expr.bin.op)
        {
        case TOKEN_OP_SOMA: printf("+"); break;
        case TOKEN_OP_SUB: printf("-"); break;
        case TOKEN_OP_MUL: printf("*"); break;
        case TOKEN_OP_DIV: printf("/"); break;
        }
        libra_exibir_expr(*expr.bin.dir);
        break;
    }
}