/**
 * @file habitantes.h
 * @author Kaio E. L. dos Santos
 * @brief Funções para gerenciamento de estruturas de dados de habitantes/moradores.
 * @version 1.0
 * @date 2026-04-27
 * @details Este módulo é responsável por encapsular informações sobre habitantes,
 * incluindo dados pessoais (CPF, nome, sobrenome, sexo, data de nascimento) e dados
 * de localização/endereço (CEP, face, número, complemento). Fornece uma interface
 * para criar, acessar, modificar e destruir instâncias de habitantes.
 */

#ifndef HABITANTES_H
#define HABITANTES_H

/**
 * @brief Tipo opaco para representar um habitante/morador.
 */
typedef void* Habitante;

/**
 * @brief Cria uma nova instância de habitante com os dados pessoais fornecidos.
 * @param[in] cpf Cadastro de Pessoa Física do habitante.
 * @param[in] nome Primeiro nome do habitante.
 * @param[in] sobrenome Sobrenome do habitante.
 * @param[in] sexo Sexo do habitante (caractere, ex: 'M' ou 'F').
 * @param[in] nasc Data de nascimento do habitante.
 * @return Ponteiro opaco para o habitante criado, ou NULL em caso de erro.
 */
Habitante habitante_criar(const char* cpf, const char* nome, const char* sobrenome, char sexo, const char* nasc);

/**
 * @brief Obtém o CPF do habitante.
 * @param[in] h Ponteiro para a estrutura de habitante.
 * @return String contendo o CPF.
 */
const char* habitante_get_cpf(Habitante h);

/**
 * @brief Obtém o primeiro nome do habitante.
 * @param[in] h Ponteiro para a estrutura de habitante.
 * @return String contendo o nome.
 */
const char* habitante_get_nome(Habitante h);

/**
 * @brief Obtém o sobrenome do habitante.
 * @param[in] h Ponteiro para a estrutura de habitante.
 * @return String contendo o sobrenome.
 */
const char* habitante_get_sobrenome(Habitante h);

/**
 * @brief Obtém o sexo do habitante.
 * @param[in] h Ponteiro para a estrutura de habitante.
 * @return Caractere representando o sexo.
 */
char habitante_get_sexo(Habitante h);

/**
 * @brief Obtém a data de nascimento do habitante.
 * @param[in] h Ponteiro para a estrutura de habitante.
 * @return String contendo a data de nascimento.
 */
const char* habitante_get_nasc(Habitante h);

/**
 * @brief Obtém o CEP do endereço do habitante.
 * @param[in] h Ponteiro para a estrutura de habitante.
 * @return String contendo o CEP.
 */
const char* habitante_get_cep(Habitante h);

/**
 * @brief Obtém a face/lado da quadra do endereço do habitante.
 * @param[in] h Ponteiro para a estrutura de habitante.
 * @return String contendo a face (pode ser NULL se não definido).
 */
char* habitante_get_face(Habitante h);

/**
 * @brief Obtém o número do endereço do habitante.
 * @param[in] h Ponteiro para a estrutura de habitante.
 * @return Inteiro representando o número do endereço.
 */
int habitante_get_num(Habitante h);

/**
 * @brief Obtém informações complementares do endereço do habitante.
 * @param[in] h Ponteiro para a estrutura de habitante.
 * @return String contendo as informações complementares.
 */
const char* habitante_get_compl(Habitante h);

/**
 * @brief Altera o endereço do habitante.
 * @param[in,out] h Ponteiro para a estrutura de habitante.
 * @param[in] cep Novo CEP.
 * @param[in] face Nova face/lado da quadra.
 * @param[in] num Novo número do endereço.
 * @param[in] compl Novas informações complementares do endereço.
 */
void alterar_endereco_habitante(Habitante h, const char* cep, const char* face, int num, const char* compl);

/**
 * @brief Libera a memória alocada para a estrutura de habitante.
 * @param[in] h Ponteiro para a estrutura de habitante a ser destruída.
 * @warning Após chamar esta função, o ponteiro h fica inválido e não deve ser utilizado.
 */
void habitante_destruir(Habitante h);

/**
 * @brief Formata os dados de um habitante em uma string de saída.
 * @details Combina todas as informações pessoais e de endereço em um formato específico
 * e escreve no buffer de saída fornecido.
 * @param[in] nome Primeiro nome do habitante.
 * @param[in] sobrenome Sobrenome do habitante.
 * @param[in] sexo Sexo do habitante.
 * @param[in] nasc Data de nascimento.
 * @param[in] cep CEP do endereço.
 * @param[in] face Face da quadra.
 * @param[in] num Número do endereço.
 * @param[in] compl Informações complementares do endereço.
 * @param[out] saida Buffer de saída onde os dados formatados serão escritos.
 * @warning O buffer 'saida' deve ter tamanho suficiente para acomodar os dados formatados.
 */
void formatar_dados_habitante(const char* nome, const char* sobrenome, char sexo, const char* nasc, const char* cep, const char* face, int num, const char* compl, char* saida);

#endif