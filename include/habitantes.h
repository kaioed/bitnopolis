#ifndef HABITANTES_H
#define HABITANTES_H

typedef void* Habitante;

Habitante habitante_criar(const char* cpf, const char* nome, const char* sobrenome, char sexo, const char* nasc);
const char* habitante_get_cpf(Habitante h);
const char* habitante_get_nome(Habitante h);
const char* habitante_get_sobrenome(Habitante h);
char habitante_get_sexo(Habitante h);
const char* habitante_get_nasc(Habitante h);
void habitante_destruir(Habitante h);

#endif