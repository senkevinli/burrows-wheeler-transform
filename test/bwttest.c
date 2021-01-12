#include "CUnit/Basic.h"
#include "../src/bwt.h"

void test_simple(void) {
    const char *orig = "banana";
    const char *transformed = createBwt(orig);
    CU_ASSERT_STRING_EQUAL("annb$aa", transformed);

    orig = "mississippi";
    transformed = createBwt(orig);
    CU_ASSERT_STRING_EQUAL("ipssm$pissii", transformed);

    orig = "aaaaaaa";
    transformed = createBwt(orig);
    CU_ASSERT_STRING_EQUAL("aaaaaaa$", transformed);

    orig =  
    "acaatgtcccgagtcaggaccggtgactagcttgcatctataatgattgccgttcacccg";
    transformed = createBwt(orig);
    CU_ASSERT_STRING_EQUAL(
        "gtc$cggtcgtcaagattgatccagccccatgcgtctttaacatgcacctggagattagca",
        transformed
    );
}

int init_suite(void) {
    return 0;
}

int clean_suite(void) {
    return 0;
}

int main() {

   CU_pSuite pSuite = NULL;
   if (CUE_SUCCESS != CU_initialize_registry())
      return CU_get_error();

   pSuite = CU_add_suite("Suite", init_suite, clean_suite);
   if (NULL == pSuite) {
      CU_cleanup_registry();
      return CU_get_error();
   }

   if ((NULL == CU_add_test(pSuite, "test proper transformation", test_simple)))
   {
      CU_cleanup_registry();
      return CU_get_error();
   }
    CU_basic_run_tests();
}