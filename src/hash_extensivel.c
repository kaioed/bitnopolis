#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hash_extensivel.h"

typedef struct {
    int chave;
    char dado[50];
} Registro;

typedef struct {
    int profundidade_local;
    int quantidade;
    Registro registros[TAMANHO_BALDE];
} Balde;

typedef struct {
    int profundidade_global;
    int tamanho_diretorio;
    Balde** diretorio;
} HashInterno;

bool hash_inserir(HashExtensivel* ptr_hash, int chave, const char* dado);

int funcao_hash(int chave, int profundidade) {
    return chave & ((1 << profundidade) - 1);
}

Balde* criar_balde(int profundidade_local) {
    Balde* b = (Balde*)malloc(sizeof(Balde));
    b->profundidade_local = profundidade_local;
    b->quantidade = 0;
    return b;
}

HashExtensivel* hash_criar(int profundidade_inicial) {
    HashInterno* hash = (HashInterno*)malloc(sizeof(HashInterno));
    hash->profundidade_global = profundidade_inicial;
    hash->tamanho_diretorio = 1 << profundidade_inicial;
    hash->diretorio = (Balde**)malloc(hash->tamanho_diretorio * sizeof(Balde*));

    for (int i = 0; i < hash->tamanho_diretorio; i++) {
        hash->diretorio[i] = criar_balde(profundidade_inicial);
    }
    return (HashExtensivel*)hash;
}

bool hash_buscar(HashExtensivel* ptr_hash, int chave, char* saida_dado) {
    HashInterno* hash = (HashInterno*)ptr_hash;
    int indice = funcao_hash(chave, hash->profundidade_global);
    Balde* b = hash->diretorio[indice];

    for (int i = 0; i < b->quantidade; i++) {
        if (b->registros[i].chave == chave) {
            if (saida_dado != NULL) {
                strcpy(saida_dado, b->registros[i].dado);
            }
            return true;
        }
    }
    return false;
}

void tratar_split(HashInterno* hash, int indice_cheio) {
    Balde* balde_antigo = hash->diretorio[indice_cheio];
    
    if (balde_antigo->profundidade_local == hash->profundidade_global) {
        int novo_tamanho = hash->tamanho_diretorio * 2;
        Balde** novo_diretorio = (Balde**)malloc(novo_tamanho * sizeof(Balde*));
        
        for (int i = 0; i < hash->tamanho_diretorio; i++) {
            novo_diretorio[i] = hash->diretorio[i];
            novo_diretorio[i + hash->tamanho_diretorio] = hash->diretorio[i];
        }
        
        free(hash->diretorio);
        hash->diretorio = novo_diretorio;
        hash->tamanho_diretorio = novo_tamanho;
        hash->profundidade_global++;
    }

    int nova_profundidade = balde_antigo->profundidade_local + 1;
    Balde* balde_novo = criar_balde(nova_profundidade);
    balde_antigo->profundidade_local = nova_profundidade;

    Registro temporario[TAMANHO_BALDE];
    int qtd_temp = balde_antigo->quantidade;
    for(int i = 0; i < qtd_temp; i++) {
        temporario[i] = balde_antigo->registros[i];
    }
    balde_antigo->quantidade = 0; 

    int mascara_novo = 1 << (nova_profundidade - 1);
    for (int i = 0; i < hash->tamanho_diretorio; i++) {
        if (hash->diretorio[i] == balde_antigo && (i & mascara_novo)) {
            hash->diretorio[i] = balde_novo;
        }
    }

    for (int i = 0; i < qtd_temp; i++) {
        hash_inserir((HashExtensivel*)hash, temporario[i].chave, temporario[i].dado); 
    }
}

bool hash_inserir(HashExtensivel* ptr_hash, int chave, const char* dado) {
    HashInterno* hash = (HashInterno*)ptr_hash;
    if (hash_buscar(ptr_hash, chave, NULL)) return false; 

    int indice = funcao_hash(chave, hash->profundidade_global);
    Balde* b = hash->diretorio[indice];

    if (b->quantidade < TAMANHO_BALDE) {
        b->registros[b->quantidade].chave = chave;
        strncpy(b->registros[b->quantidade].dado, dado, 49);
        b->registros[b->quantidade].dado[49] = '\0';
        b->quantidade++;
        return true;
    } else {
        tratar_split(hash, indice);
        return hash_inserir(ptr_hash, chave, dado);
    }
}

bool hash_remover(HashExtensivel* ptr_hash, int chave) {
    HashInterno* hash = (HashInterno*)ptr_hash;
    int indice = funcao_hash(chave, hash->profundidade_global);
    Balde* b = hash->diretorio[indice];

    for (int i = 0; i < b->quantidade; i++) {
        if (b->registros[i].chave == chave) {
            b->registros[i] = b->registros[b->quantidade - 1];
            b->quantidade--;
            return true;
        }
    }
    return false;
}

void hash_destruir(HashExtensivel* ptr_hash) {
    HashInterno* hash = (HashInterno*)ptr_hash;
    for (int i = 0; i < hash->tamanho_diretorio; i++) {
        if (hash->diretorio[i] != NULL) {
            Balde* b = hash->diretorio[i];
            for(int j = i; j < hash->tamanho_diretorio; j++){
                if(hash->diretorio[j] == b) hash->diretorio[j] = NULL;
            }
            free(b);
        }
    }
    free(hash->diretorio);
    free(hash);
}