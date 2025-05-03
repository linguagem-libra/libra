#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include "parser.h"
#include "arvore.h"
#include "memoria.h"
#include "erro.h"

static Token prox(Parser* parser)
{
    return parser->tokens[parser->pos];
}

static Token passar(Parser* parser)
{
    return parser->tokens[parser->pos++];
}

static int fim_arq(Parser* parser)
{
    return prox(parser).tipo == TOKEN_FIM_ARQ;
}

static Token consumir(Parser* parser, TokenTipo tipo)
{
    if(prox(parser).tipo != tipo)
    {
        libra_erro("Tipo inesperado");
    }

    return passar(parser);
}

static Expr* parse_expr_lit(Parser* parser)
{
    Token atual = passar(parser);
    int num = atual.valor.num;

    Expr* expr = libra_alocar(sizeof(Expr));
    expr = libra_expr_lit(libra_num(num));
    
    return expr;
}


static int prec_operador(Token token)
{
    switch (token.tipo)
    {
        case TOKEN_OP_MUL:
        case TOKEN_OP_DIV:      return 1;
        case TOKEN_OP_SOMA:
        case TOKEN_OP_SUB:      return 0;
        default:                return -999; // Não é um operador
    }
}

static Expr* parse_expr(Parser* parser, int prec_min)
{
    Expr* esq = parse_expr_lit(parser);
    
    while (1)
    {
        if (fim_arq(parser) || prec_operador(prox(parser)) < prec_min)
            break;

        TokenTipo op = passar(parser).tipo;
        int prox_prec = prec_min + 1;
        Expr* dir = parse_expr(parser, prox_prec);
        
        esq = libra_expr_bin(esq, op, dir);
    }

    return esq;
}

Nodo parse_decl_var(Parser* parser)
{
    consumir(parser, TOKEN_VAR);
    Token tokenIdent = consumir(parser, TOKEN_IDENT);
    consumir(parser, TOKEN_OP_ATRIB);
    Expr* expr = parse_expr(parser, 0);

    return libra_decl_var(tokenIdent.valor.ident, expr);
}

Nodo parser_proximo_nodo(Parser* parser)
{
    if(prox(parser).tipo == TOKEN_FIM_ARQ) return (Nodo) { .tipo = NODO_FINAL };

    Token atual = prox(parser);
    switch (atual.tipo)
    {
    case TOKEN_VAR: return parse_decl_var(parser);
    }

    return (Nodo) {.tipo = NODO_EXPR, .expr = parse_expr(parser, 0)};
}