#include "Unity/unity.h"
#include "../include/pm.h"
#include "../include/hash_extensivel.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

void test_deve_processar_pessoas_e_moradores_corretamente(void) {
    HashExtensivel* hash_pessoas = hash_criar(2, "hash_pm_temp.hf");

    FILE* f = fopen("temp_pessoas.pm", "w");
    fprintf(f, "p 123456789 Joao Silva M 01/01/1990\n");
    fprintf(f, "m 123456789 86010-000 S 10 Apto_2\n");
    fclose(f);
    
    bool resultado = pm_processar_arquivo("temp_pessoas.pm", hash_pessoas);
    TEST_ASSERT_TRUE(resultado);
    
    char dados[150];
    bool achou = hash_buscar(hash_pessoas, "123456789", dados);
    
    TEST_ASSERT_TRUE(achou);
    TEST_ASSERT_NOT_NULL(strstr(dados, "Joao"));
    TEST_ASSERT_NOT_NULL(strstr(dados, "86010-000")); 
    
    hash_destruir(hash_pessoas);
    remove("temp_pessoas.pm");
    remove("hash_pm_temp.hf");
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_deve_processar_pessoas_e_moradores_corretamente);
    return UNITY_END();
}