#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

#if defined(_WIN32)
#include <direct.h>
#endif

#include "arg.h"
#include "geo.h"
#include "hash_extensivel.h"
#include "pm.h"
#include "qry.h"
#include "quadra.h"


static const char *obter_nome_arquivo(const char *caminho) {
    const char *nome = strrchr(caminho, '/');
    const char *nome_windows = strrchr(caminho, '\\');

    if (nome_windows && (!nome || nome_windows > nome)) {
        nome = nome_windows;
    }

    return nome ? nome + 1 : caminho;
}

static void copiar_nome_sem_extensao(const char *caminho, char *saida, size_t tamanho_saida) {
    const char *nome = obter_nome_arquivo(caminho);

    strncpy(saida, nome, tamanho_saida - 1);
    saida[tamanho_saida - 1] = '\0';

    char *ponto = strrchr(saida, '.');
    if (ponto) {
        *ponto = '\0';
    }
}

void criar_diretorio(const char *caminho) {
    struct stat st = {0};
    if (stat(caminho, &st) == -1) {
        
        #if defined(_WIN32)
            _mkdir(caminho);
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

    char caminho_geo[512], caminho_svg[512], caminho_hash_quadras[512];
    snprintf(caminho_geo, sizeof(caminho_geo), "%s/%s", prefixo_pasta, entrada_geo);

    char nome_geo_sem_ext[256];
    copiar_nome_sem_extensao(entrada_geo, nome_geo_sem_ext, sizeof(nome_geo_sem_ext));

    snprintf(caminho_svg, sizeof(caminho_svg), "%s/%s.svg", saida_pasta, nome_geo_sem_ext);
    snprintf(caminho_hash_quadras, sizeof(caminho_hash_quadras), "%s/%s.hs", saida_pasta, nome_geo_sem_ext);

    HashExtensivel* hash_quadras = hash_criar(2, caminho_hash_quadras);
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

        char nome_hash[512];

        if (entrada_qry) {
            char nome_qry_sem_ext[256];
            copiar_nome_sem_extensao(entrada_qry, nome_qry_sem_ext, sizeof(nome_qry_sem_ext));

            snprintf(nome_hash, sizeof(nome_hash), "%s/%s.hash", saida_pasta, nome_qry_sem_ext);
        } else {
            snprintf(nome_hash, sizeof(nome_hash), "%s/default.hs", saida_pasta);
        }

        hash_habitantes = hash_criar(2, nome_hash);
        if (hash_habitantes) {
            pm_processar_arquivo(caminho_pm, hash_habitantes);
        } else {
            printf("Aviso: Nao foi possivel criar o arquivo hash para os habitantes.\n");
        }
    }

    if (entrada_qry) {
        char caminho_qry[512], caminho_saida_qry[512];
        snprintf(caminho_qry, sizeof(caminho_qry), "%s/%s", prefixo_pasta, entrada_qry);

        char nome_qry_sem_ext[256];
        copiar_nome_sem_extensao(entrada_qry, nome_qry_sem_ext, sizeof(nome_qry_sem_ext));

        snprintf(caminho_saida_qry, sizeof(caminho_saida_qry), "%s/%s-%s.txt",
                 saida_pasta, nome_geo_sem_ext, nome_qry_sem_ext);
        
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
