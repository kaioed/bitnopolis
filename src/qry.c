#include "../include/qry.h"
#include "../include/quadra.h"
#include "../include/hash_extensivel.h"
#include "../include/habitantes.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void converter_para_minusculas(char *str) {
    for (int i = 0; str[i]; i++) {
        str[i] = tolower((unsigned char)str[i]);
    }
}

void converter_para_maiusculas(char *str) {
    for (int i = 0; str[i]; i++) {
        str[i] = toupper((unsigned char)str[i]);
    }
}

typedef struct {
    int total_habitantes;
    int moradores;
    int sem_tetos;
    int homens;
    int mulheres;
} CensoStats;


void svg_desenhar_despejo(FILE *svg, double quadra_x, double quadra_y, double quadra_w, double quadra_h, char face, int num) {
    double dot_x, dot_y;
    calcular_coordenada_endereco(quadra_x, quadra_y, quadra_w, quadra_h, face, num, &dot_x, &dot_y);
    
    fprintf(svg, "\t<circle cx=\"%.2lf\" cy=\"%.2lf\" r=\"15\" fill=\"black\" />\n", dot_x, dot_y);
}

void svg_desenhar_cruz_morte(FILE *svg, double quadra_x, double quadra_y, double quadra_w, double quadra_h, char face, int num) {
    double dot_x, dot_y;
    calcular_coordenada_endereco(quadra_x, quadra_y, quadra_w, quadra_h, face, num, &dot_x, &dot_y);
    
    fprintf(svg, "\t<text x=\"%.2lf\" y=\"%.2lf\" fill=\"red\" font-size=\"40\" font-weight=\"bold\">+</text>\n", dot_x - 12.0, dot_y + 14.0);
}

void callback_censo(const char* chave, const char* dado, void* extra) {
    (void)chave;
    CensoStats* stats = (CensoStats*)extra;
    char nome[50], sobrenome[50], nasc[20], cep[50];
    char sexo;
    
    if (sscanf(dado, "%49[^;];%49[^;];%c;%19[^;];%*d;%49[^;]", nome, sobrenome, &sexo, nasc, cep) < 5) return;
    
    stats->total_habitantes++;
    
    if (sexo == 'M' || sexo == 'm') stats->homens++;
    else if (sexo == 'F' || sexo == 'f') stats->mulheres++;
    
    if (strcmp(cep, "SEM_TETO") == 0 || strcmp(cep, "none") == 0) stats->sem_tetos++;
    else stats->moradores++;
}

// Callback para desenhar o texto (CEP) no canto da quadra
void desenhar_textos_quadras(const char* chave, const char* dado, void* extra) {
    FILE* svg = (FILE*)extra;
    double x, y;
    
    if (sscanf(dado, "%lf;%lf", &x, &y) >= 2) {
        fprintf(svg, "\t<text x=\"%.2lf\" y=\"%.2lf\" font-size=\"12\" fill=\"black\">%s</text>\n", x + 5.0, y + 15.0, chave);
    }
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
    HashExtensivel* hq = (HashExtensivel*)hash_quadras;
    HashExtensivel* hh = (HashExtensivel*)hash_habitantes;

    if (svg) {
        fprintf(svg, "<svg xmlns=\"http://www.w3.org/2000/svg\" width=\"100%%\" height=\"100%%\" viewBox=\"-100 -100 5000 5000\">\n");
        hash_iterar(hq, desenhar_quadras, svg);
        hash_iterar(hq, desenhar_textos_quadras, svg); // <- Escreve os CEPs em cada quadra
    }
    
    char linha[256];
    while (fgets(linha, sizeof(linha), file)) {
        char comando[20];
        if (sscanf(linha, "%19s", comando) != 1) continue;
        
        converter_para_minusculas(comando);
        
        if (strcmp(comando, "rq") == 0) {
            char cep[TAMANHO_CHAVE];
            if (sscanf(linha, "%*s %49s", cep) == 1) {
                converter_para_maiusculas(cep); 
                fprintf(saida, "Comando: rq %s\n", cep);
                char dados_quadra[TAMANHO_DADO];
                
                if (hash_buscar(hq, cep, dados_quadra)) {
                    hash_remover(hq, cep);
                    fprintf(saida, "Quadra %s removida com sucesso.\n", cep);
                    if (svg) {
                        double x, y, w, h;
                        if (sscanf(dados_quadra, "%lf;%lf;%lf;%lf", &x, &y, &w, &h) == 4) {
                            double centro_x = x + (w / 2) - 10;
                            double centro_y = y + (h / 2) + 10;
                            fprintf(svg, "  <text x=\"%lf\" y=\"%lf\" fill=\"red\" font-size=\"40\" font-weight=\"bold\">X</text>\n", centro_x, centro_y);
                        }
                    }
                } else {
                    fprintf(saida, "Aviso: Quadra %s nao encontrada.\n", cep);
                }
            }
        }
        else if (strcmp(comando, "pq") == 0) {
            char cep[TAMANHO_CHAVE];
            if (sscanf(linha, "%*s %49s", cep) == 1) {
                converter_para_maiusculas(cep); 
                fprintf(saida, "Comando: Pq %s\n", cep);
            }
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
            if (sscanf(linha, "%*s %49s", cpf) == 1) {
                fprintf(saida, "Comando: h? %s\n", cpf);
                char dados_habitante[TAMANHO_DADO];
                if (hash_buscar(hh, cpf, dados_habitante)) {
                    fprintf(saida, "  Dados: %s\n", dados_habitante);
                } else {
                    fprintf(saida, "  Habitante %s nao encontrado.\n", cpf);
                }
            }
        }
        else if (strcmp(comando, "nasc") == 0) {
            char cpf[TAMANHO_CHAVE], nome[50], sobrenome[50], nasc[20];
            char sexo;
            if (sscanf(linha, "%*s %49s %49s %49s %c %19s", cpf, nome, sobrenome, &sexo, nasc) == 5) {
                fprintf(saida, "Comando: nasc %s\n", cpf);
                char dados_habitante[TAMANHO_DADO];
                snprintf(dados_habitante, sizeof(dados_habitante), "%s;%s;%c;%s;0;none;X;0;none", nome, sobrenome, sexo, nasc);
                
                if (hash_inserir(hh, cpf, dados_habitante)) {
                    fprintf(saida, "  Habitante %s registrado.\n", cpf);
                } else {
                    fprintf(saida, "  Erro: CPF %s ja existe.\n", cpf);
                }
            }
        }
        else if (strcmp(comando, "rip") == 0) {
            char cpf[TAMANHO_CHAVE];
            if (sscanf(linha, "%*s %49s", cpf) == 1) {
                fprintf(saida, "Comando: rip %s\n", cpf);
                char dados_habitante[TAMANHO_DADO];
                if (hash_buscar(hh, cpf, dados_habitante)) {
                    fprintf(saida, "  Falecido: %s\n", dados_habitante);
                    if (svg) {
                        char nome[50], sobrenome[50], nasc[20], cep[50], face[10];
                        char sexo;
                        int num;
                        
                        if (sscanf(dados_habitante, "%49[^;];%49[^;];%c;%19[^;];%*d;%49[^;];%9[^;];%d", nome, sobrenome, &sexo, nasc, cep, face, &num) >= 8) {
                            if (strcmp(cep, "SEM_TETO") != 0 && strcmp(cep, "none") != 0) {
                                char dados_quadra[TAMANHO_DADO];
                                if (hash_buscar(hq, cep, dados_quadra)) {
                                    double x, y, w, h;
                                    if (sscanf(dados_quadra, "%lf;%lf;%lf;%lf", &x, &y, &w, &h) == 4) {
                                        svg_desenhar_cruz_morte(svg, x, y, w, h, face[0], num);
                                    }
                                }
                            }
                        }
                    }
                    hash_remover(hh, cpf);
                } else {
                    fprintf(saida, "  Erro: Habitante %s nao encontrado.\n", cpf);
                }
            }
        }
        else if (strcmp(comando, "mud") == 0) {
            char cpf[TAMANHO_CHAVE], cep[TAMANHO_CHAVE], face[10], compl[50];
            int num;
            if (sscanf(linha, "%*s %49s %49s %9s %d %49s", cpf, cep, face, &num, compl) == 5) {
                converter_para_maiusculas(cep); 
                fprintf(saida, "Comando: mud %s para %s/%s/%d\n", cpf, cep, face, num);
                char dados_habitante[TAMANHO_DADO];
                if (hash_buscar(hh, cpf, dados_habitante)) {
                    char nome[50], sobrenome[50], nasc[20];
                    char sexo;
                    if (sscanf(dados_habitante, "%49[^;];%49[^;];%c;%19[^;]", nome, sobrenome, &sexo, nasc) >= 4) {
                        char novos_dados[TAMANHO_DADO];
                        snprintf(novos_dados, sizeof(novos_dados), "%s;%s;%c;%s;1;%s;%s;%d;%s", nome, sobrenome, sexo, nasc, cep, face, num, compl);
                        hash_remover(hh, cpf);
                        hash_inserir(hh, cpf, novos_dados);
                        
                        if (svg) {
                            char dados_quadra[TAMANHO_DADO];
                            if (hash_buscar(hq, cep, dados_quadra)) {
                                double x, y;
                                if (sscanf(dados_quadra, "%lf;%lf", &x, &y) == 2) {
                                    fprintf(svg, "  <rect x=\"%lf\" y=\"%lf\" width=\"25\" height=\"25\" fill=\"red\" />\n", x, y);
                                    fprintf(svg, "  <text x=\"%lf\" y=\"%lf\" font-size=\"20\">%s</text>\n", x+2, y+45, cpf);
                                }
                            }
                        }
                    }
                }
            }
        }
        else if (strcmp(comando, "dspj") == 0) {
            char cpf[TAMANHO_CHAVE];
            if (sscanf(linha, "%*s %49s", cpf) == 1) {
                fprintf(saida, "Comando: dspj %s\n", cpf);
                char dados_habitante[TAMANHO_DADO];
                if (hash_buscar(hh, cpf, dados_habitante)) {
                    char nome[50], sobrenome[50], nasc[20], cep[50], face[10];
                    char sexo;
                    int num;
                    
                    if (sscanf(dados_habitante, "%49[^;];%49[^;];%c;%19[^;];%*d;%49[^;];%9[^;];%d", nome, sobrenome, &sexo, nasc, cep, face, &num) >= 8) {
                        if (strcmp(cep, "SEM_TETO") != 0 && strcmp(cep, "none") != 0) {
                            fprintf(saida, "  Habitante despejado. Endereco antigo: %s\n", dados_habitante);
                            
                            if (svg) {
                                char dados_quadra[TAMANHO_DADO];
                                if (hash_buscar(hq, cep, dados_quadra)) {
                                 double x, y, w, h;
                                    if (sscanf(dados_quadra, "%lf;%lf;%lf;%lf", &x, &y, &w, &h) == 4) {
                                        svg_desenhar_despejo(svg, x, y, w, h, face[0], num);
                                    }
                                }
                            }
                            char novos_dados[TAMANHO_DADO];
                            snprintf(novos_dados, sizeof(novos_dados), "%s;%s;%c;%s;0;none;X;0;none", nome, sobrenome, sexo, nasc);
                            hash_remover(hh, cpf);
                            hash_inserir(hh, cpf, novos_dados);
                        } else {
                            fprintf(saida, "  Habitante %s ja era sem-teto.\n", cpf);
                        }
                    }
                }
            }
        }
    }
    
    if (svg) {
        fprintf(svg, "</svg>\n");
        printf("SVG do QRY gerado com sucesso em: %s\n", caminho_svg);
        fclose(svg);
    }
    
    fclose(file);
    fclose(saida);
}