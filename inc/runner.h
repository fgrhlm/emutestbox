#ifndef RUNNER_H
#define RUNNER_H

#define SINGLE_STEP_TESTS_LEN 10000

#include "loader.h"

typedef enum { PASSED, FAILED } TEST_STATUS;

typedef struct {
    char *file;
    char *name;
    int index;
    TEST_STATUS status;
    char *msg;
} singlesteptests_6502_test_result;

#endif
