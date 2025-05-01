#include <stdio.h>
#include "valor.h"

void libra_exibir_valor(LibraValor valor)
{
    switch (valor.tipo)
    {
    case LIBRA_NULO:
        printf("nulo\n");
        break;
    case LIBRA_NUMERO:
        printf("%d\n", valor.valor.i32);
        break;
    }
}