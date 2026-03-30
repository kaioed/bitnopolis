#include "Unity/unity.h"
#include "../include/hash_extensivel.h"
#include <stdbool.h>

void setUp(void) {}
void tearDown(void) {}

void test_deve_criar_hash_com_sucesso(void) {
    HashExtensivel* hash = hash_criar(2);
    TEST_ASSERT_NOT_NULL(hash);
    hash_destruir(hash);
}

void test_deve_inserir_e_buscar_registro_com_sucesso(void) {
    HashExtensivel* hash = hash_criar(2);
    char resultado[50];
    
    bool inseriu = hash_inserir(hash, 10, "Dados do Cliente A");
    TEST_ASSERT_EQUAL_INT(true, inseriu);
    
    bool encontrou = hash_buscar(hash, 10, resultado);
    TEST_ASSERT_EQUAL_INT(true, encontrou);
    
    TEST_ASSERT_EQUAL_STRING("Dados do Cliente A", resultado);
    
    hash_destruir(hash);
}

void test_deve_falhar_ao_inserir_chave_duplicada(void) {
    HashExtensivel* hash = hash_criar(2);
    
    hash_inserir(hash, 42, "Primeira Insercao");
    
    bool inseriu_novamente = hash_inserir(hash, 42, "Segunda Insercao");
    TEST_ASSERT_EQUAL_INT(false, inseriu_novamente);
    
    hash_destruir(hash);
}

void test_deve_falhar_ao_buscar_chave_inexistente(void) {
    HashExtensivel* hash = hash_criar(2);
    char resultado[50];
    
    bool encontrou = hash_buscar(hash, 99, resultado);
    TEST_ASSERT_EQUAL_INT(false, encontrou);
    
    hash_destruir(hash);
}

void test_deve_remover_registro_existente_com_sucesso(void) {
    HashExtensivel* hash = hash_criar(2);
    char resultado[50];
    
    hash_inserir(hash, 77, "Dado Temporario");
    
    bool removeu = hash_remover(hash, 77);
    TEST_ASSERT_EQUAL_INT(true, removeu);
    
    bool encontrou = hash_buscar(hash, 77, resultado);
    TEST_ASSERT_EQUAL_INT(false, encontrou);
    
    hash_destruir(hash);
}

void test_deve_falhar_ao_remover_chave_inexistente(void) {
    HashExtensivel* hash = hash_criar(2);
    
    bool removeu = hash_remover(hash, 100);
    TEST_ASSERT_EQUAL_INT(false, removeu);
    
    hash_destruir(hash);
}

void test_caminho_infeliz_operacoes_com_ponteiro_nulo(void) {
    char resultado[50];
    
    bool inseriu = hash_inserir(NULL, 1, "Dado");
    TEST_ASSERT_EQUAL_INT(false, inseriu);
    
    bool encontrou = hash_buscar(NULL, 1, resultado);
    TEST_ASSERT_EQUAL_INT(false, encontrou);
    
    bool removeu = hash_remover(NULL, 1);
    TEST_ASSERT_EQUAL_INT(false, removeu);
}

int main(void) {
    UNITY_BEGIN();
    
    RUN_TEST(test_deve_criar_hash_com_sucesso);
    RUN_TEST(test_deve_inserir_e_buscar_registro_com_sucesso);
    RUN_TEST(test_deve_falhar_ao_inserir_chave_duplicada);
    RUN_TEST(test_deve_falhar_ao_buscar_chave_inexistente);
    RUN_TEST(test_deve_remover_registro_existente_com_sucesso);
    RUN_TEST(test_deve_falhar_ao_remover_chave_inexistente);
    RUN_TEST(test_caminho_infeliz_operacoes_com_ponteiro_nulo);
    
    return UNITY_END();
}