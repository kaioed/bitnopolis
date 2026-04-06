#ifndef GEO_H
#define GEO_H
/**
 * @file geo.h
 * @brief Biblioteca de operações geométricas para quadras.
 *
 * Esta biblioteca fornece uma interface opaca para criar, consultar e destruir
 * objetos Quadra, além de processar arquivos .geo com comandos de criação.
 */

#include <stdbool.h>

typedef void* Quadra;

/**
 * @brief Cria e aloca uma nova Quadra na memória.
 */
Quadra geo_criar_quadra(const char* cep, double x, double y, double w, double h, const char* cfill, const char* cstrk, double sw);

/**
 * @brief Retorna o CEP da quadra.
 * @param q Ponteiro opaco para a quadra.
 * @return String com o CEP ou NULL em caso de ponteiro inválido.
 */
const char* geo_get_cep(Quadra q);

/**
 * @brief Retorna a coordenada X da quadra.
 * @param q Ponteiro opaco para a quadra.
 * @return Valor X ou 0.0 em caso de ponteiro inválido.
 */
double geo_get_x(Quadra q);

/**
 * @brief Retorna a coordenada Y da quadra.
 * @param q Ponteiro opaco para a quadra.
 * @return Valor Y ou 0.0 em caso de ponteiro inválido.
 */
double geo_get_y(Quadra q);

/**
 * @brief Retorna a largura da quadra.
 * @param q Ponteiro opaco para a quadra.
 * @return Largura ou 0.0 em caso de ponteiro inválido.
 */
double geo_get_w(Quadra q);

/**
 * @brief Retorna a altura da quadra.
 * @param q Ponteiro opaco para a quadra.
 * @return Altura ou 0.0 em caso de ponteiro inválido.
 */
double geo_get_h(Quadra q);

/**
 * @brief Retorna a cor de preenchimento da quadra.
 * @param q Ponteiro opaco para a quadra.
 * @return String com a cor de preenchimento ou NULL em caso de ponteiro inválido.
 */
const char* geo_get_cfill(Quadra q);

/**
 * @brief Retorna a cor da borda da quadra.
 * @param q Ponteiro opaco para a quadra.
 * @return String com a cor da borda ou NULL em caso de ponteiro inválido.
 */
const char* geo_get_cstrk(Quadra q);

/**
 * @brief Retorna a espessura da borda da quadra.
 * @param q Ponteiro opaco para a quadra.
 * @return Espessura da borda ou 0.0 em caso de ponteiro inválido.
 */
double geo_get_sw(Quadra q);

/**
 * @brief Libera a memória alocada para a quadra.
 */
void geo_destruir_quadra(Quadra q);

/**
 * @brief Abre e processa o arquivo .geo, interpretando comandos 'cq' e 'q'.
 * @param caminho_arquivo Caminho para o arquivo .geo
 * @param hash_quadras Ponteiro genérico para o seu HashFile Dinâmico (onde as quadras serão salvas).
 * @return true se o arquivo foi processado com sucesso, false caso dê erro (ex: arquivo não encontrado).
 */
bool geo_processar_arquivo(const char* caminho_arquivo, void* hash_quadras);

#endif