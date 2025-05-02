#ifndef L_TOKEN
#define L_TOKEN

#define libra_token_invalido(n) ((Token){ .tipo = TOKEN_INVALIDO })
#define libra_token_fim_arq() ((Token){ .tipo = TOKEN_FIM_ARQ })
#define libra_token_num(n) ((Token){ .tipo = TOKEN_NUM_LIT, .valor.num = (n) })
#define libra_token_op_soma() ((Token){ .tipo = TOKEN_OP_SOMA })
#define libra_token_op_sub() ((Token){ .tipo = TOKEN_OP_SUB })
#define libra_token_op_mul() ((Token){ .tipo = TOKEN_OP_MUL })
#define libra_token_op_div() ((Token){ .tipo = TOKEN_OP_DIV })

typedef enum TokenTipo
{
    TOKEN_INVALIDO,
    TOKEN_FIM_ARQ,
    TOKEN_NUM_LIT,
    TOKEN_OP_SOMA,
    TOKEN_OP_SUB,
    TOKEN_OP_MUL,
    TOKEN_OP_DIV
} TokenTipo;

typedef struct Token
{
    TokenTipo tipo;
    union {
        int num;
    } valor;
} Token;

#endif