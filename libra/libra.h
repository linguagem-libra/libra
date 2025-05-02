#ifndef LIBRA_H
#define LIBRA_H

#include "valor.h"
#include "erro.h"
#include "token.h"
#include "tokenizador.h"
#include "arvore.h"
#include "memoria.h"
#include "parser.h"
#include "vm.h"
#include "assembler.h"

void libra_executar(const char* codigo);
LibraValor libra_avaliar_expr(const Expr* expr);
void libra_compilar(const char* codigo, const char* arquivo_saida);
void libra_carregar(const char* arquivo_entrada);

#endif // LIBRA_H