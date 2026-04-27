#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/habitantes.h"


typedef struct {
    char cpf[20];
    char nome[100];
    char sobrenome[100];
    char sexo;
    char nasc[20];
    char cep[20];
    char face[10];
    int num;
    char compl[100];
} HabitanteStruct;

Habitante habitante_criar(const char* cpf, const char* nome, const char* sobrenome, char sexo, const char* nasc) {
    HabitanteStruct* h = (HabitanteStruct*) malloc(sizeof(HabitanteStruct));
    if (!h) return NULL;

    strncpy(h->cpf, cpf, 19); h->cpf[19] = '\0';
    strncpy(h->nome, nome, 99); h->nome[99] = '\0';
    strncpy(h->sobrenome, sobrenome, 99); h->sobrenome[99] = '\0';
    h->sexo = sexo;
    strncpy(h->nasc, nasc, 19); h->nasc[19] = '\0';

    h->cep[0] = '\0';
    h->face[0] = '\0';
    h->num = 0;
    h->compl[0] = '\0';

    return (Habitante) h;
}

const char* habitante_get_cpf(Habitante h) {
    if (!h) return NULL;
    return ((HabitanteStruct*)h)->cpf;
}

const char* habitante_get_nome(Habitante h) {
    if (!h) return NULL;
    return ((HabitanteStruct*)h)->nome;
}

const char* habitante_get_sobrenome(Habitante h) {
    if (!h) return NULL;
    return ((HabitanteStruct*)h)->sobrenome;
}

char habitante_get_sexo(Habitante h) {
    if (!h) return '\0';
    return ((HabitanteStruct*)h)->sexo;
}

const char* habitante_get_nasc(Habitante h) {
    if (!h) return NULL;
    return ((HabitanteStruct*)h)->nasc;
}

const char* habitante_get_cep(Habitante h) {
    if (!h) return NULL;
    return ((HabitanteStruct*)h)->cep;
}

char* habitante_get_face(Habitante h) {
    if (!h) return NULL;
    return ((HabitanteStruct*)h)->face;
}

int habitante_get_num(Habitante h) {
    if (!h) return 0;
    return ((HabitanteStruct*)h)->num;
}

const char* habitante_get_compl(Habitante h) {
    if (!h) return NULL;
    return ((HabitanteStruct*)h)->compl;
}

void alterar_endereco_habitante(Habitante h, const char* cep, const char* face, int num, const char* compl) {
    if (!h) return;
    HabitanteStruct* s = (HabitanteStruct*)h;

    if (cep) { strncpy(s->cep, cep, 19); s->cep[19] = '\0'; }
    if (face) { strncpy(s->face, face, 9); s->face[9] = '\0'; }
    s->num = num;
    if (compl) { strncpy(s->compl, compl, 99); s->compl[99] = '\0'; }
}

void habitante_destruir(Habitante h) {
    if (h) {
        free(h);
    }
}

void formatar_dados_habitante(const char* nome, const char* sobrenome, char sexo, const char* nasc, const char* cep, const char* face, int num, const char* compl, char* saida) {
    if (!saida) return;
    
    sprintf(saida, "Nome: %s %s | Sexo: %c | Nasc: %s | CEP: %s | Face: %s | Num: %d | Compl: %s",
            nome, sobrenome, sexo, nasc, cep, face, num, (compl ? compl : "N/A"));
}