#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <setjmp.h>
#include <string.h>
#include <cmocka.h>
#include <dirent.h>

#include "6502.h"

#include "api.h"
#include "debugger.h"

// 6502-emu step wrapper to reduce verbosity
void step_wrapper(){
    step_cpu(0);
}

// Null case
static void null_case(void **state){
    (void) state;
}

// emu cpu step wrapper to pass verbose = 0
static void test_debugger(void **state){
    (void) state;

    char t_path[] = "/home/rcd/proj/65x02/6502/v2/01.json";
    int t_index = 0;

    etb_emu_6502 emu;

    // Reference emu setup
    reset_cpu(0,0,0,0,0,0);
    memset(memory, 0, sizeof(memory));
    etb_emu_6502_hook_pc(&emu, &PC);
    etb_emu_6502_hook_s(&emu, &SP);
    etb_emu_6502_hook_a(&emu, &A);
    etb_emu_6502_hook_x(&emu, &X);
    etb_emu_6502_hook_y(&emu, &Y);
    etb_emu_6502_hook_p(&emu, &SR.byte);
    etb_emu_6502_hook_ram(&emu, memory);
    etb_emu_6502_hook_step(&emu, (void*)step_wrapper);

    int success = etb_debugger(&emu, t_path, t_index);

    assert_int_equal(1, success);
}

int main(void){
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(null_case),
        cmocka_unit_test(test_debugger),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
