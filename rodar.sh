#!/usr/bin/bash

# Criar diretório build se não existir
mkdir -p build

# Compilar arquivos em cli/ e libra/ com inclusão do diretório libra/
gcc cli/*.c libra/*.c -Ilibra -o build/librac && ./build/librac
