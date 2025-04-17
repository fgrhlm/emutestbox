#ifndef API_H
#define API_H

#include "loader.h"
#include "runner.h"

typedef enum { CYCLE_READ, CYCLE_WRITE } etb_emu_cycle_type;
typedef enum { SINGLE_THREADED, MULTI_THREADED } runner_multi;

typedef int (*ext_int)();

typedef struct {
    char *name;
    uint8_t *val;
} etb_emu_reg_8;

typedef struct {
    char *name;
    uint16_t *val;
} etb_emu_reg_16;

typedef struct {
    etb_emu_cycle_type t;
    uint8_t val;
} etb_emu_cycle;

typedef struct {

} etb_emu;

// Link registers
void etb_emu_define_regs();
// Link RAM
void etb_emu_define_ram();
// Link step
void etb_emu_define_step();

// SingleStepTests
int etb_emu_run_single_step_tests(char *path, int dir, singlesteptests_6502_test_result *results){
    results = malloc(sizeof(singlesteptests_6502_test_result) * SINGLE_STEP_TESTS_LEN);

    return 0;
}

void etb_emu_destroy_results(singlesteptests_6502_test_result *results){
    free(results);
}

#endif
