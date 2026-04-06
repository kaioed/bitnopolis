#include "unity.h"
#include "geo.h"
#include <stdio.h>
#include <stdlib.h>

void setUp(void) {}
void tearDown(void) {}

void test_deve_criar_e_destruir_quadra_com_sucesso(void) {
    Quadra q = geo_criar_quadra("cep15", 10.0, 20.0, 100.0, 50.0, "red", "black", 2.5);
    
    TEST_ASSERT_NOT_NULL(q); // Verifica se o malloc não falhou
    
    // Verifica se os getters estão retornando os dados corretos
    TEST_ASSERT_EQUAL_STRING("cep15", geo_get_cep(q));
    TEST_ASSERT_EQUAL_FLOAT(10.0, geo_get_x(q));
    TEST_ASSERT_EQUAL_STRING("red", geo_get_cfill(q));
    TEST_ASSERT_EQUAL_FLOAT(2.5, geo_get_sw(q));
    
    geo_destruir_quadra(q);
}

void test_deve_falhar_ao_abrir_arquivo_geo_inexistente(void) {
    // Passando um arquivo que não existe para garantir que a função lida com o erro sem quebrar
    bool resultado = geo_processar_arquivo("arquivo_fantasma.geo", NULL);
    TEST_ASSERT_EQUAL_INT(false, resultado);
}

void test_deve_processar_comandos_q_e_cq_corretamente(void) {
    // Preparo: Cria um arquivo temporário com o formato esperado pelo projeto
    FILE* f = fopen("teste_temporario.geo", "w");
    fprintf(f, "cq 3.0 yellow blue\n");
    fprintf(f, "q cep01 10.0 10.0 50.0 50.0\n");
    fclose(f);
    
    // Ação
    bool resultado = geo_processar_arquivo("teste_temporario.geo", NULL);
    
    // Verificação
    TEST_ASSERT_EQUAL_INT(true, resultado);
    
    // Limpeza: remove o arquivo criado pelo teste
    remove("teste_temporario.geo");
}

int main(void) {
    UNITY_BEGIN();
    
    RUN_TEST(test_deve_criar_e_destruir_quadra_com_sucesso);
    RUN_TEST(test_deve_falhar_ao_abrir_arquivo_geo_inexistente);
    RUN_TEST(test_deve_processar_comandos_q_e_cq_corretamente);
    
    return UNITY_END();
}