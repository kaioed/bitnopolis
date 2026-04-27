/**
 * @file casa.h
 * @author Kaio E. L. dos Santos
 * @brief Funções para gerenciamento de estruturas de dados de casas/residências.
 * @version 1.0
 * @date 2026-04-27
 * @details Este módulo é responsável por encapsular informações sobre casas,
 * incluindo dados pessoais do proprietário (CPF), localização (CEP, face, número, complemento)
 * e fornecer uma interface para criar, acessar e destruir instâncias de casas.
 */

#ifndef CASA_H
#define CASA_H

/**
 * @brief Tipo opaco para representar uma casa/residência.
 */
typedef void* Casa;

/**
 * @brief Cria uma nova instância de casa com as informações fornecidas.
 * @param[in] cpf CPF do proprietário da casa.
 * @param[in] cep Código de Endereçamento Postal (CEP) da localização.
 * @param[in] face Face/lado da quadra onde a casa está localizada.
 * @param[in] num Número da casa.
 * @param[in] compl Informações complementares do endereço.
 * @return Ponteiro opaco para a casa criada, ou NULL em caso de erro.
 */
Casa casa_criar(const char* cpf, const char* cep, char face, int num, const char* compl);

/**
 * @brief Obtém o CPF do proprietário da casa.
 * @param[in] c Ponteiro para a estrutura de casa.
 * @return String contendo o CPF do proprietário.
 */
const char* casa_get_cpf(Casa c);

/**
 * @brief Obtém o CEP de localização da casa.
 * @param[in] c Ponteiro para a estrutura de casa.
 * @return String contendo o CEP.
 */
const char* casa_get_cep(Casa c);

/**
 * @brief Obtém a face/lado da quadra onde a casa está localizada.
 * @param[in] c Ponteiro para a estrutura de casa.
 * @return Caractere representando a face da quadra.
 */
char casa_get_face(Casa c);

/**
 * @brief Obtém o número da casa.
 * @param[in] c Ponteiro para a estrutura de casa.
 * @return Número inteiro da casa.
 */
int casa_get_num(Casa c);

/**
 * @brief Obtém informações complementares do endereço da casa.
 * @param[in] c Ponteiro para a estrutura de casa.
 * @return String contendo as informações complementares.
 */
const char* casa_get_compl(Casa c);

/**
 * @brief Libera a memória alocada para a estrutura de casa.
 * @param[in] c Ponteiro para a estrutura de casa a ser destruída.
 * @warning Após chamar esta função, o ponteiro c fica inválido e não deve ser utilizado.
 */
void casa_destruir(Casa c);

#endif