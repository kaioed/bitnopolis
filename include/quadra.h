#ifndef QUADRA_H
#define QUADRA_H

#include <stdbool.h>

typedef void* Quadra;

Quadra quadra_criar(const char* cep, double x, double y, double w, double h, const char* cfill, const char* cstrk, double sw);
const char* quadra_get_cep(Quadra q);
double quadra_get_x(Quadra q);
double quadra_get_y(Quadra q);
double quadra_get_w(Quadra q);
double quadra_get_h(Quadra q);
const char* quadra_get_cfill(Quadra q);
const char* quadra_get_cstrk(Quadra q);
double quadra_get_sw(Quadra q);
void quadra_destruir(Quadra q);

#endif