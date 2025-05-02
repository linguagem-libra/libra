#include <stddef.h>
#include <stdio.h>
#include "libra.h"

#define CAPACIDADE_INICIAL_TOKENS 10

static Token* gerar_tokens(const char* codigo, size_t* saida_quantidade);
static Nodo* gerar_nodos(const Token* tokens, size_t* saida_quantidade);

void libra_executar(const char* codigo)
{
    size_t t_qtd;
    Token* tokens = gerar_tokens(codigo, &t_qtd);

    size_t n_qtd;
    Nodo* nodos = gerar_nodos(tokens, &n_qtd);
    
    for (size_t i = 0; i < n_qtd; i++)
    {
        if(nodos[i].tipo == NODO_EXPR)
        {
            LibraValor valor = libra_avaliar_expr(nodos[i].expr);
            libra_exibir_valor(valor);
        }
    }
}

LibraValor libra_avaliar_expr(const Expr* expr)
{
    if (!expr) {
        libra_erro("Erro: ponteiro expr é NULL em libra_avaliar_expr");
    }

    switch (expr->tipo)
    {
        case EXPR_LIT:
            return expr->lit.valor;

        case EXPR_BIN:
            if (!expr->bin.esq || !expr->bin.dir) {
                libra_erro("expr->bin.esq ou dir é NULL\n");
            }

            LibraValor a = libra_avaliar_expr(expr->bin.esq);
            LibraValor b = libra_avaliar_expr(expr->bin.dir);

            switch (expr->bin.op)
            {
            case TOKEN_OP_SOMA: return libra_num(a.i32 + b.i32);
            case TOKEN_OP_SUB: return libra_num(a.i32 - b.i32);
            case TOKEN_OP_MUL: return libra_num(a.i32 * b.i32);
            case TOKEN_OP_DIV: return libra_num(a.i32 / b.i32);
            }

            return libra_num(0);
    }

    libra_erro("expr->tipo inválido");
}


static Nodo* gerar_nodos(const Token* tokens, size_t* saida_quantidade)
{
    Parser parser = { tokens, 0 };
    size_t capacidade = CAPACIDADE_INICIAL_TOKENS;
    size_t quantidade = 0;
    Nodo* nodos = libra_alocar(capacidade * sizeof(Nodo));
    Nodo nodo;
    do
    {
        nodo = parser_proximo_nodo(&parser);
        if (quantidade >= capacidade) 
        {
            capacidade *= 2;
            nodos = libra_realocar(nodos, capacidade * sizeof(Nodo));
        }
        nodos[quantidade++] = nodo;
    } while (nodo.tipo != NODO_FINAL);

    *saida_quantidade = quantidade;
    return nodos;
}

static Token* gerar_tokens(const char* codigo, size_t* saida_quantidade)
{
    Tokenizador tokenizador = { codigo, 0 };
    size_t capacidade = CAPACIDADE_INICIAL_TOKENS;
    size_t quantidade = 0;
    Token* tokens = libra_alocar(capacidade * sizeof(Token));
    Token token;
    do
    {
        token = tokenizador_proximo_token(&tokenizador);
        if (quantidade >= capacidade) 
        {
            capacidade *= 2;
            tokens = libra_realocar(tokens, capacidade * sizeof(Token));
        }
        tokens[quantidade++] = token;
    } while (token.tipo != TOKEN_FIM_ARQ);

    *saida_quantidade = quantidade;
    return tokens;
}