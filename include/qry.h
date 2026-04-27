/**
 * @file qry.h
 * @author Kaio E. L. dos Santos
 * @brief Funções para processamento de consultas/queries.
 * @version 1.0
 * @date 2026-04-27
 * @details Este módulo é responsável pelo processamento de consultas e queries
 * sobre as estruturas de dados. Permite realizar buscas, filtros e operações
 * de análise sobre habitantes, casas e quadras.
 */

#ifndef QRY_H
#define QRY_H

/**
 * @brief Processa um arquivo de consultas (.qry) e gera um arquivo de saída.
 * @details Lê linha por linha do arquivo de entrada, executa as consultas
 * especificadas sobre as estruturas de dados fornecidas e escreve os resultados
 * no arquivo de saída.
 * @param[in] caminho_qry Caminho completo para o arquivo .qry de entrada.
 * @param[in] caminho_saida Caminho completo onde o arquivo de saída será salvo.
 * @param[in,out] hash_quadras Ponteiro para a estrutura de dados das quadras.
 * @param[in,out] hash_habitantes Ponteiro para a estrutura de dados dos habitantes.
 * @return Retorna true se o processamento foi bem-sucedido, false em caso de erro.
 * @warning A memória das estruturas de dados não é liberada por esta função.
 */
void processar_qry(const char* caminho_qry, const char* caminho_saida, void* hash_quadras, void* hash_habitantes);

/**
 * @brief Função callback para processamento de dados durante o censo.
 * @details Esta função é utilizada como callback durante iterações sobre
 * estruturas de dados, permitindo processar cada par chave-valor encontrado.
 * @param[in] chave Chave do elemento atual sendo processado.
 * @param[in] dado Valor associado à chave atual.
 * @param[in,out] extra Ponteiro para dados extras fornecidos pelo chamador.
 */
void callback_censo(const char* chave, const char* dado, void* extra);

#endif 
