#include <stddef.h>
#include <stdio.h>
#include "libra.h"

#define CAPACIDADE_INICIAL_TOKENS 1000
#define CAPACIDADE_INICIAL_PROG 500 * sizeof(int)

static Token* gerar_tokens(const char* codigo, size_t* saida_quantidade);
static Nodo* gerar_nodos(const Token* tokens, size_t* saida_quantidade);
static int* gerar_bytecode(const Nodo* nodos, size_t* saida_quantidade);
static void liberar_expr(Expr* expr);
static void liberar_tudo(Token* tokens, Nodo* nodos, int* bytecode); // lá ele

void libra_compilar(const char* codigo, const char* arquivo_saida) {
    size_t t_qtd;
    Token* tokens = gerar_tokens(codigo, &t_qtd);

    size_t n_qtd;
    Nodo* nodos = gerar_nodos(tokens, &n_qtd);

    size_t i_qtd;
    int* bytecode = gerar_bytecode(nodos, &i_qtd);

    libra_vm_salvar_bytecode(arquivo_saida, bytecode, i_qtd);

    liberar_tudo(tokens, nodos, bytecode);
}

void libra_carregar(const char* arquivo_entrada) {
    size_t tam_cod;
    int* bytecode = libra_vm_carregar_bytecode(arquivo_entrada, &tam_cod);

    LibraVM vm;
    libra_vm_iniciar(&vm, 1000);
    libra_vm_carregar_prog(&vm, bytecode, tam_cod);
    libra_vm_executar(&vm);
    if(1)
        printf("%d\n", libra_vm_topo_pilha(&vm));

    libra_vm_limpar(&vm);
    libra_liberar(bytecode);
}

void libra_executar(const char* codigo)
{
    size_t t_qtd;
    Token* tokens = gerar_tokens(codigo, &t_qtd);

    size_t n_qtd;
    Nodo* nodos = gerar_nodos(tokens, &n_qtd);
    
    size_t i_qtd;
    int* bytecode = gerar_bytecode(nodos, &i_qtd);

    LibraVM vm;
    libra_vm_iniciar(&vm, 1000);
    libra_vm_carregar_prog(&vm, bytecode, i_qtd);
    libra_vm_executar(&vm);
    printf("%d\n", libra_vm_topo_pilha(&vm));

    libra_vm_limpar(&vm);

    liberar_tudo(tokens, nodos, NULL); // VM já limpa o bytecode
}

static void liberar_expr(Expr* expr)
{
    if(expr->tipo == EXPR_BIN)
    {
        liberar_expr(expr->bin.esq);
        liberar_expr(expr->bin.dir);
    }

    libra_liberar(expr);
}

static void liberar_tudo(Token* tokens, Nodo* nodos, int* bytecode)
{
    // Limpar os nodos
    for (size_t i = 0; ; i++) {
        if(nodos[i].tipo == NODO_FINAL) break;
        if (nodos[i].tipo == NODO_EXPR) {
            Expr* expr = nodos[i].expr;
            liberar_expr(expr);
        }
    }

    libra_liberar(tokens);
    libra_liberar(nodos);
    libra_liberar(bytecode);
}

LibraValor libra_avaliar_expr(const Expr* expr)
{
    if (!expr)
    {
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

static int* gerar_bytecode(const Nodo* nodos, size_t* saida_quantidade)
{
    Compilador comp = { nodos, 0 };
    size_t capacidade = CAPACIDADE_INICIAL_PROG;
    size_t quantidade = 0;
    int* instrucoes = libra_alocar(capacidade * sizeof(int));
    
    while (1)
    {
        size_t tam_inst;
        int* instrucao = libra_compilar_prox(&comp, &tam_inst);
        
        if (*instrucao == 0) // Verifica se chegou ao final da compilação
        {
            break;
        }

        // Redimensiona o array de instruções caso necessário
        if (quantidade + tam_inst > capacidade)
        {
            capacidade *= 2; // Dobra a capacidade
            instrucoes = libra_realocar(instrucoes, capacidade * sizeof(int));
        }

        // Copia a nova instrução para o array principal
        libra_copiar_mem(&instrucoes[quantidade], instrucao, tam_inst * sizeof(int));
        quantidade += (tam_inst/sizeof(int));
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