#ifndef RUNNER_H
#define RUNNER_H

#define SINGLE_STEP_TESTS_LEN 10000

#include "loader.h"

typedef enum { TEST_PASSED, TEST_FAILED } etb_test_status;

typedef struct {
    char *file;
    char *name;
    int index;
    etb_test_status status;
    char *msg;
} test_result_6502;

test_result_6502 run_6502_test(etb_test *test);

#endif
