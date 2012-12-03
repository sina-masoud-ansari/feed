#include <stdlib.h>
#include <check.h>

#include "data_tests.h"

int main (void)
{
	int num_failed;

	run_data_tests(&num_failed);

	return (num_failed == 0) ? EXIT_SUCCESS: EXIT_FAILURE;
}


