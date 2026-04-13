#include "../include/pm.h"
#include "../include/habitantes.h"
#include "../include/quadra.h"
#include "../include/hash_extensivel.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bool pm_processar_arquivo(const char* caminho_arquivo, void* ptr_hash_habitantes) {
    if (caminho_arquivo == NULL) return false;

    FILE* file = fopen(caminho_arquivo, "r");
    if (file == NULL) return false;

    HashExtensivel* hash_habitantes = (HashExtensivel*) ptr_hash_habitantes;
    char comando[10];

    while (fscanf(file, "%s", comando) != EOF) {
        
        if(strcmp(comando, "p") == 0) {
            char cpf[20], nome[30], sobrenome[30], data_nasc[15];
            char sexo;
            
            fscanf(file, "%s %s %s %c %s", cpf, nome, sobrenome, &sexo, data_nasc);
            
            char dados_habitante[150];
            snprintf(dados_habitante, sizeof(dados_habitante), "%s;%s;%c;%s;0;none;X;0;none", 
                     nome, sobrenome, sexo, data_nasc);
            
            hash_inserir(hash_habitantes, cpf, dados_habitante);   
        }
        
        else if(strcmp(comando, "m") == 0) {
            char cpf[20], cep[30], face[5], compl[30];
            int num;
            
            fscanf(file,"%s %s %s %d %s", cpf, cep, face, &num, compl);
            
            char buffer_busca[150];
            
            if (hash_buscar(hash_habitantes, cpf, buffer_busca)) {
                
                char nome[30], sobrenome[30], data_nasc[15], sexo;
                int is_morador, temp_num;
                char temp_cep[30], temp_face[5], temp_compl[30];
                
                sscanf(buffer_busca, "%[^;];%[^;];%c;%[^;];%d;%[^;];%[^;];%d;%s", 
                       nome, sobrenome, &sexo, data_nasc, &is_morador, 
                       temp_cep, temp_face, &temp_num, temp_compl);
                
                
                char novos_dados[150];
                snprintf(novos_dados, sizeof(novos_dados), "%s;%s;%c;%s;1;%s;%s;%d;%s", 
                         nome, sobrenome, sexo, data_nasc, cep, face, num, compl);
                
                hash_remover(hash_habitantes, cpf);
                hash_inserir(hash_habitantes, cpf, novos_dados);
            }
        }
    }
    
    fclose(file);
    return true;
}