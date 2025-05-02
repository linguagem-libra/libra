#ifndef L_VALOR_H
#define L_VALOR_H

#define libra_nulo() (LibraValor) { .tipo = LIBRA_NULO }
#define libra_num(n) (LibraValor) { .tipo = LIBRA_NUMERO, .i32 = (n) }

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
    };
    
} LibraValor;

void libra_exibir_valor(LibraValor valor);

#endif // L_VALOR_H
