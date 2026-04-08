#include "../include/casa.h"
#include <stdlib.h>
#include <string.h>

typedef struct {
    char cpf[15];
    char cep[30];
    char face;
    int num;
    char compl[50];
} CasaImpl;

Casa casa_criar(const char* cpf, const char* cep, char face, int num, const char* compl) {
    CasaImpl* c = (CasaImpl*) malloc(sizeof(CasaImpl));
    if (c != NULL) {
        strncpy(c->cpf, cpf, 14); c->cpf[14] = '\0';
        strncpy(c->cep, cep, 29); c->cep[29] = '\0';
        c->face = face;
        c->num = num;
        strncpy(c->compl, compl, 49); c->compl[49] = '\0';
    }
    return (Casa) c;
}

const char* casa_get_cpf(Casa c) { return c ? ((CasaImpl*)c)->cpf : NULL; }
const char* casa_get_cep(Casa c) { return c ? ((CasaImpl*)c)->cep : NULL; }
char casa_get_face(Casa c) { return c ? ((CasaImpl*)c)->face : '\0'; }
int casa_get_num(Casa c) { return c ? ((CasaImpl*)c)->num : 0; }
const char* casa_get_compl(Casa c) { return c ? ((CasaImpl*)c)->compl : NULL; }

void casa_destruir(Casa c) {
    if (c) free(c);
}