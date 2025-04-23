#ifndef DEBUGGER_H
#define DEBUGGER_H

#include "api.h"
#include "loader.h"
#include "runner.h"

typedef enum {
    A_STEP,
    A_RESET,
    A_QUIT,
    A_INVALID,
    A_WAIT
} user_action;

user_action etb_get_event(etb_emu_6502 *emu);
int etb_debugger(etb_emu_6502 *emu, char *test_path, int test_index);

#endif
