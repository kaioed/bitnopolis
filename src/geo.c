#include "../include/geo.h"
#include "../include/quadra.h"
#include "../include/hash_extensivel.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

static void converter_para_maiusculas(char *str) {
    for (int i = 0; str[i] != '\0'; i++) {
        str[i] = (char)toupper((unsigned char)str[i]);
    }
}

bool geo_processar_arquivo(const char* caminho_arquivo, const char* caminho_svg, void* hash_quadras) {
    if (caminho_arquivo == NULL || caminho_svg == NULL) return false;

    FILE* file = fopen(caminho_arquivo, "r");
    if (file == NULL) return false;

    FILE* svg = fopen(caminho_svg, "w");
    if (svg == NULL) {
        fclose(file);
        return false;
    }

    fprintf(svg, "<svg xmlns=\"http://www.w3.org/2000/svg\" width=\"100%%\" height=\"100%%\" viewBox=\"-100 -100 5000 5000\">\n");

    char comando[10];
    char cor_preenchimento[30] = "none"; 
    char cor_borda[30] = "black";
    double espessura_borda = 1.0; 

    while (fscanf(file, "%s", comando) != EOF) {
        if (strcmp(comando, "cq") == 0) {
            char espessura_str[30];
            if (fscanf(file, "%29s %29s %29s", espessura_str, cor_preenchimento, cor_borda) == 3) {
                espessura_borda = strtod(espessura_str, NULL);
            }
        } 
        else if (strcmp(comando, "q") == 0) {
            char cep[30];
            double x, y, w, h;
            fscanf(file, "%29s %lf %lf %lf %lf", cep, &x, &y, &w, &h);
            converter_para_maiusculas(cep);
            
            char dados_quadra[150];
            snprintf(dados_quadra, sizeof(dados_quadra), "%lf;%lf;%lf;%lf;%s;%s;%lf", 
                     x, y, w, h, cor_preenchimento, cor_borda, espessura_borda);
            
            hash_inserir((HashExtensivel*)hash_quadras, cep, dados_quadra);
            
            fprintf(svg, "  <rect x=\"%.lf\" y=\"%.lf\" width=\"%.lf\" height=\"%.lf\" fill=\"%s\" stroke=\"%s\" stroke-width=\"%.lf\" />\n", 
                    x, y, w, h, cor_preenchimento, cor_borda, espessura_borda);
        }
    }
    
    fprintf(svg, "</svg>\n");
    
    fclose(file);
    fclose(svg);
    return true;
}