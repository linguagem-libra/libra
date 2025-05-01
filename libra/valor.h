#ifndef L_VALOR_H
#define L_VALOR_H

#define libra_nulo() (LibraValor) { .tipo = LIBRA_NULO }
#define libra_numero(n) (LibraValor) { .tipo = LIBRA_NUMERO, .valor.i32 = (n) }

typedef enum
{
    LIBRA_NULO,
    LIBRA_NUMERO
} LibraTipo;

typedef struct
{
    LibraTipo tipo;
    union
    {
        int i32;
    } valor;
    
} LibraValor;

void libra_exibir_valor(LibraValor valor);

#endif // L_VALOR_H
