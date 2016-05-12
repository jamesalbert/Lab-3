#include <stdio.h>
#include <string.h>
#include <CUnit/Basic.h>
#include <CUnit/Console.h>
#include <CUnit/Automated.h>
#include "Lab3_12912150_16004325_29856616.c"

int* heap;

int init_suite(void) {
  heap = malloc(HEAPSIZE);
  *heap = HEAPSIZE;
  bID = 0;
	return 0;
}

int clean_suite(void) {
  free(heap);
  bID = 0;
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

void test_allocate_maxing(void) {
  CU_ASSERT(Allocate(heap, 100) == 0);
  CU_ASSERT(Allocate(heap, 300) == -1);
  CU_ASSERT(Allocate(heap, 400) == -1);
  CU_ASSERT(Allocate(heap, 1) == 0);
  Free(heap, 1);
  Free(heap, 2);
}

void test_allocate_normal(void) {
  CU_ASSERT(Allocate(heap, 0) == -1);
  CU_ASSERT(Allocate(heap, 5) == 0);
  CU_ASSERT(Allocate(heap, 10) == 0);
  CU_ASSERT(Allocate(heap, 50) == 0);
  CU_ASSERT(Allocate(heap, 100) == 0);
}

void test_free(void) {
  Allocate(heap, 10);
  Allocate(heap, 40);
  Allocate(heap, 20);
  Allocate(heap, 50);
  CU_ASSERT(Free(heap, 1) == 0);
  CU_ASSERT(Free(heap, 2) == 0);
  CU_ASSERT(Free(heap, 3) == 0);
  CU_ASSERT(Free(heap, 4) == 0);
}

void test_write_heap(void) {
  Allocate(heap, 10);
  Allocate(heap, 4);
  Allocate(heap, 6);
  CU_ASSERT(writeHeap(heap, 2, 'A', 10) == 0);
  CU_ASSERT(writeHeap(heap, 1, 'B', 10) == 0);
  CU_ASSERT(writeHeap(heap, 3, 'C', 2) == 0);
  CU_ASSERT(writeHeap(heap, 4, 'D', 2) == -1);
  Free(heap, 1);
  Free(heap, 2);
  Free(heap, 3);
}

void test_print_heap(void) {

}

int main() {
	CU_pSuite aSuite = NULL,
            fSuite = NULL,
            pSuite = NULL,
            hSuite = NULL;
	if (CUE_SUCCESS != CU_initialize_registry())
		return CU_get_error();

	aSuite = CU_add_suite("allocate suite", init_suite, clean_suite);
	fSuite = CU_add_suite("free suite", init_suite, clean_suite);
	pSuite = CU_add_suite("parsecommand suite", init_suite, clean_suite);
	hSuite = CU_add_suite("heap suite", init_suite, clean_suite);
	if (aSuite == NULL || fSuite == NULL || pSuite == NULL) {
		CU_cleanup_registry();
		return CU_get_error();
	}

	if (CU_add_test(pSuite, "test parsecommand", test_parsecommand) == NULL ||
			CU_add_test(fSuite, "test free", test_free) == NULL ||
			CU_add_test(hSuite, "test write heap", test_write_heap) == NULL ||
			CU_add_test(hSuite, "test print heap", test_print_heap) == NULL ||
			CU_add_test(aSuite, "test allocate max", test_allocate_maxing) == NULL ||
			CU_add_test(aSuite, "test allocate norm", test_allocate_normal) == NULL) {
		CU_cleanup_registry();
    return CU_get_error();
	}

	CU_basic_set_mode(CU_BRM_VERBOSE);
  //CU_automated_run_tests();
  CU_basic_run_tests();
  CU_cleanup_registry();
  return CU_get_error();
}
