#ifndef API_H
#define API_H

#include "runner.h"

typedef enum {
    REG_6502_PC,
    REG_6502_S,
    REG_6502_A,
    REG_6502_X,
    REG_6502_Y,
    REG_6502_P
} etb_reg_6502;

typedef enum { CYCLE_READ, CYCLE_WRITE } etb_cycle_type;
typedef enum { SINGLE_THREADED, MULTI_THREADED } threads;

typedef int (*ext_int)();

typedef struct {
    int *regs[6];
    int *ram;
    etb_cycle_type *cycles_type;
    int *cycles_val;
} etb_emu_6502;

etb_test_status etb_run_6502_tests(etb_emu_6502 *emu, char *dir, int num_threads);

void etb_emu_6502_hook_pc(etb_emu_6502 *emu, int *pc);
void etb_emu_6502_hook_s(etb_emu_6502 *emu, int *s);
void etb_emu_6502_hook_a(etb_emu_6502 *emu, int *a);
void etb_emu_6502_hook_x(etb_emu_6502 *emu, int *x);
void etb_emu_6502_hook_y(etb_emu_6502 *emu, int *y);
void etb_emu_6502_hook_p(etb_emu_6502 *emu, int *p);
void etb_emu_6502_hook_ram(etb_emu_6502 *emu, int *ram);

int etb_emu_6502_reg_get(etb_emu_6502 *emu, etb_reg_6502 reg);
int etb_emu_6502_reg_set(etb_emu_6502 *emu, etb_reg_6502 reg, int val);

int etb_emu_6502_ram_get(etb_emu_6502 *emu, int addr);
int etb_emu_6502_ram_set(etb_emu_6502 *emu, int addr, int val);

int etb_emu_6502_step(etb_emu_6502 *emu);

#endif
