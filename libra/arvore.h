#ifndef L_ARVORE_H
#define L_ARVORE_H

#include "token.h"
#include "valor.h"

typedef struct nodo Nodo;
typedef struct expr Expr;

typedef enum
{
    NODO_FINAL,
    NODO_DECL_VAR,
    NODO_EXPR
} NodoTipo; 

typedef enum
{
    EXPR_LIT,
    EXPR_IDENT,
    EXPR_BIN
} ExprTipo;

typedef struct
{
    LibraValor valor;
} ExprLit;

typedef struct
{
    char* nome;
} ExprIdent;

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
        ExprIdent ident;
        ExprBin bin;
    };
    
} Expr;

typedef struct
{
    char* ident;
    Expr* expr;
} DeclVar;

typedef struct nodo
{
    NodoTipo tipo;
    union
    {
        Expr* expr;
        DeclVar decl_var;
    };
} Nodo;

Expr* libra_expr_lit(LibraValor valor);
Expr* libra_expr_ident(char* nome);
Expr* libra_expr_bin(Expr* esq, TokenTipo op, Expr* dir);
Nodo libra_decl_var(char* ident, Expr* expr);
void libra_exibir_expr(Expr expr);

#endif // L_ARVORE_H