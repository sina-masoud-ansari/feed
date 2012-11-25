#include <stdlib.h>
#include <check.h>

#include "data_tests.h"

// TEMPLATE

/*
START_TEST (test_t1)
{
	fail_unless(1 == 2, NULL);
}
END_TEST
*
Suite* test_suite(void)
{
	Suite* s = suite_create("Example");
	TCase* tc_core = tcase_create("Core");
	tcase_add_test (tc_core, test_t1);
	suite_add_tcase (s, tc_core);

	return s;
}
*/

int main (void)
{
	int number_failed;

	number_failed += run_data_tests();

	return (number_failed == 0) ? EXIT_SUCCESS: EXIT_FAILURE;
}


