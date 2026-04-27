#include "Unity/unity.h"
#include "../include/casa.h"

void setUp(void) {}
void tearDown(void) {}

void test_casa_criar_e_getters(void) {
    Casa c = casa_criar("12345678900", "86000-000", 'N', 100, "Apto 1");
    TEST_ASSERT_NOT_NULL(c);
    TEST_ASSERT_EQUAL_STRING("12345678900", casa_get_cpf(c));
    TEST_ASSERT_EQUAL_STRING("86000-000", casa_get_cep(c));
    TEST_ASSERT_EQUAL('N', casa_get_face(c));
    TEST_ASSERT_EQUAL(100, casa_get_num(c));
    TEST_ASSERT_EQUAL_STRING("Apto 1", casa_get_compl(c));
    casa_destruir(c);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_casa_criar_e_getters);
    return UNITY_END();
}