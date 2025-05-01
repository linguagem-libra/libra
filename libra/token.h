#ifndef L_TOKEN
#define L_TOKEN

#define libra_token_fim_arq() ((Token){ .tipo = TOKEN_FIM_ARQ })
#define libra_token_num(n) ((Token){ .tipo = TOKEN_NUM_LIT, .valor.num = (n) })

typedef enum TokenTipo
{
    TOKEN_FIM_ARQ,
    TOKEN_NUM_LIT
} TokenTipo;

typedef struct Token
{
    TokenTipo tipo;
    union {
        int num;
    } valor;
} Token;

#endif