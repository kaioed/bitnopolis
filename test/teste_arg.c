#include "Unity/unity.h"
#include "../include/arg.h"
#include <stdlib.h>

void setUp(void) {}
void tearDown(void) {}

void test_obter_valor_opcao_encontrado(void) {
    char* argv[] = {"./ted", "-e", "pasta_entrada", "-f", "mapa.geo"};
    int argc = 5;
    TEST_ASSERT_EQUAL_STRING("pasta_entrada", obter_valor_opcao(argc, argv, "e"));
    TEST_ASSERT_EQUAL_STRING("mapa.geo", obter_valor_opcao(argc, argv, "f"));
}

void test_obter_valor_opcao_nao_encontrado(void) {
    char* argv[] = {"./ted", "-e", "pasta_entrada"};
    int argc = 3;
    TEST_ASSERT_NULL(obter_valor_opcao(argc, argv, "f"));
}

void test_obter_sufixo_comando(void) {
    char* argv[] = {"./ted", "-e", "pasta_entrada", "sufixo_teste"};
    int argc = 4;
    TEST_ASSERT_EQUAL_STRING("sufixo_teste", obter_sufixo_comando(argc, argv));
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_obter_valor_opcao_encontrado);
    RUN_TEST(test_obter_valor_opcao_nao_encontrado);
    RUN_TEST(test_obter_sufixo_comando);
    return UNITY_END();
}