#ifndef L_ARVORE_H
#define L_ARVORE_H

#include "token.h"
#include "valor.h"

typedef struct nodo Nodo;
typedef struct expr Expr;

typedef enum
{
    NODO_FINAL,
    NODO_EXPR
} NodoTipo; 

typedef enum
{
    EXPR_LIT,
    EXPR_BIN
} ExprTipo;

typedef struct
{
    LibraValor valor;
} ExprLit;

typedef struct
{
    Expr* esq;
    TokenTipo op;
    Expr* dir;
} ExprBin;

typedef struct expr
{
    ExprTipo tipo;
    union
    {
        ExprLit lit;
        ExprBin bin;
    };
    
} Expr;

typedef struct nodo
{
    NodoTipo tipo;
    union
    {
        Expr* expr;
    };
} Nodo;

Expr* libra_expr_lit(LibraValor valor);
Expr* libra_expr_bin(Expr* esq, TokenTipo op, Expr* dir);
void libra_exibir_expr(Expr expr);
#endif // L_ARVORE_H