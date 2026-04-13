#ifndef HASH_EXTENSIVEL_H
#define HASH_EXTENSIVEL_H

/**
* @file hash_extensivel.h
* @brief Biblioteca para manipulação de um Hash Extensível em disco.
*/

#include <stdbool.h>

#define TAMANHO_BALDE 4
#define TAMANHO_CHAVE 30   // Tamanho suficiente para um CEP ou ID em texto
#define TAMANHO_DADO 150   // Espaço suficiente para guardar os dados de uma Quadra

typedef void HashExtensivel;

/**
 * @brief Inicializa a estrutura do Hash Extensível vinculado a um arquivo.
 * @param profundidade_inicial A profundidade global inicial do diretório.
 * @param nome_arquivo Caminho do arquivo binário onde o hash será salvo.
 * @return Ponteiro opaco para a estrutura criada.
 */
HashExtensivel* hash_criar(int profundidade_inicial, const char* nome_arquivo);

/**
 * @brief Insere um novo registro no hash usando uma string como chave.
 * @param hash Ponteiro para o hash.
 * @param chave A chave de texto do registro (ex: CEP).
 * @param dado O payload contendo as informações (texto ou bytes).
 * @return true se inserido com sucesso, false se a chave já existe.
 */
bool hash_inserir(HashExtensivel* hash, const char* chave, const char* dado);

/**
 * @brief Busca um dado pela chave de texto.
 * @param hash Ponteiro para o hash.
 * @param chave A chave a ser buscada.
 * @param saida_dado Buffer onde o texto/dado encontrado será copiado.
 * @return true se encontrado, false caso contrário.
 */
bool hash_buscar(HashExtensivel* hash, const char* chave, char* saida_dado);

/**
 * @brief Remove um registro pela chave.
 * @param hash Ponteiro para o hash.
 * @param chave A chave de texto a ser removida.
 * @return true se removido com sucesso, false se não for encontrado.
 */
bool hash_remover(HashExtensivel* hash, const char* chave);

/**
 * @brief Libera toda a memória e fecha os arquivos do Hash.
 * @param hash Ponteiro para o hash.
 */
void hash_destruir(HashExtensivel* hash);

#endif