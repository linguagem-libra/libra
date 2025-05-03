#ifndef L_COMPILADOR_H
#define L_COMPILADOR_H

#include "arvore.h"
#include "vm.h"

typedef struct
{
    const Nodo* nodos;
    size_t pos;
} Compilador;

int* libra_compilar_decl_var(Compilador* comp, DeclVar decl);
int* libra_compilar_expr(Compilador* comp, Expr* expr, size_t* tam_inst);
int* libra_compilar_prox(Compilador* comp, size_t* tam_inst);

#endif // L_COMPILADOR_H