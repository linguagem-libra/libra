#ifndef L_MEMORIA_H
#define L_MEMORIA_H

void* libra_alocar(size_t tamanho);
void* libra_realocar(void* ptr, size_t tamanho);
void libra_liberar(void* ptr);

#endif // L_MEMORIA_H