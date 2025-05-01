#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include "tokenizador.h"
#include "token.h"

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
}

void exibir_token(Token token)
{
    switch (token.tipo)
    {
        case TOKEN_NUM_LIT: printf("NUM_LIT(%d)\n", token.valor.num); break;
        case TOKEN_FIM_ARQ: printf("FIM_ARQ\n"); break;
    }
}