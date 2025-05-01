#ifndef L_TOKENIZADOR
#define L_TOKENIZADOR

#include "token.h"

typedef struct
{
    const char* fonte;
    size_t pos;
} Tokenizador;

Token tokenizador_proximo_token(Tokenizador* tokenizador);
void exibir_token(Token token);

#endif // L_TOKENIZADOR