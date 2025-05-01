#include <stdio.h>
#include "valor.h"
#include "erro.h"

int main(void)
{
    LibraValor nulo = libra_nulo();
    LibraValor num = libra_numero(42);
    
    libra_exibir_valor(nulo);
    libra_exibir_valor(num);

    libra_erro("oops\n");
    printf("teste\n");
}