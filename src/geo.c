#include "../include/geo.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char cep[30];
    double x, y, w, h;
    char cfill[30];
    char cstrk[30];
    double sw;
} QuadraImpl;

Quadra geo_criar_quadra(const char* cep, double x, double y, double w, double h, const char* cfill, const char* cstrk, double sw) {
    QuadraImpl* q = (QuadraImpl*) malloc(sizeof(QuadraImpl));
    if (q != NULL) {
        strncpy(q->cep, cep, 29); q->cep[29] = '\0';
        q->x = x; q->y = y; q->w = w; q->h = h;
        strncpy(q->cfill, cfill, 29); q->cfill[29] = '\0';
        strncpy(q->cstrk, cstrk, 29); q->cstrk[29] = '\0';
        q->sw = sw;
    }
    return (Quadra) q;
}

const char* geo_get_cep(Quadra q) { return q ? ((QuadraImpl*)q)->cep : NULL; }
double geo_get_x(Quadra q) { return q ? ((QuadraImpl*)q)->x : 0.0; }
double geo_get_y(Quadra q) { return q ? ((QuadraImpl*)q)->y : 0.0; }
double geo_get_w(Quadra q) { return q ? ((QuadraImpl*)q)->w : 0.0; }
double geo_get_h(Quadra q) { return q ? ((QuadraImpl*)q)->h : 0.0; }
const char* geo_get_cfill(Quadra q) { return q ? ((QuadraImpl*)q)->cfill : NULL; }
const char* geo_get_cstrk(Quadra q) { return q ? ((QuadraImpl*)q)->cstrk : NULL; }
double geo_get_sw(Quadra q) { return q ? ((QuadraImpl*)q)->sw : 0.0; }

void geo_destruir_quadra(Quadra q) {
    if (q) free(q);
}

bool geo_processar_arquivo(const char* caminho_arquivo, void* hash_quadras) {
    if (caminho_arquivo == NULL) return false;

    FILE* file = fopen(caminho_arquivo, "r");
    if (file == NULL) return false;

    char comando[10];
    char cor_preenchimento[30] = "none"; 
    char cor_borda[30] = "black";
    double espessura_borda = 1.0; 


        if (strcmp(comando, "cq") == 0) {
            fscanf(file, "%lf %s %s", &espessura_borda, cor_preenchimento, cor_borda);
        } 
        else if (strcmp(comando, "q") == 0) {
            char cep[30];
            double x, y, w, h;
            fscanf(file, "%s %lf %lf %lf %lf", cep, &x, &y, &w, &h);
            
            
            Quadra q = geo_criar_quadra(cep, x, y, w, h, cor_preenchimento, cor_borda, espessura_borda);
            
            geo_destruir_quadra(q); 
        }
        
        fclose(file);
        return true;
    }