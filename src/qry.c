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

typedef struct {
    const char* cep_alvo;
    HashExtensivel* hash_habitantes;
    FILE* arquivo_saida;
} ContextoRemocaoQuadra;

typedef struct {
    const char* cep_alvo;
    HashExtensivel* hash_quadras;
    FILE* arquivo_saida;
    FILE* arquivo_svg;
} ContextoContadorMoradores;

void svg_desenhar_despejo(FILE *svg, double quadra_x, double quadra_y, double quadra_w, double quadra_h, char face, int num) {
    double dot_x, dot_y;
    calcular_coordenada_endereco(quadra_x, quadra_y, quadra_w, quadra_h, face, num, &dot_x, &dot_y);
    
    fprintf(svg, "\t<circle cx=\"%.2lf\" cy=\"%.2lf\" r=\"15\" fill=\"black\" />\n", dot_x, dot_y);
}

void svg_desenhar_cruz_morte(FILE *svg, double quadra_x, double quadra_y, double quadra_w, double quadra_h, char face, int num) {
    double dot_x, dot_y;
    calcular_coordenada_endereco(quadra_x, quadra_y, quadra_w, quadra_h, face, num, &dot_x, &dot_y);
    
    // Adicionei a bolinha preta de fundo para dar destaque à cruz vermelha
    fprintf(svg, "\t<circle cx=\"%.2lf\" cy=\"%.2lf\" r=\"15\" fill=\"black\" />\n", dot_x, dot_y);
    fprintf(svg, "\t<text x=\"%.2lf\" y=\"%.2lf\" fill=\"red\" font-size=\"40\" font-weight=\"bold\">+</text>\n", dot_x - 12.0, dot_y + 14.0);
}

void callback_remocao_quadra(const char* chave, const char* dado, void* extra) {
    ContextoRemocaoQuadra* ctx = (ContextoRemocaoQuadra*)extra;
    
    char nome[50], sobrenome[50], nasc[20], cep[50], face[10];
    char sexo;
    int num;
    
    if (sscanf(dado, "%49[^;];%49[^;];%c;%19[^;];%*d;%49[^;];%9[^;];%d", 
               nome, sobrenome, &sexo, nasc, cep, face, &num) < 7) return;
    
    if (strcmp(cep, ctx->cep_alvo) == 0) {
        // Informa no TXT
        fprintf(ctx->arquivo_saida, "  Morador: %s %s (CPF: %s)\n", nome, sobrenome, chave);
        
        // Marca como sem-teto
        char novos_dados[TAMANHO_DADO];
        snprintf(novos_dados, sizeof(novos_dados), "%s;%s;%c;%s;0;SEM_TETO;X;0;none", 
                 nome, sobrenome, sexo, nasc);
        hash_remover(ctx->hash_habitantes, chave);
        hash_inserir(ctx->hash_habitantes, chave, novos_dados);
    }
}

typedef struct {
    char face;
    int count;
} ContagemPorFace;

typedef struct {
    const char* cep_alvo;
    ContagemPorFace faces[4]; 
    int total;
} ContextoContagemFaces;

void callback_contar_por_face(const char* chave, const char* dado, void* extra) {
    (void)chave;
    ContextoContagemFaces* ctx = (ContextoContagemFaces*)extra;
    
    char nome[50], sobrenome[50], nasc[20], cep[50], face[10];
    char sexo;
    int num;
    
    // Extrai os dados do habitante
    if (sscanf(dado, "%49[^;];%49[^;];%c;%19[^;];%*d;%49[^;];%9[^;];%d", 
               nome, sobrenome, &sexo, nasc, cep, face, &num) < 7) return;
    
    // Se o habitante mora na quadra alvo
    if (strcmp(cep, ctx->cep_alvo) == 0) {
        ctx->total++;
        char face_upper = toupper((unsigned char)face[0]);
        
        for (int i = 0; i < 4; i++) {
            if (ctx->faces[i].face == face_upper) {
                ctx->faces[i].count++;
                break;
            }
        }
    }
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
        hash_iterar(hq, desenhar_textos_quadras, svg); 
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
                    // Lista moradores que serão sem-tetos
                    ContextoRemocaoQuadra ctx = {cep, hh, saida};
                    hash_iterar(hh, callback_remocao_quadra, &ctx);
                    
                    // Remove a quadra
                    hash_remover(hq, cep);
                    fprintf(saida, "Quadra %s removida com sucesso.\n", cep);
                    
                    // Desenha X no SVG
                    if (svg) {
                        double x, y, w, h;
                        if (sscanf(dados_quadra, "%lf;%lf;%lf;%lf", &x, &y, &w, &h) == 4) {
                            double centro_x = x + (w / 2.0) - 12.0;
                            double centro_y = y + (h / 2.0) + 14.0;
                            fprintf(svg, "\t<text x=\"%.2lf\" y=\"%.2lf\" fill=\"red\" font-size=\"40\" font-weight=\"bold\">X</text>\n", centro_x, centro_y);
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
                fprintf(saida, "Comando: pq %s\n", cep);
                
                char dados_quadra[TAMANHO_DADO];
                if (hash_buscar(hq, cep, dados_quadra)) {
                    // Conta moradores por face
                    ContextoContagemFaces ctx;
                    ctx.cep_alvo = cep;
                    ctx.faces[0].face = 'S'; ctx.faces[0].count = 0;
                    ctx.faces[1].face = 'N'; ctx.faces[1].count = 0;
                    ctx.faces[2].face = 'L'; ctx.faces[2].count = 0;
                    ctx.faces[3].face = 'O'; ctx.faces[3].count = 0;
                    ctx.total = 0;
                    
                    hash_iterar(hh, callback_contar_por_face, &ctx);
                    
                    // Informa no arquivo de saída
                    fprintf(saida, "  Moradores por face:\n");
                    fprintf(saida, "    Norte: %d\n", ctx.faces[1].count);
                    fprintf(saida, "    Sul: %d\n", ctx.faces[0].count);
                    fprintf(saida, "    Leste: %d\n", ctx.faces[2].count);
                    fprintf(saida, "    Oeste: %d\n", ctx.faces[3].count);
                    fprintf(saida, "  Total de moradores: %d\n", ctx.total);
                    
                    // Desenha no SVG
                    if (svg) {
                        double x, y, w, h;
                        if (sscanf(dados_quadra, "%lf;%lf;%lf;%lf", &x, &y, &w, &h) == 4) {
                            // Norte
                            fprintf(svg, "\t<text x=\"%.2lf\" y=\"%.2lf\" font-size=\"14\" fill=\"black\" font-weight=\"bold\">%d</text>\n", 
                                    x + w/2 - 5, y + 18, ctx.faces[1].count);
                            // Sul
                            fprintf(svg, "\t<text x=\"%.2lf\" y=\"%.2lf\" font-size=\"14\" fill=\"black\" font-weight=\"bold\">%d</text>\n", 
                                    x + w/2 - 5, y + h - 8, ctx.faces[0].count);
                            // Leste
                            fprintf(svg, "\t<text x=\"%.2lf\" y=\"%.2lf\" font-size=\"14\" fill=\"black\" font-weight=\"bold\">%d</text>\n", 
                                    x + w - 18, y + h/2 + 5, ctx.faces[2].count);
                            // Oeste
                            fprintf(svg, "\t<text x=\"%.2lf\" y=\"%.2lf\" font-size=\"14\" fill=\"black\" font-weight=\"bold\">%d</text>\n", 
                                    x + 8, y + h/2 + 5, ctx.faces[3].count);
                            // Total no centro
                            fprintf(svg, "\t<text x=\"%.2lf\" y=\"%.2lf\" font-size=\"16\" fill=\"black\" font-weight=\"bold\">%d</text>\n", 
                                    x + w/2 - 8, y + h/2 + 8, ctx.total);
                        }
                    }
                } else {
                    fprintf(saida, "Aviso: Quadra %s nao encontrada.\n", cep);
                }
            }
        }
        else if (strcmp(comando, "censo") == 0) {
            fprintf(saida, "Comando: censo\n");
            CensoStats stats = {0, 0, 0, 0, 0};
            
            hash_iterar(hh, callback_censo, &stats);
            
            fprintf(saida, "Estatisticas do Censo de Bitnopolis:\n");
            fprintf(saida, "  Total de Habitantes: %d\n", stats.total_habitantes);
            fprintf(saida, "  Total de Moradores: %d\n", stats.moradores);
            fprintf(saida, "  Total de Sem-tetos: %d\n", stats.sem_tetos);
            
            if (stats.total_habitantes > 0) {
                double prop_moradores = (double)stats.moradores / stats.total_habitantes;
                fprintf(saida, "  Proporcao Moradores/Habitantes: %.2f\n", prop_moradores);
                
                double pct_homens = (double)stats.homens / stats.total_habitantes * 100.0;
                double pct_mulheres = (double)stats.mulheres / stats.total_habitantes * 100.0;
                fprintf(saida, "  Homens: %d (%.1f%%)\n", stats.homens, pct_homens);
                fprintf(saida, "  Mulheres: %d (%.1f%%)\n", stats.mulheres, pct_mulheres);
                
                if (stats.sem_tetos > 0) {
                    double pct_sem_tetos = (double)stats.sem_tetos / stats.total_habitantes * 100.0;
                    fprintf(saida, "  Sem-tetos: %.1f%%\n", pct_sem_tetos);
                }
            }
        }
        else if (strcmp(comando, "b?") == 0) {
            char cpf[TAMANHO_CHAVE];
            if (sscanf(linha, "%*s %49s", cpf) == 1) {
                fprintf(saida, "Comando: b? %s\n", cpf);
                char dados_habitante[TAMANHO_DADO];
                if (hash_buscar(hh, cpf, dados_habitante)) {
                    char nome[50], sobrenome[50], nasc[20], cep[50], face[10], compl[50];
                    char sexo;
                    int num;
                    
                    if (sscanf(dados_habitante, "%49[^;];%49[^;];%c;%19[^;];%*d;%49[^;];%9[^;];%d;%49[^;]", 
                               nome, sobrenome, &sexo, nasc, cep, face, &num, compl) >= 5) {
                        fprintf(saida, "  CPF: %s\n", cpf);
                        fprintf(saida, "  Nome: %s %s\n", nome, sobrenome);
                        fprintf(saida, "  Sexo: %c\n", sexo);
                        fprintf(saida, "  Data de Nascimento: %s\n", nasc);
                        
                        if (strcmp(cep, "SEM_TETO") != 0 && strcmp(cep, "none") != 0) {
                            fprintf(saida, "  Endereco: CEP %s, Face %s, Num %d, %s\n", cep, face, num, compl);
                        } else {
                            fprintf(saida, "  Status: Sem-teto\n");
                        }
                    } else {
                        fprintf(saida, "  Dados: %s\n", dados_habitante);
                    }
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
                    fprintf(saida, "  Pessoa nascida: %s %s\n", nome, sobrenome);
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
                    char nome[50], sobrenome[50], nasc[20], cep[50], face[10], compl[50];
                    char sexo;
                    int num;
                    
                    if (sscanf(dados_habitante, "%49[^;];%49[^;];%c;%19[^;];%*d;%49[^;];%9[^;];%d;%49[^;]", 
                               nome, sobrenome, &sexo, nasc, cep, face, &num, compl) >= 5) {
                        fprintf(saida, "  Falecido: %s %s\n", nome, sobrenome);
                        
                        // Se for morador, mostra o endereço
                        if (strcmp(cep, "SEM_TETO") != 0 && strcmp(cep, "none") != 0) {
                            fprintf(saida, "  Endereco onde ocorreu: CEP %s, Face %s, Num %d\n", cep, face, num);
                            
                            // Desenha cruz no SVG
                            if (svg) {
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
                        
                        fprintf(saida, "  Mudanca registrada: %s %s para CEP %s\n", nome, sobrenome, cep);
                        
                        if (svg) {
                            char dados_quadra[TAMANHO_DADO];
                            if (hash_buscar(hq, cep, dados_quadra)) {
                                double x, y, w, h;
                                if (sscanf(dados_quadra, "%lf;%lf;%lf;%lf", &x, &y, &w, &h) == 4) {
                                    // Calcula a posição do endereço de destino
                                    double dest_x, dest_y;
                                    calcular_coordenada_endereco(x, y, w, h, face[0], num, &dest_x, &dest_y);
                                    
                                    // Desenha quadrado vermelho no local de destino
                                    fprintf(svg, "\t<rect x=\"%.2lf\" y=\"%.2lf\" width=\"20\" height=\"20\" fill=\"red\" />\n", 
                                            dest_x - 10, dest_y - 10);
                                    // Escreve o CPF dentro do quadrado em minúsculas
                                    fprintf(svg, "\t<text x=\"%.2lf\" y=\"%.2lf\" font-size=\"8\" fill=\"white\" font-weight=\"bold\">%s</text>\n", 
                                            dest_x - 8, dest_y - 2, cpf);
                                }
                            }
                        }
                    }
                } else {
                    fprintf(saida, "  Erro: Habitante %s nao encontrado.\n", cpf);
                }
            }
        }
        else if (strcmp(comando, "dspj") == 0) {
            char cpf[TAMANHO_CHAVE];
            if (sscanf(linha, "%*s %49s", cpf) == 1) {
                fprintf(saida, "Comando: dspj %s\n", cpf);
                char dados_habitante[TAMANHO_DADO];
                if (hash_buscar(hh, cpf, dados_habitante)) {
                    char nome[50], sobrenome[50], nasc[20], cep[50], face[10], compl[50];
                    char sexo;
                    int num;
                    
                    if (sscanf(dados_habitante, "%49[^;];%49[^;];%c;%19[^;];%*d;%49[^;];%9[^;];%d;%49[^;]", 
                               nome, sobrenome, &sexo, nasc, cep, face, &num, compl) >= 5) {
                        if (strcmp(cep, "SEM_TETO") != 0 && strcmp(cep, "none") != 0) {
                            fprintf(saida, "  Morador despejado: %s %s\n", nome, sobrenome);
                            fprintf(saida, "  Endereco do despejo: CEP %s, Face %s, Num %d\n", cep, face, num);
                            
                            // Marca como sem-teto
                            char novos_dados[TAMANHO_DADO];
                            snprintf(novos_dados, sizeof(novos_dados), "%s;%s;%c;%s;0;SEM_TETO;X;0;none", 
                                     nome, sobrenome, sexo, nasc);
                            hash_remover(hh, cpf);
                            hash_inserir(hh, cpf, novos_dados);
                            
                            // Desenha círculo preto no SVG
                            if (svg) {
                                char dados_quadra[TAMANHO_DADO];
                                if (hash_buscar(hq, cep, dados_quadra)) {
                                    double x, y, w, h;
                                    if (sscanf(dados_quadra, "%lf;%lf;%lf;%lf", &x, &y, &w, &h) == 4) {
                                        svg_desenhar_despejo(svg, x, y, w, h, face[0], num);
                                    }
                                }
                            }
                        } else {
                            fprintf(saida, "  Habitante %s ja era sem-teto.\n", cpf);
                        }
                    }
                } else {
                    fprintf(saida, "  Erro: Habitante %s nao encontrado.\n", cpf);
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