#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "tokenizador.h"
#include "token.h"
#include "erro.h"
#include "memoria.h"

static char prox(Tokenizador* tokenizador)
{
    return tokenizador->fonte[tokenizador->pos];
}

static char passar(Tokenizador* tokenizador)
{
    return tokenizador->fonte[tokenizador->pos++];
}

static void pular_espacos(Tokenizador* tokenizador)
{
    while (isspace(prox(tokenizador)))
    {
        passar(tokenizador);
    }
}

void exibir_token(Token token)
{
    switch (token.tipo)
    {
        case TOKEN_NUM_LIT: printf("NUM_LIT(%d)\n", token.valor.num); break;
        case TOKEN_FIM_ARQ: printf("FIM_ARQ\n"); break;
        case TOKEN_INVALIDO: printf("INVALIDO\n"); break;
        case TOKEN_OP_SOMA: printf("OP_SOMA\n"); break;
        case TOKEN_OP_SUB: printf("OP_SUB\n"); break;
        case TOKEN_OP_MUL: printf("OP_MUL\n"); break;
        case TOKEN_OP_DIV: printf("OP_DIV\n"); break;
    }
}

Token tokenizador_proximo_token(Tokenizador* tokenizador)
{
    pular_espacos(tokenizador);

    char atual = prox(tokenizador);

    if (atual == '\0')
    {
        return libra_token_fim_arq();
    }

    if (isdigit(atual))
    {
        int valor = 0;
        while (isdigit(prox(tokenizador)))
        {
            valor = valor * 10 + (passar(tokenizador) - '0');
        }
        return libra_token_num(valor);
    }
    
    if (isalpha(prox(tokenizador)))
    {
        char buf[256] = "";
        while (isalnum(prox(tokenizador)))
        {
            char c = passar(tokenizador);
            size_t len = strlen(buf);
            if (len < sizeof(buf) - 1)
            {
                buf[len] = c;
                buf[len + 1] = '\0';
            } 
            else
            {
                libra_erro("Buffer overflow!\n");
            }
        }

    // Alocar dinamicamente o buffer final
    char* buf_ptr = (char*)libra_alocar(strlen(buf) + 1);

    strcpy(buf_ptr, buf);

    if (strcmp(buf_ptr, "var") == 0)
    {
        free(buf_ptr); // Liberar memória, pois não será usada aqui
        return libra_token_var();
    } 
    else
    {
        return (Token) {.tipo = TOKEN_IDENT, .valor.ident = buf_ptr};
    }
}

    switch (prox(tokenizador))
    {
    case '+': passar(tokenizador); return libra_token_op_soma();
    case '-': passar(tokenizador); return libra_token_op_sub();
    case '*': passar(tokenizador); return libra_token_op_mul();
    case '/': passar(tokenizador); return libra_token_op_div();
    case '=': passar(tokenizador); return libra_token_op_atrib();
    }

    return libra_token_invalido();
}
