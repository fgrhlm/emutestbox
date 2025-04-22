#ifndef API_H
#define API_H

#include "runner.h"

typedef enum {
    REG_6502_PC = -1,
    REG_6502_S = 0,
    REG_6502_A = 1,
    REG_6502_X = 2,
    REG_6502_Y = 3,
    REG_6502_P = 4
} etb_reg_6502;

typedef enum { CYCLE_READ, CYCLE_WRITE } etb_cycle_type;
typedef enum { SINGLE_THREADED, MULTI_THREADED } threads;

typedef int (*ext_int)();

typedef struct {
    uint8_t *regs[6];
    uint16_t *pc;
    uint8_t *ram;
    void (*step)(void);
    etb_cycle_type *cycles_type;
    int *cycles_val;
} etb_emu_6502;

etb_test_status etb_run_6502_test(etb_emu_6502 *emu, char *test_path, int num_threads);

void etb_emu_6502_hook_pc(etb_emu_6502 *emu, uint16_t *pc);
void etb_emu_6502_hook_s(etb_emu_6502 *emu, uint8_t *s);
void etb_emu_6502_hook_a(etb_emu_6502 *emu, uint8_t *a);
void etb_emu_6502_hook_x(etb_emu_6502 *emu, uint8_t *x);
void etb_emu_6502_hook_y(etb_emu_6502 *emu, uint8_t *y);
void etb_emu_6502_hook_p(etb_emu_6502 *emu, uint8_t *p);
void etb_emu_6502_hook_ram(etb_emu_6502 *emu, uint8_t *ram);
void etb_emu_6502_hook_step(etb_emu_6502 *emu, void (*step)(void));

int etb_emu_6502_reg_get(etb_emu_6502 *emu, etb_reg_6502 reg);
int etb_emu_6502_reg_set(etb_emu_6502 *emu, etb_reg_6502 reg, int val);

int etb_emu_6502_ram_get(etb_emu_6502 *emu, int addr);
int etb_emu_6502_ram_set(etb_emu_6502 *emu, int addr, uint8_t val);

#endif
