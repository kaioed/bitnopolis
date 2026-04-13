#include "Unity/unity.h"
#include "../include/geo.h"
#include "../include/hash_extensivel.h"
#include <stdio.h>
#include <stdlib.h>

void setUp(void) {}
void tearDown(void) {}

void test_deve_falhar_ao_abrir_arquivo_geo_inexistente(void) {
    bool resultado = geo_processar_arquivo("arquivo_fantasma.geo", "saida.svg", NULL);
    TEST_ASSERT_FALSE(resultado);
}

void test_deve_processar_comandos_q_e_cq_corretamente(void) {
    HashExtensivel* hash_temp = hash_criar(2, "hash_geo_temp.hf");

    FILE* f = fopen("temp_entrada.geo", "w");
    fprintf(f, "cq 3.0 yellow blue\n");
    fprintf(f, "q 86010-000 10.0 10.0 50.0 50.0\n");
    fclose(f);
    
    bool resultado = geo_processar_arquivo("temp_entrada.geo", "temp_saida.svg", hash_temp);
    TEST_ASSERT_TRUE(resultado);
    
    char dados_salvos[150];
    bool achou_no_hash = hash_buscar(hash_temp, "86010-000", dados_salvos);
    TEST_ASSERT_TRUE(achou_no_hash);
    
    FILE* svg_verificacao = fopen("temp_saida.svg", "r");
    TEST_ASSERT_NOT_NULL(svg_verificacao);
    if(svg_verificacao) fclose(svg_verificacao);
    
    hash_destruir(hash_temp);
    remove("temp_entrada.geo");
    remove("temp_saida.svg");
    remove("hash_geo_temp.hf");
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_deve_falhar_ao_abrir_arquivo_geo_inexistente);
    RUN_TEST(test_deve_processar_comandos_q_e_cq_corretamente);
    return UNITY_END();
}