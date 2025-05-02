#ifndef LIBRA_H
#define LIBRA_H

#include "valor.h"
#include "erro.h"
#include "token.h"
#include "tokenizador.h"
#include "arvore.h"
#include "memoria.h"
#include "parser.h"

void libra_executar(const char* codigo);
LibraValor libra_avaliar_expr(const Expr* expr);

#endif // LIBRA_H