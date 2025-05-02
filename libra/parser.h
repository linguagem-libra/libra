#ifndef L_PARSER_H
#define L_PARSER_H

#include "token.h"
#include "arvore.h"

typedef struct
{
    const Token* tokens;
    size_t pos;
} Parser;

Nodo parser_proximo_nodo(Parser* parser);

#endif // L_PARSER_H