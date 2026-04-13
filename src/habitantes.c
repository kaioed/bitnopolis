#include "../include/habitantes.h"
#include "../include/hash_extensivel.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char cpf[15];
    char nome[50];
    char sobrenome[50];
    char sexo;
    char nasc[15];
    bool is_morador;    
    char cep[30];
    char face[2];       
    int num;
    char compl[30];
} HabitanteImpl;

Habitante habitante_criar(const char* cpf, const char* nome, const char* sobrenome, char sexo, const char* nasc) {
    HabitanteImpl* h = (HabitanteImpl*) malloc(sizeof(HabitanteImpl));
    if (h != NULL) {
        strncpy(h->cpf, cpf, 14); h->cpf[14] = '\0';
        strncpy(h->nome, nome, 49); h->nome[49] = '\0';
        strncpy(h->sobrenome, sobrenome, 49); h->sobrenome[49] = '\0';
        h->sexo = sexo;
        strncpy(h->nasc, nasc, 14); h->nasc[14] = '\0';
    }
    return (Habitante) h;
}

const char* habitante_get_cpf(Habitante h) { return h ? ((HabitanteImpl*)h)->cpf : NULL; }
const char* habitante_get_nome(Habitante h) { return h ? ((HabitanteImpl*)h)->nome : NULL; }
const char* habitante_get_sobrenome(Habitante h) { return h ? ((HabitanteImpl*)h)->sobrenome : NULL; }
char habitante_get_sexo(Habitante h) { return h ? ((HabitanteImpl*)h)->sexo : '\0'; }
const char* habitante_get_nasc(Habitante h) { return h ? ((HabitanteImpl*)h)->nasc : NULL; }
const char* habitante_get_cep(Habitante h) { return h ? ((HabitanteImpl*)h)->cep : NULL; }
char* habitante_get_face(Habitante h) { return h ? ((HabitanteImpl*)h)->face : NULL; }
int habitante_get_num(Habitante h) { return h ? ((HabitanteImpl*)h)->num : 0; }
const char* habitante_get_compl(Habitante h) { return h ? ((HabitanteImpl*)h)->compl : NULL; }

void alterar_endereco_habitante(Habitante h, const char* cep, const char* face, int num, const char* compl) {
    if (h) {
        HabitanteImpl* impl = (HabitanteImpl*)h;
        strncpy(impl->cep, cep, 29); impl->cep[29] = '\0';
        strncpy(impl->face, face, 1); impl->face[1] = '\0';
        impl->num = num;
        strncpy(impl->compl, compl, 29); impl->compl[29] = '\0';
    }
}


void habitante_destruir(Habitante h) {
    if (h) free(h);
}