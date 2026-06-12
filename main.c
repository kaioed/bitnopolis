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

static char *copiar_nome_sem_extensao(const char *caminho) {
    const char *nome = obter_nome_arquivo(caminho);
    const char *ponto = strrchr(nome, '.');
    size_t tamanho = ponto ? (size_t)(ponto - nome) : strlen(nome);

    char *saida = (char *)malloc(tamanho + 1);
    if (!saida) {
        return NULL;
    }

    memcpy(saida, nome, tamanho);
    saida[tamanho] = '\0';
    return saida;
}

static int tamanho_estourou(size_t atual, size_t adicional) {
    return atual > (size_t)-1 - adicional;
}

static char *montar_caminho(const char *pasta, const char *nome, const char *sufixo) {
    size_t tam_pasta = strlen(pasta);
    size_t tam_nome = strlen(nome);
    size_t tam_sufixo = strlen(sufixo);

    if (tamanho_estourou(tam_pasta, 1) ||
        tamanho_estourou(tam_pasta + 1, tam_nome) ||
        tamanho_estourou(tam_pasta + 1 + tam_nome, tam_sufixo) ||
        tamanho_estourou(tam_pasta + 1 + tam_nome + tam_sufixo, 1)) {
        return NULL;
    }

    size_t tamanho = tam_pasta + 1 + tam_nome + tam_sufixo + 1;
    char *resultado = (char *)malloc(tamanho);
    if (!resultado) {
        return NULL;
    }

    char *pos = resultado;
    memcpy(pos, pasta, tam_pasta);
    pos += tam_pasta;
    *pos++ = '/';
    memcpy(pos, nome, tam_nome);
    pos += tam_nome;
    memcpy(pos, sufixo, tam_sufixo);
    pos += tam_sufixo;
    *pos = '\0';

    return resultado;
}

static char *montar_saida_qry(const char *pasta, const char *nome_geo, const char *nome_qry) {
    const char *sufixo = ".txt";
    size_t tam_pasta = strlen(pasta);
    size_t tam_geo = strlen(nome_geo);
    size_t tam_qry = strlen(nome_qry);
    size_t tam_sufixo = strlen(sufixo);

    if (tamanho_estourou(tam_pasta, 1) ||
        tamanho_estourou(tam_pasta + 1, tam_geo) ||
        tamanho_estourou(tam_pasta + 1 + tam_geo, 1) ||
        tamanho_estourou(tam_pasta + 1 + tam_geo + 1, tam_qry) ||
        tamanho_estourou(tam_pasta + 1 + tam_geo + 1 + tam_qry, tam_sufixo) ||
        tamanho_estourou(tam_pasta + 1 + tam_geo + 1 + tam_qry + tam_sufixo, 1)) {
        return NULL;
    }

    size_t tamanho = tam_pasta + 1 + tam_geo + 1 + tam_qry + tam_sufixo + 1;
    char *resultado = (char *)malloc(tamanho);
    if (!resultado) {
        return NULL;
    }

    char *pos = resultado;
    memcpy(pos, pasta, tam_pasta);
    pos += tam_pasta;
    *pos++ = '/';
    memcpy(pos, nome_geo, tam_geo);
    pos += tam_geo;
    *pos++ = '-';
    memcpy(pos, nome_qry, tam_qry);
    pos += tam_qry;
    memcpy(pos, sufixo, tam_sufixo);
    pos += tam_sufixo;
    *pos = '\0';

    return resultado;
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
    int status = EXIT_FAILURE;

    char *caminho_geo = NULL;
    char *caminho_svg = NULL;
    char *caminho_hash_quadras = NULL;
    char *nome_geo_sem_ext = NULL;
    char *caminho_pm = NULL;
    char *nome_hash = NULL;
    char *nome_qry_sem_ext = NULL;
    char *caminho_qry = NULL;
    char *caminho_saida_qry = NULL;
    HashExtensivel* hash_quadras = NULL;
    HashExtensivel* hash_habitantes = NULL;

    if (entrada_geo == NULL || saida_pasta == NULL || prefixo_pasta == NULL) {
        printf("Erro: Faltam argumentos obrigatorios (-f, -o, ou -e).\n");
        goto finalizar;
    }

    
    criar_diretorio(saida_pasta);

    nome_geo_sem_ext = copiar_nome_sem_extensao(entrada_geo);
    caminho_geo = montar_caminho(prefixo_pasta, entrada_geo, "");
    caminho_svg = montar_caminho(saida_pasta, nome_geo_sem_ext ? nome_geo_sem_ext : "", ".svg");
    caminho_hash_quadras = montar_caminho(saida_pasta, nome_geo_sem_ext ? nome_geo_sem_ext : "", ".hs");

    if (!nome_geo_sem_ext || !caminho_geo || !caminho_svg || !caminho_hash_quadras) {
        printf("Erro: Memoria insuficiente ao montar caminhos de arquivo.\n");
        goto finalizar;
    }

    hash_quadras = hash_criar(2, caminho_hash_quadras);
    if (!hash_quadras) {
        printf("Erro: Nao foi possivel criar o arquivo hash para as quadras.\n");
        goto finalizar;
    }

    if (!geo_processar_arquivo(caminho_geo, caminho_svg, hash_quadras)) {
        printf("Erro: Falha ao processar o arquivo GEO. Verifique se o arquivo '%s' existe.\n", caminho_geo);
        goto finalizar;
    }else {
        printf("Arquivo GEO processado com sucesso. SVG gerado em: %s\n", caminho_svg);
    }

    if (entrada_pessoas) {
        caminho_pm = montar_caminho(prefixo_pasta, entrada_pessoas, "");

        if (entrada_qry) {
            nome_qry_sem_ext = copiar_nome_sem_extensao(entrada_qry);
            nome_hash = montar_caminho(saida_pasta, nome_qry_sem_ext ? nome_qry_sem_ext : "", ".hash");
        } else {
            nome_hash = montar_caminho(saida_pasta, "default", ".hs");
        }

        if (!caminho_pm || !nome_hash || (entrada_qry && !nome_qry_sem_ext)) {
            printf("Erro: Memoria insuficiente ao montar caminhos de arquivo.\n");
            goto finalizar;
        }

        hash_habitantes = hash_criar(2, nome_hash);
        if (hash_habitantes) {
            pm_processar_arquivo(caminho_pm, hash_habitantes);
        } else {
            printf("Aviso: Nao foi possivel criar o arquivo hash para os habitantes.\n");
        }
    }

    if (entrada_qry) {
        caminho_qry = montar_caminho(prefixo_pasta, entrada_qry, "");

        if (!nome_qry_sem_ext) {
            nome_qry_sem_ext = copiar_nome_sem_extensao(entrada_qry);
        }
        caminho_saida_qry = montar_saida_qry(saida_pasta, nome_geo_sem_ext, nome_qry_sem_ext ? nome_qry_sem_ext : "");
        if (!caminho_qry || !nome_qry_sem_ext || !caminho_saida_qry) {
            printf("Erro: Memoria insuficiente ao montar caminhos de arquivo.\n");
            goto finalizar;
        }
        
        printf("Processando arquivo QRY '%s'...\n", entrada_qry);
        processar_qry(caminho_qry, caminho_saida_qry, hash_quadras, hash_habitantes);
    }else {
        printf("Nenhum arquivo QRY fornecido. Pulando processamento de consultas.\n");
    }
    printf("finalizando processamento...\n");

    status = EXIT_SUCCESS;

    printf("Processamento concluido com sucesso! Os arquivos foram gerados na pasta '%s'.\n", saida_pasta);

finalizar:
    if (hash_quadras) hash_destruir(hash_quadras);
    if (hash_habitantes) hash_destruir(hash_habitantes);
    free(caminho_geo);
    free(caminho_svg);
    free(caminho_hash_quadras);
    free(nome_geo_sem_ext);
    free(caminho_pm);
    free(nome_hash);
    free(nome_qry_sem_ext);
    free(caminho_qry);
    free(caminho_saida_qry);

    return status;
}
