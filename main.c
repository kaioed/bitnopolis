#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "include/arg.h"
#include "include/geo.h"
#include "include/pm.h"
#include "include/qry.h"
#include "include/hash_extensivel.h"
#include "include/quadra.h"

int main(int argc, char *argv[]){
    const char *entrada_geo     = obter_valor_opcao(argc, argv, "f");
    const char *saida_pasta     = obter_valor_opcao(argc, argv, "o");
    const char *entrada_qry     = obter_valor_opcao(argc, argv, "q");
    const char *prefixo_pasta   = obter_valor_opcao(argc, argv, "e");
    const char *entrada_pessoas = obter_valor_opcao(argc, argv, "pm");

    if(entrada_geo == NULL || saida_pasta == NULL || prefixo_pasta == NULL) {
        return EXIT_FAILURE;
    }

    char caminho_geo[512], caminho_svg[512];
    snprintf(caminho_geo, sizeof(caminho_geo), "%s/%s", prefixo_pasta, entrada_geo);
    snprintf(caminho_svg, sizeof(caminho_svg), "%s/geo_output.svg", saida_pasta);

    HashExtensivel* hash_quadras = hash_criar(2, "hash_quadras.hf");
    if (!hash_quadras) return EXIT_FAILURE;

    if (!geo_processar_arquivo(caminho_geo, caminho_svg, hash_quadras)) {
        hash_destruir(hash_quadras);
        return EXIT_FAILURE;
    }

    HashExtensivel* hash_habitantes = NULL;
    if (entrada_pessoas) {
        char caminho_pm[512];
        snprintf(caminho_pm, sizeof(caminho_pm), "%s/%s", prefixo_pasta, entrada_pessoas);
        
        hash_habitantes = hash_criar(2, "hash_habitantes.hf");
        if (hash_habitantes) {
            pm_processar_arquivo(caminho_pm, hash_habitantes);
        }
    }

    if (entrada_qry) {
        char caminho_qry[512], caminho_saida_qry[512];
        snprintf(caminho_qry, sizeof(caminho_qry), "%s/%s", prefixo_pasta, entrada_qry);
        snprintf(caminho_saida_qry, sizeof(caminho_saida_qry), "%s/saida_qry.txt", saida_pasta);
        
        processar_qry(caminho_qry, caminho_saida_qry, hash_quadras, hash_habitantes);
    }

    hash_destruir(hash_quadras);
    if(hash_habitantes) hash_destruir(hash_habitantes);

    return EXIT_SUCCESS;
}