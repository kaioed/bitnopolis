#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "include/arg.h"
#include "include/geo.h"
#include "include/pm.h"
#include "include/qry.h"
#include "include/hash_extensivel.h"
#include "include/quadra.h"


void criar_diretorio(const char *caminho) {
    struct stat st = {0};
    if (stat(caminho, &st) == -1) {
        
        #if defined(_WIN32)
            mkdir(caminho);
        #else
            mkdir(caminho, 0777);
        #endif
    }
}

int main(int argc, char *argv[]) {
    const char *entrada_geo     = obter_valor_opcao(argc, argv, "f");
    const char *saida_pasta     = obter_valor_opcao(argc, argv, "o");
    const char *entrada_qry     = obter_valor_opcao(argc, argv, "q");
    const char *prefixo_pasta   = obter_valor_opcao(argc, argv, "e");
    const char *entrada_pessoas = obter_valor_opcao(argc, argv, "pm");

    if (entrada_geo == NULL || saida_pasta == NULL || prefixo_pasta == NULL) {
        printf("Erro: Faltam argumentos obrigatorios (-f, -o, ou -e).\n");
        return EXIT_FAILURE;
    }

    
    criar_diretorio(saida_pasta);

    char caminho_geo[512], caminho_svg[512];
    snprintf(caminho_geo, sizeof(caminho_geo), "%s/%s", prefixo_pasta, entrada_geo);
    snprintf(caminho_svg, sizeof(caminho_svg), "%s/%s.svg", saida_pasta, entrada_geo);

    HashExtensivel* hash_quadras = hash_criar(2, "hash_quadras.hf");
    if (!hash_quadras) {
        printf("Erro: Nao foi possivel criar o arquivo hash para as quadras.\n");
        return EXIT_FAILURE;
    }

    if (!geo_processar_arquivo(caminho_geo, caminho_svg, hash_quadras)) {
        printf("Erro: Falha ao processar o arquivo GEO. Verifique se o arquivo '%s' existe.\n", caminho_geo);
        hash_destruir(hash_quadras);
        return EXIT_FAILURE;
    }else {
        printf("Arquivo GEO processado com sucesso. SVG gerado em: %s\n", caminho_svg);
    }

    HashExtensivel* hash_habitantes = NULL;
    if (entrada_pessoas) {
        char caminho_pm[512];
        snprintf(caminho_pm, sizeof(caminho_pm), "%s/%s", prefixo_pasta, entrada_pessoas);
        
        hash_habitantes = hash_criar(2, "hash_habitantes.hf");
        if (hash_habitantes) {
            pm_processar_arquivo(caminho_pm, hash_habitantes);
        } else {
            printf("Aviso: Nao foi possivel criar o arquivo hash para os habitantes.\n");
        }
    }

    if (entrada_qry) {
        char caminho_qry[512], caminho_saida_qry[512];
        snprintf(caminho_qry, sizeof(caminho_qry), "%s/%s", prefixo_pasta, entrada_qry);
        
        
        const char *nome_qry = strrchr(entrada_qry, '/');
        if (nome_qry) nome_qry++;
        else nome_qry = entrada_qry;
        
        snprintf(caminho_saida_qry, sizeof(caminho_saida_qry), "%s/%s.txt", saida_pasta, nome_qry);
        
        printf("Processando arquivo QRY '%s'...\n", entrada_qry);
        processar_qry(caminho_qry, caminho_saida_qry, hash_quadras, hash_habitantes);
    }else {
        printf("Nenhum arquivo QRY fornecido. Pulando processamento de consultas.\n");
    }
    printf("finalizando processamento...\n");

    hash_destruir(hash_quadras);
    if (hash_habitantes) hash_destruir(hash_habitantes);

    printf("Processamento concluido com sucesso! Os arquivos foram gerados na pasta '%s'.\n", saida_pasta);
    return EXIT_SUCCESS;
}