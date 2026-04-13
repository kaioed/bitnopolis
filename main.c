#include <stdio.h>
#include <stdlib.h>
#include "include/arg.h"
#include "include/geo.h"
#include "include/pm.h"
#include "include/hash_extensivel.h"
#include "include/quadra.h"



int main(int argc, char *argv[]){
    const char *entrada_geo     = obter_valor_opcao(argc, argv, "f");
    const char *saida_pasta     = obter_valor_opcao(argc, argv, "o");
    const char *entrada_qry     = obter_valor_opcao(argc, argv, "q");
    const char *prefixo_pasta   = obter_valor_opcao(argc, argv, "e");
    const char *entrada_pessoas = obter_valor_opcao(argc, argv, "pm");

    if(entrada_geo == NULL || saida_pasta == NULL || prefixo_pasta == NULL) {
        fprintf(stderr, "Uso: %s -f <entrada.geo> -o <saida_pasta> -e <prefixo_pasta> [-q <entrada.qry>] [-pm <entrada.pm>]\n", argv[0]);
        return EXIT_FAILURE;
    }

    HashExtensivel* hash_quadras = hash_criar(2, "hash_quadras.hf");
    if (!hash_quadras) {
        fprintf(stderr, "Erro ao criar hash de quadras.\n");
        return EXIT_FAILURE;
    }

    if (!geo_processar_arquivo(entrada_geo, saida_pasta, hash_quadras)) {
        fprintf(stderr, "Erro ao processar arquivo .geo.\n");
        hash_destruir(hash_quadras);
        return EXIT_FAILURE;
    }

    if (entrada_pessoas) {
        HashExtensivel* hash_habitantes = hash_criar(2, "hash_habitantes.hf");
        if (!hash_habitantes) {
            fprintf(stderr, "Erro ao criar hash de habitantes.\n");
            hash_destruir(hash_quadras);
            return EXIT_FAILURE;
        }

        if (!pm_processar_arquivo(entrada_pessoas, hash_habitantes)) {
            fprintf(stderr, "Erro ao processar arquivo .pm.\n");
            hash_destruir(hash_quadras);
            hash_destruir(hash_habitantes);
            return EXIT_FAILURE;    
        }



    
    
}