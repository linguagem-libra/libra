#include <stdio.h>

#include "libra.h"

size_t libra_alocacoes;

int main(void)
{
    libra_executar("1+2*3");
    printf("%d\n", libra_alocacoes);
}
