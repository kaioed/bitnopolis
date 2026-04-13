/**
 * @file pm.h
 * @author Kaio E. L. dos Santos
 * @brief Funções para leitura e processamento de arquivos de formato .pm.
 * @version 1.0
 * @date 2026-04-13
 * @details Este módulo é responsável por fazer o parser de arquivos `.pm`,
 * que contêm comandos para a criação de habitantes/moradores.
 * Ele extrai as informações, armazena os habitantes em uma estrutura de dados
 * e gera um arquivo de saída.
 */

#ifndef PM_H
#define PM_H

#include <stdbool.h>

/**
 * @brief Processa um arquivo .pm, armazena os habitantes e gera um arquivo de saída.
 * @details A função lê linha por linha do arquivo de entrada.
 * Ao encontrar comandos específicos, cria novos habitantes, os armazena na
 * estrutura de dados fornecida e escreve as informações correspondentes
 * no arquivo de saída.
 * @param[in] caminho_arquivo O caminho completo para o arquivo `.pm` de entrada.
 * @param[in,out] hash_habitantes Ponteiro para a estrutura de dados (ex: Hash Map)
 * onde os habitantes lidos serão armazenados.
 * @return Retorna `true` se o arquivo de leitura foi
 * aberto com sucesso e processado. Retorna `false` em caso de erro
 * (ex: arquivo não encontrado ou permissão negada).
 * @warning A memória dos habitantes criados e armazenados no `hash_habitantes`
 * **não** é liberada por esta função. É responsabilidade do chamador
 * liberar a memória da estrutura de dados posteriormente.
 */
bool pm_processar_arquivo(const char* caminho_arquivo, void* hash_habitantes);

#endif // PM_H