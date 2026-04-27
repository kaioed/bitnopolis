#include "../include/qry.h"
#include "../include/quadra.h"
#include "../include/hash_extensivel.h"
#include "../include/habitantes.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int total_habitantes;
    int moradores;
    int sem_tetos;
    int homens;
    int mulheres;
} CensoStats;

void callback_censo(const char* chave, const char* dado, void* extra) {
    CensoStats* stats = (CensoStats*)extra;
    char nome[50], sobrenome[50], nasc[20], cep[50];
    char sexo;
    
    sscanf(dado, "%s %s %c %s %s", nome, sobrenome, &sexo, nasc, cep);
    
    stats->total_habitantes++;
    
    if (sexo == 'M' || sexo == 'm') stats->homens++;
    else if (sexo == 'F' || sexo == 'f') stats->mulheres++;
    
    if (strcmp(cep, "SEM_TETO") == 0) stats->sem_tetos++;
    else stats->moradores++;
}

void processar_qry(const char* caminho_qry, const char* caminho_saida, void* hash_quadras, void* hash_habitantes) {
    FILE* file = fopen(caminho_qry, "r");
    if (!file) return;

    FILE* saida = fopen(caminho_saida, "w");
    if (!saida) {
        fclose(file);
        return;
    }

    char caminho_svg[512];
    strncpy(caminho_svg, caminho_saida, sizeof(caminho_svg));
    char* ext = strrchr(caminho_svg, '.');
    if (ext) strcpy(ext, ".svg");
    else strcat(caminho_svg, ".svg");

    FILE* svg = fopen(caminho_svg, "w");
    if (svg) fprintf(svg, "<svg xmlns=\"http://www.w3.org/2000/svg\" width=\"2000\" height=\"2000\">\n");

    HashExtensivel* hq = (HashExtensivel*)hash_quadras;
    HashExtensivel* hh = (HashExtensivel*)hash_habitantes;

    char linha[256];
    while (fgets(linha, sizeof(linha), file)) {
        char comando[20];
        if (sscanf(linha, "%s", comando) != 1) continue;

        if (strcmp(comando, "rq") == 0) {
            char cep[TAMANHO_CHAVE];
            sscanf(linha, "%*s %s", cep);
            fprintf(saida, "Comando: rq %s\n", cep);

            char dados_quadra[TAMANHO_DADO];
            if (hash_buscar(hq, cep, dados_quadra)) {
                hash_remover(hq, cep);
                fprintf(saida, "Quadra %s removida com sucesso.\n", cep);
                if (svg) {
                    double x, y;
                    sscanf(dados_quadra, "%lf;%lf", &x, &y);
                    fprintf(svg, "<text x=\"%lf\" y=\"%lf\" fill=\"red\" font-size=\"14\">X</text>\n", x, y);
                }
            } else {
                fprintf(saida, "Aviso: Quadra %s nao encontrada.\n", cep);
            }
        }
        else if (strcmp(comando, "Pq") == 0) {
            char cep[TAMANHO_CHAVE];
            sscanf(linha, "%*s %s", cep);
            fprintf(saida, "Comando: Pq %s\n", cep);
        }
        else if (strcmp(comando, "censo") == 0) {
            fprintf(saida, "Comando: censo\n");
            CensoStats stats = {0, 0, 0, 0, 0};
            
            hash_iterar(hh, callback_censo, &stats);

            fprintf(saida, "Estatisticas do Censo:\n");
            fprintf(saida, "  Total Habitantes: %d\n", stats.total_habitantes);
            fprintf(saida, "  Moradores: %d | Sem-tetos: %d\n", stats.moradores, stats.sem_tetos);
            fprintf(saida, "  Homens: %d | Mulheres: %d\n", stats.homens, stats.mulheres);
        }
        else if (strcmp(comando, "h?") == 0) {
            char cpf[TAMANHO_CHAVE];
            sscanf(linha, "%*s %s", cpf);
            fprintf(saida, "Comando: h? %s\n", cpf);

            char dados_habitante[TAMANHO_DADO];
            if (hash_buscar(hh, cpf, dados_habitante)) {
                fprintf(saida, "  Dados: %s\n", dados_habitante);
            } else {
                fprintf(saida, "  Habitante %s nao encontrado.\n", cpf);
            }
        }
        else if (strcmp(comando, "nasc") == 0) {
            char cpf[TAMANHO_CHAVE], nome[50], sobrenome[50], nasc[20];
            char sexo;
            sscanf(linha, "%*s %s %s %s %c %s", cpf, nome, sobrenome, &sexo, nasc);
            fprintf(saida, "Comando: nasc %s\n", cpf);

            char dados_habitante[TAMANHO_DADO];
            formatar_dados_habitante(nome, sobrenome, sexo, nasc, "SEM_TETO", "*", -1, "*", dados_habitante);
            
            if (hash_inserir(hh, cpf, dados_habitante)) {
                fprintf(saida, "  Habitante %s registrado.\n", cpf);
            } else {
                fprintf(saida, "  Erro: CPF %s ja existe.\n", cpf);
            }
        }
        else if (strcmp(comando, "rip") == 0) {
            char cpf[TAMANHO_CHAVE];
            sscanf(linha, "%*s %s", cpf);
            fprintf(saida, "Comando: rip %s\n", cpf);

            char dados_habitante[TAMANHO_DADO];
            if (hash_buscar(hh, cpf, dados_habitante)) {
                fprintf(saida, "  Falecido: %s\n", dados_habitante);
                if (svg) {
                    char nome[50], sobrenome[50], nasc[20], cep[50];
                    char sexo;
                    sscanf(dados_habitante, "%s %s %c %s %s", nome, sobrenome, &sexo, nasc, cep);
                    if (strcmp(cep, "SEM_TETO") != 0) {
                        char dados_quadra[TAMANHO_DADO];
                        if (hash_buscar(hq, cep, dados_quadra)) {
                            double x, y;
                            sscanf(dados_quadra, "%lf;%lf", &x, &y);
                            fprintf(svg, "<text x=\"%lf\" y=\"%lf\" fill=\"red\" font-size=\"14\">+</text>\n", x, y);
                        }
                    }
                }
                hash_remover(hh, cpf);
            } else {
                fprintf(saida, "  Erro: Habitante %s nao encontrado.\n", cpf);
            }
        }
        else if (strcmp(comando, "mud") == 0) {
            char cpf[TAMANHO_CHAVE], cep[TAMANHO_CHAVE], face[10], compl[50];
            int num;
            sscanf(linha, "%*s %s %s %s %d %s", cpf, cep, face, &num, compl);
            fprintf(saida, "Comando: mud %s para %s/%s/%d\n", cpf, cep, face, num);

            char dados_habitante[TAMANHO_DADO];
            if (hash_buscar(hh, cpf, dados_habitante)) {
                char nome[50], sobrenome[50], nasc[20];
                char sexo;
                sscanf(dados_habitante, "%s %s %c %s", nome, sobrenome, &sexo, nasc);
                
                formatar_dados_habitante(nome, sobrenome, sexo, nasc, cep, face, num, compl, dados_habitante);
                hash_remover(hh, cpf);
                hash_inserir(hh, cpf, dados_habitante);

                if (svg) {
                    char dados_quadra[TAMANHO_DADO];
                    if (hash_buscar(hq, cep, dados_quadra)) {
                        double x, y;
                        sscanf(dados_quadra, "%lf;%lf", &x, &y);
                        fprintf(svg, "<rect x=\"%lf\" y=\"%lf\" width=\"10\" height=\"10\" fill=\"red\" />\n", x, y);
                        fprintf(svg, "<text x=\"%lf\" y=\"%lf\" font-size=\"8\">%s</text>\n", x+2, y+8, cpf);
                    }
                }
            }
        }
        else if (strcmp(comando, "dspj") == 0) {
            char cpf[TAMANHO_CHAVE];
            sscanf(linha, "%*s %s", cpf);
            fprintf(saida, "Comando: dspj %s\n", cpf);

            char dados_habitante[TAMANHO_DADO];
            if (hash_buscar(hh, cpf, dados_habitante)) {
                char nome[50], sobrenome[50], nasc[20], cep[50];
                char sexo;
                sscanf(dados_habitante, "%s %s %c %s %s", nome, sobrenome, &sexo, nasc, cep);
                
                if (strcmp(cep, "SEM_TETO") != 0) {
                    fprintf(saida, "  Habitante despejado. Endereco antigo: %s\n", dados_habitante);
                    
                    if (svg) {
                        char dados_quadra[TAMANHO_DADO];
                        if (hash_buscar(hq, cep, dados_quadra)) {
                            double x, y;
                            sscanf(dados_quadra, "%lf;%lf", &x, &y);
                            fprintf(svg, "<circle cx=\"%lf\" cy=\"%lf\" r=\"5\" fill=\"black\" />\n", x, y);
                        }
                    }

                    formatar_dados_habitante(nome, sobrenome, sexo, nasc, "SEM_TETO", "*", -1, "*", dados_habitante);
                    hash_remover(hh, cpf);
                    hash_inserir(hh, cpf, dados_habitante);
                } else {
                    fprintf(saida, "  Habitante %s ja era sem-teto.\n", cpf);
                }
            }
        }
    }

    if (svg) {
        fprintf(svg, "</svg>\n");
        fclose(svg);
    }
    fclose(file);
    fclose(saida);
}