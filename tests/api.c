#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <setjmp.h>
#include <cmocka.h>

#include "api.h"

int pc = 0;
int regs[] = {0,1,2,3,4,5};
int ram[] = {0,1,2,3,4,5};

int t_reg_get(etb_reg_6502 reg){ return regs[reg]; }
void t_reg_set(etb_reg_6502 reg, int val){ regs[reg] = val; }
int t_ram_get(int addr){ return ram[addr]; }
void t_ram_set(int addr, int val){ ram[addr] = val; }
int t_step(){ pc++; return pc; }

// Mock emu hooks
#define ETB_EMU_6502_REG_GET t_reg_get
#define ETB_EMU_6502_REG_SET t_reg_set
#define ETB_EMU_6502_RAM_GET t_ram_get
#define ETB_EMU_6502_RAM_SET t_ram_set
#define ETB_EMU_6502_STEP  t_step

// Null case
static void null_case(void **state){
    (void) state;
}

static void t_etb_test_reg(void **state){
    (void) state;

    // Get
    assert_int_equal(t_reg_get(REG_6502_PC), 0);
    assert_int_equal(t_reg_get(REG_6502_S), 1);
    assert_int_equal(t_reg_get(REG_6502_A), 2);
    assert_int_equal(t_reg_get(REG_6502_X), 3);
    assert_int_equal(t_reg_get(REG_6502_Y), 4);
    assert_int_equal(t_reg_get(REG_6502_P), 5);

    // Set
    t_reg_set(REG_6502_PC, 100);
    t_reg_set(REG_6502_S, 101);
    t_reg_set(REG_6502_A, 102);
    t_reg_set(REG_6502_X, 103);
    t_reg_set(REG_6502_Y, 104);
    t_reg_set(REG_6502_P, 105);

    assert_int_equal(t_reg_get(REG_6502_PC), 100);
    assert_int_equal(t_reg_get(REG_6502_S), 101);
    assert_int_equal(t_reg_get(REG_6502_A), 102);
    assert_int_equal(t_reg_get(REG_6502_X), 103);
    assert_int_equal(t_reg_get(REG_6502_Y), 104);
    assert_int_equal(t_reg_get(REG_6502_P), 105);
}

static void t_etb_test_ram(void **state){
    (void) state;

    // Get
    assert_int_equal(t_ram_get(0), 0);
    assert_int_equal(t_ram_get(1), 1);
    assert_int_equal(t_ram_get(2), 2);
    assert_int_equal(t_ram_get(3), 3);
    assert_int_equal(t_ram_get(4), 4);

    // Set
    t_ram_set(0, 100);
    t_ram_set(1, 101);
    t_ram_set(2, 102);
    t_ram_set(3, 103);
    t_ram_set(4, 104);

    assert_int_equal(t_ram_get(0), 100);
    assert_int_equal(t_ram_get(1), 101);
    assert_int_equal(t_ram_get(2), 102);
    assert_int_equal(t_ram_get(3), 103);
    assert_int_equal(t_ram_get(4), 104);
}

static void t_etb_test_step(void **state){
    (void) state;

    int old_pc = pc;

    assert_int_equal(t_step(), (old_pc + 1));
}

static void t_etb_test_6502_single_step_tests(void **state){
    (void) state;
}

int main(void){
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(null_case),
        cmocka_unit_test(t_etb_test_reg),
        cmocka_unit_test(t_etb_test_ram),
        cmocka_unit_test(t_etb_test_step),
        cmocka_unit_test(t_etb_test_6502_single_step_tests)
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
