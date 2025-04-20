#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <setjmp.h>
#include <cmocka.h>

#include "api.h"

int dum_pc = 1;
int dum_s = 2;
int dum_a = 3;
int dum_x = 4;
int dum_y = 5;
int dum_p = 6;
int dum_ram[3] = {1,2,3};

int etb_emu_6502_step(etb_emu_6502 *emu){
    *emu->regs[REG_6502_PC] = 12;
    return *emu->regs[REG_6502_PC];
}

// Null case
static void null_case(void **state){
    (void) state;
}

static void t_hooks_get_set(void **state){
    (void) state;

    etb_emu_6502 emu;

    etb_emu_6502_hook_pc(&emu, &dum_pc);
    assert_int_equal(etb_emu_6502_reg_get(&emu, REG_6502_PC), 1);

    etb_emu_6502_hook_s(&emu, &dum_s);
    assert_int_equal(etb_emu_6502_reg_get(&emu, REG_6502_S), 2);

    etb_emu_6502_hook_a(&emu, &dum_a);
    assert_int_equal(etb_emu_6502_reg_get(&emu, REG_6502_A), 3);

    etb_emu_6502_hook_x(&emu, &dum_x);
    assert_int_equal(etb_emu_6502_reg_get(&emu, REG_6502_X), 4);

    etb_emu_6502_hook_y(&emu, &dum_y);
    assert_int_equal(etb_emu_6502_reg_get(&emu, REG_6502_Y), 5);

    etb_emu_6502_hook_p(&emu, &dum_p);
    assert_int_equal(etb_emu_6502_reg_get(&emu, REG_6502_P), 6);

    etb_emu_6502_hook_ram(&emu, dum_ram);

    assert_int_equal(etb_emu_6502_ram_get(&emu, 0), 1);
    assert_int_equal(etb_emu_6502_ram_get(&emu, 1), 2);
    assert_int_equal(etb_emu_6502_ram_get(&emu, 2), 3);

    etb_emu_6502_reg_set(&emu, REG_6502_PC, 101);
    etb_emu_6502_reg_set(&emu, REG_6502_S, 102);
    etb_emu_6502_reg_set(&emu, REG_6502_A, 103);
    etb_emu_6502_reg_set(&emu, REG_6502_X, 104);
    etb_emu_6502_reg_set(&emu, REG_6502_Y, 105);
    etb_emu_6502_reg_set(&emu, REG_6502_P, 106);

    etb_emu_6502_ram_set(&emu, 0, 101);
    etb_emu_6502_ram_set(&emu, 1, 102);
    etb_emu_6502_ram_set(&emu, 2, 103);

    assert_int_equal(etb_emu_6502_reg_get(&emu, REG_6502_PC), 101);
    assert_int_equal(etb_emu_6502_reg_get(&emu, REG_6502_S), 102);
    assert_int_equal(etb_emu_6502_reg_get(&emu, REG_6502_A), 103);
    assert_int_equal(etb_emu_6502_reg_get(&emu, REG_6502_X), 104);
    assert_int_equal(etb_emu_6502_reg_get(&emu, REG_6502_Y), 105);
    assert_int_equal(etb_emu_6502_reg_get(&emu, REG_6502_P), 106);

    assert_int_equal(etb_emu_6502_ram_get(&emu, 0), 101);
    assert_int_equal(etb_emu_6502_ram_get(&emu, 1), 102);
    assert_int_equal(etb_emu_6502_ram_get(&emu, 2), 103);
}


static void t_step(void **state){
    (void) state;

    etb_emu_6502 emu;

    etb_emu_6502_hook_pc(&emu, &dum_pc);
    assert_int_equal(etb_emu_6502_reg_get(&emu, REG_6502_PC), 101);

    etb_emu_6502_step(&emu);
    assert_int_equal(etb_emu_6502_reg_get(&emu, REG_6502_PC), 12);
}

static void t_run_6502_tests(void **state){
    (void) state;

    etb_emu_6502 emu;
    etb_test_status status = etb_run_6502_tests(&emu, "./build", 1);

    assert_int_equal(status, TEST_FAILED);
}

int main(void){
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(null_case),
        cmocka_unit_test(t_hooks_get_set),
        cmocka_unit_test(t_step),
        cmocka_unit_test(t_run_6502_tests)
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
