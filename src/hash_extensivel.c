#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/hash_extensivel.h"

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
    long* diretorio_offsets; // Guarda os bytes exatos no arquivo
    FILE* arquivo;           // Ponteiro para o arquivo binário aberto
} HashInterno;

int funcao_hash(int chave, int profundidade) {
    return chave & ((1 << profundidade) - 1);
}

void ler_balde(FILE* f, long offset, Balde* b) {
    fseek(f, offset, SEEK_SET);
    fread(b, sizeof(Balde), 1, f);
}

void escrever_balde(FILE* f, long offset, Balde* b) {
    fseek(f, offset, SEEK_SET);
    fwrite(b, sizeof(Balde), 1, f);
    fflush(f);
}

long anexar_balde(FILE* f, Balde* b) {
    fseek(f, 0, SEEK_END);
    long offset = ftell(f);
    fwrite(b, sizeof(Balde), 1, f);
    fflush(f);
    return offset;
}

HashExtensivel* hash_criar(int profundidade_inicial, const char* nome_arquivo) {
    HashInterno* hash = (HashInterno*)malloc(sizeof(HashInterno));
    hash->profundidade_global = profundidade_inicial;
    hash->tamanho_diretorio = 1 << profundidade_inicial;
    hash->diretorio_offsets = (long*)malloc(hash->tamanho_diretorio * sizeof(long));
    
    hash->arquivo = fopen(nome_arquivo, "wb+");
    if (!hash->arquivo) {
        free(hash->diretorio_offsets);
        free(hash);
        return NULL;
    }

    Balde b_vazio;
    b_vazio.profundidade_local = profundidade_inicial;
    b_vazio.quantidade = 0;

    for (int i = 0; i < hash->tamanho_diretorio; i++) {
        hash->diretorio_offsets[i] = anexar_balde(hash->arquivo, &b_vazio);
    }
    
    return (HashExtensivel*)hash;
}

bool hash_buscar(HashExtensivel* ptr_hash, int chave, char* saida_dado) {
    HashInterno* hash = (HashInterno*)ptr_hash;
    int indice = funcao_hash(chave, hash->profundidade_global);
    long offset = hash->diretorio_offsets[indice];
    
    Balde b;
    ler_balde(hash->arquivo, offset, &b);

    for (int i = 0; i < b.quantidade; i++) {
        if (b.registros[i].chave == chave) {
            if (saida_dado != NULL) {
                strcpy(saida_dado, b.registros[i].dado);
            }
            return true;
        }
    }
    return false;
}

bool hash_inserir(HashExtensivel* ptr_hash, int chave, const char* dado);

void tratar_split(HashInterno* hash, int indice_cheio) {
    long offset_antigo = hash->diretorio_offsets[indice_cheio];
    
    Balde balde_antigo;
    ler_balde(hash->arquivo, offset_antigo, &balde_antigo);
    
    if (balde_antigo.profundidade_local == hash->profundidade_global) {
        int novo_tamanho = hash->tamanho_diretorio * 2;
        long* novo_diretorio = (long*)malloc(novo_tamanho * sizeof(long));
        
        for (int i = 0; i < hash->tamanho_diretorio; i++) {
            novo_diretorio[i] = hash->diretorio_offsets[i];
            novo_diretorio[i + hash->tamanho_diretorio] = hash->diretorio_offsets[i];
        }
        
        free(hash->diretorio_offsets);
        hash->diretorio_offsets = novo_diretorio;
        hash->tamanho_diretorio = novo_tamanho;
        hash->profundidade_global++;
    }

    int nova_profundidade = balde_antigo.profundidade_local + 1;
    balde_antigo.profundidade_local = nova_profundidade;
    
    Balde balde_novo;
    balde_novo.profundidade_local = nova_profundidade;
    balde_novo.quantidade = 0;

    Registro temporario[TAMANHO_BALDE];
    int qtd_temp = balde_antigo.quantidade;
    for(int i = 0; i < qtd_temp; i++) {
        temporario[i] = balde_antigo.registros[i];
    }
    balde_antigo.quantidade = 0; 

    long offset_novo = anexar_balde(hash->arquivo, &balde_novo);

    
    int mascara_novo = 1 << (nova_profundidade - 1);
    for (int i = 0; i < hash->tamanho_diretorio; i++) {
        if (hash->diretorio_offsets[i] == offset_antigo && (i & mascara_novo)) {
            hash->diretorio_offsets[i] = offset_novo;
        }
    }

   
    escrever_balde(hash->arquivo, offset_antigo, &balde_antigo);


    for (int i = 0; i < qtd_temp; i++) {
        hash_inserir((HashExtensivel*)hash, temporario[i].chave, temporario[i].dado); 
    }
}

bool hash_inserir(HashExtensivel* ptr_hash, int chave, const char* dado) {
    HashInterno* hash = (HashInterno*)ptr_hash;
    
    if (hash_buscar(ptr_hash, chave, NULL)) return false; 

    int indice = funcao_hash(chave, hash->profundidade_global);
    long offset = hash->diretorio_offsets[indice];
    
    
    Balde b;
    ler_balde(hash->arquivo, offset, &b);

    if (b.quantidade < TAMANHO_BALDE) {
       
        b.registros[b.quantidade].chave = chave;
        strncpy(b.registros[b.quantidade].dado, dado, 49);
        b.registros[b.quantidade].dado[49] = '\0';
        b.quantidade++;
        
        
        escrever_balde(hash->arquivo, offset, &b);
        return true;
    } else {
        tratar_split(hash, indice);
        return hash_inserir(ptr_hash, chave, dado);
    }
}

bool hash_remover(HashExtensivel* ptr_hash, int chave) {
    HashInterno* hash = (HashInterno*)ptr_hash;
    int indice = funcao_hash(chave, hash->profundidade_global);
    long offset = hash->diretorio_offsets[indice];
    
    
    Balde b;
    ler_balde(hash->arquivo, offset, &b);

    for (int i = 0; i < b.quantidade; i++) {
        if (b.registros[i].chave == chave) {
            b.registros[i] = b.registros[b.quantidade - 1];
            b.quantidade--;
            
            
            escrever_balde(hash->arquivo, offset, &b);
            return true;
        }
    }
    return false;
}

void hash_destruir(HashExtensivel* ptr_hash) {
    HashInterno* hash = (HashInterno*)ptr_hash;
    
    if (hash->arquivo) {
        fclose(hash->arquivo); 
    }
    
    free(hash->diretorio_offsets);
    free(hash);
}