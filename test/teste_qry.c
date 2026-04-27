#include "Unity/unity.h"
#include "../include/qry.h"
#include "../include/hash_extensivel.h"
#include <stdio.h>
#include <stdlib.h>

void setUp(void) {}
void tearDown(void) {}

void processar_qry(const char* caminho_qry, const char* caminho_saida, void* hash_quadras, void* hash_habitantes);

void test_processar_qry_censo_vazio(void) {
    const char* qry_file = "dummy.qry";
    const char* out_file = "dummy_out.txt";
    
    FILE* f = fopen(qry_file, "w");
    fprintf(f, "censo\n");
    fclose(f);

    HashExtensivel* hq = hash_criar(2, "dummy_hq.hf");
    HashExtensivel* hh = hash_criar(2, "dummy_hh.hf");

    processar_qry(qry_file, out_file, hq, hh);

    FILE* out = fopen(out_file, "r");
    TEST_ASSERT_NOT_NULL(out);
    if (out) fclose(out);

    hash_destruir(hq);
    hash_destruir(hh);
    remove(qry_file);
    remove(out_file);
    remove("dummy_out.svg");
    remove("dummy_hq.hf");
    remove("dummy_hh.hf");
    remove("dummy_hq.hf_exp.log");
    remove("dummy_hh.hf_exp.log");
    remove("dummy_hq.hf.hfd");
    remove("dummy_hh.hf.hfd");
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_processar_qry_censo_vazio);
    return UNITY_END();
}