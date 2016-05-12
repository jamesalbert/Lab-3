#include <stdio.h>
#include <string.h>
#include <CUnit/Basic.h>
#include <CUnit/Console.h>
#include <CUnit/Automated.h>
#include "Lab3_12912150_16004325_29856616.c"

static int* heap;

int init_suite(void) {
  heap = malloc(HEAPSIZE);
  *heap = HEAPSIZE;
	return 0;
}

int clean_suite(void) {
  free(heap);
	return 0;
}

void test_parsecommand(void) {
	char command_basic[MAXCMD]      = "./test this that";
	char command_tabbed_mid[MAXCMD] = "./test 		this 		that";
	char command_tabbed_all[MAXCMD] = "		./test 		this 		that		";
	char* argv[MAXARGS];
	CU_ASSERT(parsecommand(command_basic, argv) == 2); // right argc
	CU_ASSERT(strcmp(argv[0], "./test") == 0);
	CU_ASSERT(strcmp(argv[1], "this") == 0);
	CU_ASSERT(strcmp(argv[2], "that") == 0);
	CU_ASSERT(parsecommand(command_tabbed_mid, argv) == 2);
	CU_ASSERT(strcmp(argv[0], "./test") == 0);
	CU_ASSERT(strcmp(argv[1], "this") == 0);
	CU_ASSERT(strcmp(argv[2], "that") == 0);
	CU_ASSERT(parsecommand(command_tabbed_all, argv) == 2);
	CU_ASSERT(strcmp(argv[0], "./test") == 0);
	CU_ASSERT(strcmp(argv[1], "this") == 0);
	CU_ASSERT(strcmp(argv[2], "that") == 0);
}

void test_allocate(void) {
  CU_ASSERT(Allocate(heap, 368) == 0); // <= 368 passes, o/w fails
  CU_ASSERT(Allocate(heap, 369) == 0); // >= 369 fails
  CU_ASSERT(Allocate(heap, 1) == 0);
}

int main() {
	CU_pSuite pSuite = NULL;
	if (CUE_SUCCESS != CU_initialize_registry())
		return CU_get_error();

	pSuite = CU_add_suite("test suite", init_suite, clean_suite);
	if (NULL == pSuite) {
		CU_cleanup_registry();
		return CU_get_error();
	}

	if (CU_add_test(pSuite, "test parsecommand", test_parsecommand) == NULL ||
			CU_add_test(pSuite, "test allocate", test_allocate) == NULL) {
		CU_cleanup_registry();
    return CU_get_error();
	}

	CU_basic_set_mode(CU_BRM_VERBOSE);
  //CU_automated_run_tests();
  CU_basic_run_tests();
  CU_cleanup_registry();
  return CU_get_error();
}
