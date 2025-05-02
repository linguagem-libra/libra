#include <stddef.h>
#include <stdio.h>
#include "libra.h"

#define CAPACIDADE_INICIAL_TOKENS 10

static Token* gerar_tokens(const char* codigo, size_t* saida_quantidade);
static Nodo* gerar_nodos(const Token* tokens, size_t* saida_quantidade);
static int* gerar_instrucoes_vm(const Nodo* nodos, size_t n_qtd, size_t* saida_quantidade);
static void libra_liberar_expr(Expr* expr);

void libra_compilar(const char* codigo, const char* arquivo_saida) {
    size_t t_qtd;
    Token* tokens = gerar_tokens(codigo, &t_qtd);

    size_t n_qtd;
    Nodo* nodos = gerar_nodos(tokens, &n_qtd);

    size_t i_qtd;
    int* instrucoes = gerar_instrucoes_vm(nodos, n_qtd, &i_qtd);

    // Salvar as instruções no arquivo de saída
    libra_vm_salvar_bytecode(arquivo_saida, instrucoes, i_qtd);

    // Limpar memória alocada
    for (size_t i = 0; i < n_qtd; i++) {
        if (nodos[i].tipo == NODO_EXPR) {
            Expr* expr = nodos[i].expr;
            libra_liberar_expr(expr);
        }
    }

    libra_liberar(tokens);
    libra_liberar(nodos);
}

void libra_carregar(const char* arquivo_entrada) {
    size_t tam_cod;
    int* codigo = libra_vm_carregar_bytecode(arquivo_entrada, &tam_cod);

    LibraVM vm;
    libra_vm_iniciar(&vm, 1000);
    libra_vm_carregar_prog(&vm, codigo, tam_cod);
    libra_vm_executar(&vm);
    printf("%d\n", libra_vm_topo_pilha(&vm));

    libra_vm_limpar(&vm);
    libra_liberar(codigo);
}

void libra_executar(const char* codigo)
{
    size_t t_qtd;
    Token* tokens = gerar_tokens(codigo, &t_qtd);

    size_t n_qtd;
    Nodo* nodos = gerar_nodos(tokens, &n_qtd);
    
    size_t i_qtd;
    int* instrucoes = gerar_instrucoes_vm(nodos, n_qtd, &i_qtd);

    LibraVM vm;
    libra_vm_iniciar(&vm, 1000);
    libra_vm_carregar_prog(&vm, instrucoes, i_qtd);
    libra_vm_executar(&vm);
    printf("%d\n", libra_vm_topo_pilha(&vm));

    libra_vm_limpar(&vm);

    for (size_t i = 0; i < n_qtd; i++)
    {
        if(nodos[i].tipo == NODO_EXPR)
        {
            Expr* expr = nodos[i].expr;
            libra_liberar_expr(expr);
        }
    }

    libra_liberar(tokens);
    libra_liberar(nodos);
}

static void libra_liberar_expr(Expr* expr)
{
    if(expr->tipo == EXPR_BIN)
    {
        libra_liberar_expr(expr->bin.esq);
        libra_liberar_expr(expr->bin.dir);
    }

    libra_liberar(expr);
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

static int* gerar_instrucoes_vm(const Nodo* nodos, size_t n_qtd, size_t* saida_quantidade)
{
    size_t capacidade = CAPACIDADE_INICIAL_TOKENS;
    size_t quantidade = 0;
    int* instrucoes = libra_alocar(capacidade * sizeof(int));

    for (size_t i = 0; i < n_qtd; i++)
    {
        const Nodo* nodo = &nodos[i];
        if (nodo->tipo == NODO_EXPR)
        {
            Expr* expr = nodo->expr;
            if (expr->tipo == EXPR_LIT)
            {
                // Instrução de empilhar o valor literal
                if (quantidade + 2 >= capacidade) // Precisamos de dois elementos: OP e valor
                {
                    capacidade *= 2;
                    instrucoes = libra_realocar(instrucoes, capacidade * sizeof(int));
                }

                instrucoes[quantidade++] = OP_EMPILHAR;  // Tipo de operação
                instrucoes[quantidade++] = expr->lit.valor.i32; // Valor da literal
            }
            else if (expr->tipo == EXPR_BIN)
            {
                // Gerar instruções para expressões binárias
                // Empilhar operando esquerdo
                int esq_valor = libra_avaliar_expr(expr->bin.esq).i32;
                if (quantidade + 2 >= capacidade)
                {
                    capacidade *= 2;
                    instrucoes = libra_realocar(instrucoes, capacidade * sizeof(int));
                }
                instrucoes[quantidade++] = OP_EMPILHAR;
                instrucoes[quantidade++] = esq_valor;

                // Empilhar operando direito
                int dir_valor = libra_avaliar_expr(expr->bin.dir).i32;
                if (quantidade + 2 >= capacidade)
                {
                    capacidade *= 2;
                    instrucoes = libra_realocar(instrucoes, capacidade * sizeof(int));
                }
                instrucoes[quantidade++] = OP_EMPILHAR;
                instrucoes[quantidade++] = dir_valor;

                // Instrução de operação binária
                int op;
                switch (expr->bin.op)
                {
                    case TOKEN_OP_SOMA: op = OP_SOMAR; break;
                    case TOKEN_OP_SUB: op = OP_SUBTRAIR; break;
                    case TOKEN_OP_MUL: op = OP_MULTIPLICAR; break;
                    //case TOKEN_OP_DIV: op = OP_DIVIDIR; break;
                    default: continue;  // Ignora operações desconhecidas
                }

                if (quantidade + 1 >= capacidade)
                {
                    capacidade *= 2;
                    instrucoes = libra_realocar(instrucoes, capacidade * sizeof(int));
                }
                instrucoes[quantidade++] = op;
            }
        }
    }

    *saida_quantidade = quantidade;
    return instrucoes;
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