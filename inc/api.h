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
    int reg_pc, reg_s, reg_a, reg_x, reg_y, reg_p;
    int *ram;
    etb_cycle_type *cycles_type;
    int *cycles_val;
} etb_emu_6502;

/*

int etb_emu_6502_reg_get(char* reg_name){ return 0; };
int etb_emu_6502_reg_set(etb_reg_6502 reg, int val){ return 0; };

int etb_emu_6502_ram_get(int addr){ return 0; };
int etb_emu_6502_ram_set(int addr, int val){ return 0; };

int etb_emu_6502_step(){ return 0; };

*/

#endif
