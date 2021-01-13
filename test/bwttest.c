#include <stdlib.h>
#include "CUnit/Basic.h"
#include "../src/bwt.h"

void test_creation(void) {
    const char *orig = "banana";
    const char *transformed = createBwt(orig);
    CU_ASSERT_STRING_EQUAL("annb$aa", transformed);

    orig = "mississippi";
    transformed = createBwt(orig);
    CU_ASSERT_STRING_EQUAL("ipssm$pissii", transformed);

    free((void *)transformed);

    orig = "aaaaaaa";
    transformed = createBwt(orig);
    CU_ASSERT_STRING_EQUAL("aaaaaaa$", transformed);

    free((void *)transformed);

    orig =  
    "acaatgtcccgagtcaggaccggtgactagcttgcatctataatgattgccgttcacccg";
    transformed = createBwt(orig);
    CU_ASSERT_STRING_EQUAL(
        "gtc$cggtcgtcaagattgatccagccccatgcgtctttaacatgcacctggagattagca",
        transformed
    );

    free((void *)transformed);
}

/* For helping inversion tests. */
void inversion_helper(const char *strings[], int len) {
    int i;
    for (i = 0; i < len; i++) {
        const char *orig = strings[i];

        const char *transformed = createBwt(orig);
        const char *inverted = inverseBwt(transformed);
        CU_ASSERT_STRING_EQUAL(orig, inverted);

        free((void *)transformed);
        free((void *)inverted);
    }
}

void test_inversion(void) {
    const char *strings[] = {
        "banana",
        "mississippi",
        "aaaaaaa",
        "ilikepie",
        "acaatgtcccgagtcaggaccggtgactagcttgcatctataatgattgccgttcacccg"
    };
    inversion_helper(strings, 5);
    
}

int init_suite(void) {
    return 0;
}

int clean_suite(void) {
    return 0;
}

/* Main runner function for unit tests. */
int main() {

   CU_pSuite pSuite = NULL;
   if (CUE_SUCCESS != CU_initialize_registry())
      return CU_get_error();

   pSuite = CU_add_suite("Suite", init_suite, clean_suite);
   if (NULL == pSuite) {
      CU_cleanup_registry();
      return CU_get_error();
   }

   if ((NULL == CU_add_test(pSuite, "test proper transformation", test_creation) ||
        NULL == CU_add_test(pSuite, "test inversion", test_inversion)))
   {
      CU_cleanup_registry();
      return CU_get_error();
   }
    CU_basic_run_tests();
}