#include <stdio.h>
#include "valor.h"
#include "erro.h"
#include "token.h"
#include "tokenizador.h"

int main(void)
{
    Tokenizador tokenizador = { "69 + 420", 0 };

    Token token;
    do {
        token = tokenizador_proximo_token(&tokenizador);
        exibir_token(token);
    } while (token.tipo != TOKEN_FIM_ARQ);
}