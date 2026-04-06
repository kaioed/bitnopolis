#ifndef HASH_EXTENSIVEL_H
#define HASH_EXTENSIVEL_H
/**
* @file hash_extensivel.h
* @brief Biblioteca para manipulação de um Hash Extensível simples.
 *
 * Esta biblioteca fornece uma interface opaca para criar, inserir, buscar,
 * remover e destruir registros em uma tabela de hash extensível.
 */

#include <stdbool.h>

// Definição de tamanho do balde
#define TAMANHO_BALDE 4

// O usuário manipula o ponteiro, mas só o .c sabe como a memória é organizada.
typedef void HashExtensivel;

/**
 * @brief Inicializa a estrutura do Hash Extensível.
 * @param profundidade_inicial A profundidade global inicial do diretório.
 * @return Ponteiro opaco para a estrutura criada.
 */
HashExtensivel* hash_criar(int profundidade_inicial);

/**
 * @brief Insere um novo par de chave e dado no hash.
 * @param hash Ponteiro para o hash.
 * @param chave A chave numérica do registro.
 * @param dado O texto ou carga útil (string).
 * @return true se inserido com sucesso, false se a chave já existe.
 */
bool hash_inserir(HashExtensivel* hash, int chave, const char* dado);

/**
 * @brief Busca um dado pela chave.
 * @param hash Ponteiro para o hash.
 * @param chave A chave a ser buscada.
 * @param saida_dado Buffer (vetor de char) onde o texto encontrado será copiado.
 * @return true se encontrado, false caso contrário.
 */
bool hash_buscar(HashExtensivel* hash, int chave, char* saida_dado);

/**
 * @brief Remove um registro pela chave.
 * @param hash Ponteiro para o hash.
 * @param chave A chave a ser removida.
 * @return true se removido com sucesso, false se não for encontrado.
 */
bool hash_remover(HashExtensivel* hash, int chave);

/**
 * @brief Libera toda a memória alocada pelo Hash.
 * @param hash Ponteiro para o hash.
 */
void hash_destruir(HashExtensivel* hash);

#endif 