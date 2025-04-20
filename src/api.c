#include "api.h"
#include "loader.h"
#include "runner.h"

int etb_emu_6502_reg_get(etb_emu_6502* emu, etb_reg_6502 reg){ return *emu->regs[reg]; }
int etb_emu_6502_reg_set(etb_emu_6502* emu, etb_reg_6502 reg, int val){ *emu->regs[reg] = val; return *emu->regs[reg]; }

int etb_emu_6502_ram_get(etb_emu_6502* emu, int addr){ return emu->ram[addr]; }
int etb_emu_6502_ram_set(etb_emu_6502* emu, int addr, int val){ emu->ram[addr] = val; return emu->ram[addr]; }

void etb_emu_6502_hook_pc(etb_emu_6502 *emu, int *pc){ emu->regs[REG_6502_PC] = pc; }
void etb_emu_6502_hook_s(etb_emu_6502 *emu, int *s){ emu->regs[REG_6502_S] = s; }
void etb_emu_6502_hook_a(etb_emu_6502 *emu, int *a){ emu->regs[REG_6502_A] = a; }
void etb_emu_6502_hook_x(etb_emu_6502 *emu, int *x){ emu->regs[REG_6502_X] = x; }
void etb_emu_6502_hook_y(etb_emu_6502 *emu, int *y){ emu->regs[REG_6502_Y] = y; }
void etb_emu_6502_hook_p(etb_emu_6502 *emu, int *p){ emu->regs[REG_6502_P] = p; }
void etb_emu_6502_hook_ram(etb_emu_6502 *emu, int *ram){ emu->ram = ram; }

etb_test_status etb_run_6502_tests(etb_emu_6502 *emu, char *dir, int num_threads){
    etb_loader loader;
    etb_init_loader_threaded(&loader, num_threads);

    return TEST_FAILED;
}
