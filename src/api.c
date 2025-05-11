#include "api.h"
#include "loader.h"
#include "runner.h"

int TEST_VERBOSE = 2;
// 1 - Only print last fail count
// 2 - Print individual failed cases
// 3 - Print everything

int etb_emu_6502_reg_get(etb_emu_6502* emu, etb_reg_6502 reg){
    // Gets the value stored in emulator (@emu) register (@reg).

    if(reg == REG_6502_PC){
        return *emu->pc;
    }

    return (int)*emu->regs[reg];
}

int etb_emu_6502_reg_set(etb_emu_6502* emu, etb_reg_6502 reg, int val){
    // Sets the value (@val) stored in emulator (@emu) register (@reg).

    if(reg == REG_6502_PC){
        *emu->pc = val; return *emu->pc;
    }else{
        *emu->regs[reg] = val; return *emu->regs[reg];
    }
}

int etb_emu_6502_ram_get(etb_emu_6502* emu, int addr){
    // Gets the value stored in the emulators (@emu) ram at @addr.

    return emu->ram[addr];
}

int etb_emu_6502_ram_set(etb_emu_6502* emu, int addr, uint8_t val){
    // Sets the value in the emulators (@emu) ram at @addr to value (@val).

    emu->ram[addr] = (uint8_t)val;

    return emu->ram[addr];
}

/*
 *  These functions are used to "hook" your emulators registers to the emutestbox dummy emulator.
 *  This is done by supplying a pointer to the register as the second argument to any of the following functions.
 */
void etb_emu_6502_hook_pc(etb_emu_6502 *emu, uint16_t *pc){ emu->pc = pc; }
void etb_emu_6502_hook_s(etb_emu_6502 *emu, uint8_t *s){ emu->regs[REG_6502_S] = s; }
void etb_emu_6502_hook_a(etb_emu_6502 *emu, uint8_t *a){ emu->regs[REG_6502_A] = a; }
void etb_emu_6502_hook_x(etb_emu_6502 *emu, uint8_t *x){ emu->regs[REG_6502_X] = x; }
void etb_emu_6502_hook_y(etb_emu_6502 *emu, uint8_t *y){ emu->regs[REG_6502_Y] = y; }
void etb_emu_6502_hook_p(etb_emu_6502 *emu, uint8_t *p){ emu->regs[REG_6502_P] = p; }

/*
 *  Same as the above but this time with your emulators memory.
 *  The hook function expects a plain array of uint8_t. If this is not compatible with your emulator
 *  you can probably get around it by wrapping your emulation stepping function and copying over your
 *  RAM values to a separate array. Idk.
 */
void etb_emu_6502_hook_ram(etb_emu_6502 *emu, uint8_t *ram){ emu->ram = ram; }

/* Same principle as above, expects a pointer to a function that steps the emulation forward by _one_ step.
 * If your emulator does not have this functionality readily available, please feel free to define one.
*/
void etb_emu_6502_hook_step(etb_emu_6502 *emu, void (*step)(void)){ emu->step = step; }

/* Utility function (i.e not meant for public use) to notify about mismatching register values.
 * @reg is the register which value was incorrect.
 * @expected is what was.. expected.
 * @value is the actual value that was encountered.
*/
int print_reg_failed(char *reg, int expected, int value){
    if(TEST_VERBOSE > 1){
        printf("Assert failed: %s (should be: %d was %d)\n", reg, expected, value);
    }
    return 1;
}

/* Utility function (i.e not meant for public use) to initialize all registers at once.
*/
void set_regs(etb_emu_6502 *emu, int pc, int s, int a, int x, int y, int p){
    etb_emu_6502_reg_set(emu, REG_6502_PC, pc);
    etb_emu_6502_reg_set(emu, REG_6502_S, s);
    etb_emu_6502_reg_set(emu, REG_6502_A, a);
    etb_emu_6502_reg_set(emu, REG_6502_X, x);
    etb_emu_6502_reg_set(emu, REG_6502_Y, y);
    etb_emu_6502_reg_set(emu, REG_6502_P, p);
}

// Utility function that tests if a 16bit value @a matches another 16bit value @b
int assert_reg_16(char *reg, uint16_t a, uint16_t b){
    if(a != b){
        if(TEST_VERBOSE >= 2){
            printf("Assert failed: %s (should be: %d was %d)\n", reg, a, b);
        }

        return 1;

    }

    if(TEST_VERBOSE == 3){
        printf("Assert OK: %s (should be: %d was %d)\n", reg, a, b);
    }

    return 0;
}

// Utility function that tests if a 8bit value @a matches another 8bit value @b
int assert_reg_8(char *reg, uint8_t a, uint8_t b){
    if(a != b){
        if(TEST_VERBOSE >= 2){
            printf("Assert failed: %s (should be: %d was %d)\n", reg, a, b);
        }

        return 1;
    }

    if(TEST_VERBOSE == 3){
        printf("Assert OK: %s (should be: %d was %d)\n", reg, a, b);
    }

    return 0;
}

/* Load and run single SingleStepTests 6502 definition.
 * @emu - Pointer to a emutestbox dummy emulator.
 * @test_path - Path to testd efinition.
 * @num_threads - Should always be 1 since threaded test runs are WIP.
 * */
etb_test_status etb_run_6502_test(etb_emu_6502 *emu, char *test_path, int num_threads){
    // Initial and final Program Counter values
    uint16_t i_pc, f_pc;
    // Initial and final Register values
    uint8_t i_s, i_a, i_x, i_y, i_p;
    uint8_t f_s, f_a, f_x, f_y, f_p;

    // Loader initialization.
    etb_loader loader;
    etb_init_loader_threaded(&loader, num_threads);

    // Load the test
    etb_test *tests = load_6502_test(test_path);

    // Counters for fail/pass ratio.
    long res_pass = 0, res_fail = 0;

    // Initialize test status.
    int status = TEST_PASSED;

    // For every individual test in file at @test_path
    for(int i=0; i<tests[0].len; i++){
        // A non-zero results means an error was encountered. Value reflects the amount of errors.
        int reg_results = 0;
        int ram_results = 0;

        // Initialize the registers to the Initial registers state of the test.
        set_regs(
            emu,
            tests[i].initial->pc,
            tests[i].initial->s,
            tests[i].initial->a,
            tests[i].initial->x,
            tests[i].initial->y,
            tests[i].initial->p
        );

        // Same with ram
        for(int j=0; j<tests[i].initial->mem_len; j++){
            int addr = tests[i].initial->mem[j].addr;
            int val = tests[i].initial->mem[j].val;
            etb_emu_6502_ram_set(emu, addr, val);
        }

        // Get the current registers state.
        i_pc = (uint16_t)etb_emu_6502_reg_get(emu, REG_6502_PC);
        i_s = (uint8_t)etb_emu_6502_reg_get(emu, REG_6502_S);
        i_a = (uint8_t)etb_emu_6502_reg_get(emu, REG_6502_A);
        i_x = (uint8_t)etb_emu_6502_reg_get(emu, REG_6502_X);
        i_y = (uint8_t)etb_emu_6502_reg_get(emu, REG_6502_Y);
        i_p = (uint8_t)etb_emu_6502_reg_get(emu, REG_6502_P);

        // Check if registers hold the expected values.
        reg_results += assert_reg_16("Init. PC", tests[i].initial->pc, i_pc);
        reg_results += assert_reg_8("Init. S", tests[i].initial->s, i_s);
        reg_results += assert_reg_8("Init. A", tests[i].initial->a, i_a);
        reg_results += assert_reg_8("Init. X", tests[i].initial->x, i_x);
        reg_results += assert_reg_8("Init. Y", tests[i].initial->y, i_y);
        reg_results += assert_reg_8("Init. P", tests[i].initial->p, i_p);

        // Same with ram
        for(int j=0; j<tests[i].initial->mem_len; j++){
            int addr = tests[i].initial->mem[j].addr;
            int val = tests[i].initial->mem[j].val;

            int initial = etb_emu_6502_ram_get(emu, addr);

            if(initial != val){
                ram_results++;

                if(TEST_VERBOSE >= 2){
                    printf("Assert failed: RAM->%x (should be: %d was %d)\n", addr, val, initial);
                }
            }else{
                if(TEST_VERBOSE == 3){
                    printf("Assert OK: RAM->%x (should be: %d was %d)\n", addr, val, initial);
                }
            }
        }

        // Step emulator
        (*emu->step)();

        // Get registers state.
        f_pc = (uint16_t)etb_emu_6502_reg_get(emu, REG_6502_PC);
        f_s = etb_emu_6502_reg_get(emu, REG_6502_S);
        f_a = etb_emu_6502_reg_get(emu, REG_6502_A);
        f_x = etb_emu_6502_reg_get(emu, REG_6502_X);
        f_y = etb_emu_6502_reg_get(emu, REG_6502_Y);
        f_p = etb_emu_6502_reg_get(emu, REG_6502_P);

        // Check if registers hold the expected values.
        reg_results += assert_reg_16("Final PC", tests[i].final->pc, f_pc);
        reg_results += assert_reg_8("Final S", tests[i].final->s, f_s);
        reg_results += assert_reg_8("Final A", tests[i].final->a, f_a);
        reg_results += assert_reg_8("Final X", tests[i].final->x, f_x);
        reg_results += assert_reg_8("Final Y", tests[i].final->y, f_y);
        reg_results += assert_reg_8("Final P", tests[i].final->p, f_p);

        // Same with ram
        for(int j=0; j<tests[i].final->mem_len; j++){
            int addr = tests[i].final->mem[j].addr;
            int val = tests[i].final->mem[j].val;
            int final = etb_emu_6502_ram_get(emu, addr);

            if(final != val){
                ram_results++;

                if(TEST_VERBOSE >= 2){
                    printf("Assert failed: RAM->%x (should be: %d was %d)\n", addr, val, final);
                }
            }else{
                if(TEST_VERBOSE == 3){
                    printf("Assert OK: RAM->%x (should be: %d was %d)\n", addr, val, final);
                }
            }
        }

        // Compute the final results.
        status = (reg_results == 0) && (ram_results == 0) ? TEST_PASSED : TEST_FAILED;

        status == TEST_PASSED ? res_pass++ : res_fail++;

        if(TEST_VERBOSE >= 1){
            if(status == TEST_FAILED){
                printf("Test Failed: %s -> %d\n", test_path, i);
            }

        }

        if(TEST_VERBOSE == 3){
            if(status == TEST_PASSED){
                printf("Test OK: %s -> %d\n", test_path, i);
            }
        }

    }

    // Calculate final test status and present/return results.
    status = res_fail > 0 ? TEST_PASSED : TEST_FAILED;

    if(status == TEST_PASSED){
        printf("[OK!](%s) PASS:%lu FAIL: %lu\n", test_path, res_pass, res_fail);
    }

    if(status == TEST_FAILED){
        printf("[FAILED!](%s) PASS:%lu FAIL: %lu\n", test_path, res_pass, res_fail);
    }

    return status;
}
