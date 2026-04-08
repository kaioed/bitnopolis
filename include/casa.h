#ifndef CASA_H
#define CASA_H

typedef void* Casa;

Casa casa_criar(const char* cpf, const char* cep, char face, int num, const char* compl);
const char* casa_get_cpf(Casa c);
const char* casa_get_cep(Casa c);
char casa_get_face(Casa c);
int casa_get_num(Casa c);
const char* casa_get_compl(Casa c);
void casa_destruir(Casa c);

#endif