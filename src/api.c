#include "api.h"
#include "loader.h"
#include "runner.h"

const int TEST_VERBOSE = 1;

int etb_emu_6502_reg_get(etb_emu_6502* emu, etb_reg_6502 reg){
    if(reg == REG_6502_PC){
        return *emu->pc;
    }

    return *emu->regs[reg];
}
int etb_emu_6502_reg_set(etb_emu_6502* emu, etb_reg_6502 reg, int val){
    if(reg == REG_6502_PC){
        *emu->pc = val; return *emu->pc;
    }else{
        *emu->regs[reg] = val; return *emu->regs[reg];
    }
}

int etb_emu_6502_ram_get(etb_emu_6502* emu, int addr){ return emu->ram[addr]; }
int etb_emu_6502_ram_set(etb_emu_6502* emu, int addr, uint8_t val){ emu->ram[addr] = (uint8_t)val; return emu->ram[addr]; }

void etb_emu_6502_hook_pc(etb_emu_6502 *emu, uint16_t *pc){ emu->pc = pc; }
void etb_emu_6502_hook_s(etb_emu_6502 *emu, uint8_t *s){ emu->regs[REG_6502_S] = s; }
void etb_emu_6502_hook_a(etb_emu_6502 *emu, uint8_t *a){ emu->regs[REG_6502_A] = a; }
void etb_emu_6502_hook_x(etb_emu_6502 *emu, uint8_t *x){ emu->regs[REG_6502_X] = x; }
void etb_emu_6502_hook_y(etb_emu_6502 *emu, uint8_t *y){ emu->regs[REG_6502_Y] = y; }
void etb_emu_6502_hook_p(etb_emu_6502 *emu, uint8_t *p){ emu->regs[REG_6502_P] = p; }
void etb_emu_6502_hook_ram(etb_emu_6502 *emu, uint8_t *ram){ emu->ram = ram; }
void etb_emu_6502_hook_step(etb_emu_6502 *emu, void (*step)(void)){ emu->step = step; }

int print_reg_failed(char *reg, int expected, int value){
    if(TEST_VERBOSE){
        printf("Assert failed: %s (should be: %d was %d)\n", reg, expected, value);
    }
    return 1;
}

void set_regs(etb_emu_6502 *emu, int pc, int s, int a, int x, int y, int p){
    etb_emu_6502_reg_set(emu, REG_6502_PC, pc);
    etb_emu_6502_reg_set(emu, REG_6502_S, s);
    etb_emu_6502_reg_set(emu, REG_6502_A, a);
    etb_emu_6502_reg_set(emu, REG_6502_X, x);
    etb_emu_6502_reg_set(emu, REG_6502_Y, y);
    etb_emu_6502_reg_set(emu, REG_6502_P, p);
}

int assert_reg(char *reg, uint16_t a, uint16_t b){
    if(a != b){
        if(TEST_VERBOSE){
            printf("Assert failed: %s (should be: %d was %d)\n", reg, a, b);
        }
        return 1;
    }

    return 0;
}

int assert_reg_8(char* reg, uint8_t a, uint8_t b){ return assert_reg(reg, (int)a, (int)b); }
int assert_reg_16(char* reg, uint16_t a, uint16_t b){ return assert_reg(reg, (int)a, (int)b); }

etb_test_status etb_run_6502_test(etb_emu_6502 *emu, char *test_path, int num_threads){
    printf("Run test: %s..\n", test_path);
    uint16_t f_pc;
    uint8_t f_s, f_a, f_x, f_y, f_p;
    etb_loader loader;
    etb_init_loader_threaded(&loader, num_threads);

    etb_test *tests = load_6502_test(test_path);

    int status = TEST_PASSED;

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
        f_pc = (uint16_t)etb_emu_6502_reg_get(emu, REG_6502_PC);
        f_s = (uint8_t)etb_emu_6502_reg_get(emu, REG_6502_S);
        f_a = (uint8_t)etb_emu_6502_reg_get(emu, REG_6502_A);
        f_x = (uint8_t)etb_emu_6502_reg_get(emu, REG_6502_X);
        f_y = (uint8_t)etb_emu_6502_reg_get(emu, REG_6502_Y);
        f_p = (uint8_t)etb_emu_6502_reg_get(emu, REG_6502_P);

        // Check if registers hold the expected values.
        reg_results += assert_reg_16("PC", tests[i].initial->pc, f_pc);
        reg_results += assert_reg_8("S", tests[i].initial->s, f_s);
        reg_results += assert_reg_8("A", tests[i].initial->a, f_a);
        reg_results += assert_reg_8("X", tests[i].initial->x, f_x);
        reg_results += assert_reg_8("Y", tests[i].initial->y, f_y);
        reg_results += assert_reg_8("P", tests[i].initial->p, f_p);

        // Same with ram
        for(int j=0; j<tests[i].initial->mem_len; j++){
            int addr = tests[i].initial->mem[j].addr;
            int val = tests[i].initial->mem[j].val;

            int initial = etb_emu_6502_ram_get(emu, addr);

            if(initial != val){
                ram_results++;
                if(TEST_VERBOSE){ return 1; }
                printf("[ASSERT FAIL]: RAM [%d]\n\tExpected: %d\n\tValue: %d\n",addr,val,initial);
            }
        }

        // Step emulator
        etb_emu_6502_reg_set(emu, REG_6502_PC, *emu->pc+1);
        (*emu->step)();

        // Get registers state.
        //f_pc = etb_emu_6502_reg_get(emu, REG_6502_PC);
        f_s = etb_emu_6502_reg_get(emu, REG_6502_S);
        f_a = etb_emu_6502_reg_get(emu, REG_6502_A);
        f_x = etb_emu_6502_reg_get(emu, REG_6502_X);
        f_y = etb_emu_6502_reg_get(emu, REG_6502_Y);
        f_p = etb_emu_6502_reg_get(emu, REG_6502_P);

        // Check if registers hold the expected values.
        reg_results += assert_reg_16("PC", tests[i].final->pc, f_pc);
        reg_results += assert_reg_8("S", tests[i].final->s, f_s);
        reg_results += assert_reg_8("A", tests[i].final->a, f_a);
        reg_results += assert_reg_8("X", tests[i].final->x, f_x);
        reg_results += assert_reg_8("Y", tests[i].final->y, f_y);
        reg_results += assert_reg_8("P", tests[i].final->p, f_p);

        // Same with ram
        for(int j=0; j<tests[i].final->mem_len; j++){
            int addr = tests[i].final->mem[j].addr;
            int val = tests[i].final->mem[j].val;
            int final = etb_emu_6502_ram_get(emu, addr);

            if(final != val){
                ram_results++;
                if(TEST_VERBOSE){
                    printf("[ASSERT FAIL]: RAM [%d]\n\tExpected: %d\n\tValue: %d\n",addr,val,final);
                }
            }
        }

        // Compute the final results.
        status = (reg_results == 0) && (ram_results == 0) ? TEST_PASSED : TEST_FAILED;
        if(status == TEST_FAILED && TEST_VERBOSE){
            printf("!! [FAILED]: file: %s -> %d\n", test_path, i);
        }
    }

    return status;
}
