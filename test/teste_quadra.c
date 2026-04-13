#include "Unity/unity.h"
#include "../include/quadra.h"

void setUp(void) {}
void tearDown(void) {}

void test_deve_criar_e_destruir_quadra_com_sucesso(void) {
    Quadra q = quadra_criar("cep15", 10.0, 20.0, 100.0, 50.0, "red", "black", 2.5);
    
    TEST_ASSERT_NOT_NULL(q);
    
    TEST_ASSERT_EQUAL_STRING("cep15", quadra_get_cep(q));
    TEST_ASSERT_EQUAL_FLOAT(10.0, quadra_get_x(q));
    TEST_ASSERT_EQUAL_STRING("red", quadra_get_cfill(q));
    TEST_ASSERT_EQUAL_STRING("black", quadra_get_cstrk(q));
    TEST_ASSERT_EQUAL_FLOAT(2.5, quadra_get_sw(q));

    
    quadra_destruir(q);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_deve_criar_e_destruir_quadra_com_sucesso);
    return UNITY_END();
}