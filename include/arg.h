/**
 * @file arg.h
 * @author Kaio E. L. dos Santos
 * @brief Biblioteca de utilitários para tratamento de argumentos de linha de comando.
 * @version 1.0
 * @date 2026-04-27
 * @details Este módulo é responsável por fazer o parser de argumentos de linha de comando.
 * Auxilia na leitura de opções no formato "-opt valor" e na extração do sufixo de comando
 * final passado para o programa.
 */

#ifndef ARGS_HANDLER_H
#define ARGS_HANDLER_H

#include <stdio.h>

/**
 * @brief Obtém o valor de uma opção no formato -opt valor.
 * @details Procura pela opção especificada no vetor de argumentos e retorna o valor
 * associado à opção, que é o argumento subsequente.
 *
 * @param[in] argc número de argumentos recebidos pela aplicação (main).
 * @param[in] argv vetor de strings com os argumentos.
 * @param[in] opt_name nome da opção (sem o '-'). Exemplo: "opt".
 * @return Ponteiro para a string do valor encontrado ou NULL se não encontrado.
 */
char *obter_valor_opcao(int argc, char *argv[], char *opt_name);

/**
 * @brief Obtém o sufixo do comando (última string que não começa com '-').
 * @details Remove todos os pares -opt valor antes de buscar o sufixo. Se houver mais de um
 * candidato a sufixo, a função imprime erro e termina o programa.
 *
 * @param[in] argc número de argumentos.
 * @param[in] argv vetor de argumentos.
 * @return Ponteiro para a string do sufixo (ponteiro para argv original) ou NULL se não existir.
 * @warning Se houver múltiplos sufixos, o programa será terminado com erro.
 */
char *obter_sufixo_comando(int argc, char *argv[]);

#endif