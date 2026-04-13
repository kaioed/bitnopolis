#include "Unity/unity.h"
#include "../include/hash_extensivel.h"
#include <stdbool.h>
#include <stdio.h>

#define ARQUIVO_TESTE_HASH "teste_temp.hf"

void setUp(void) {
    remove(ARQUIVO_TESTE_HASH); 
}

void tearDown(void) {
    remove(ARQUIVO_TESTE_HASH); 
}

void test_deve_criar_hash_com_sucesso(void) {
    HashExtensivel* hash = hash_criar(2, ARQUIVO_TESTE_HASH);
    TEST_ASSERT_NOT_NULL(hash);
    hash_destruir(hash);
}

void test_deve_inserir_e_buscar_registro_com_sucesso(void) {
    HashExtensivel* hash = hash_criar(2, ARQUIVO_TESTE_HASH);
    char resultado[150];
    
    bool inseriu = hash_inserir(hash, "86010-000", "Dados da Quadra A");
    TEST_ASSERT_TRUE(inseriu);
    
    bool encontrou = hash_buscar(hash, "86010-000", resultado);
    TEST_ASSERT_TRUE(encontrou);
    TEST_ASSERT_EQUAL_STRING("Dados da Quadra A", resultado);
    
    hash_destruir(hash);
}

void test_deve_falhar_ao_inserir_chave_duplicada(void) {
    HashExtensivel* hash = hash_criar(2, ARQUIVO_TESTE_HASH);
    
    hash_inserir(hash, "CEP-DUPLICADO", "Primeira Insercao");
    bool inseriu_novamente = hash_inserir(hash, "CEP-DUPLICADO", "Segunda Insercao");
    
    TEST_ASSERT_FALSE(inseriu_novamente);
    hash_destruir(hash);
}

void test_deve_remover_registro_existente_com_sucesso(void) {
    HashExtensivel* hash = hash_criar(2, ARQUIVO_TESTE_HASH);
    char resultado[150];
    
    hash_inserir(hash, "CEP-REMOVER", "Dado Temporario");
    
    bool removeu = hash_remover(hash, "CEP-REMOVER");
    TEST_ASSERT_TRUE(removeu);
    
    bool encontrou = hash_buscar(hash, "CEP-REMOVER", resultado);
    TEST_ASSERT_FALSE(encontrou);
    
    hash_destruir(hash);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_deve_criar_hash_com_sucesso);
    RUN_TEST(test_deve_inserir_e_buscar_registro_com_sucesso);
    RUN_TEST(test_deve_falhar_ao_inserir_chave_duplicada);
    RUN_TEST(test_deve_remover_registro_existente_com_sucesso);
    return UNITY_END();
}