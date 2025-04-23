#include <stdlib.h>
#include <stdio.h>

#include "debugger.h"
#include "api.h"
#include "loader.h"
#include "runner.h"

char *t_path;
int t_index, t_steps;

void print_reg(char *name, int value){ printf("%s: d: %d - h: [%x]\n", name, value, value); }
void print_regs(etb_emu_6502 *emu){
    etb_reg_6502 regs[] = {REG_6502_PC,REG_6502_S,REG_6502_A,REG_6502_X,REG_6502_Y,REG_6502_P};
    char *reg_names[] = {"PC","S","A","X","Y","P"};

    for(int i=0; i<6; i++){
        print_reg(reg_names[i], etb_emu_6502_reg_get(emu, regs[i]));
    }
}

user_action etb_get_event(etb_emu_6502 *emu){
    user_action a = A_INVALID;
    char inp;
    printf("\e[1;1H\e[2J");
    printf("File: %s\n", t_path);
    printf("Test index: %d\n", t_index);
    printf("Step: %d\n", t_steps);
    print_regs(emu);
    printf("\n[Q: Quit - S: Step - R: Reset]\n|>");

    scanf("%c", &inp);

    switch(inp){
        case 'q':
            a = A_QUIT;
            break;
        case 'Q':
            a = A_QUIT;
            break;
        case 's':
            a = A_STEP;
            break;
        case 'S':
            a = A_STEP;
            break;
        case 'r':
            a = A_RESET;
            break;
        case 'R':
            a = A_RESET;
            break;
        default:
            a = A_INVALID;
            printf("invalid command: %c\n", inp);
    }

    return a;
}


int etb_debugger(etb_emu_6502 *emu, char *test_path, int test_index){
    t_path = test_path;
    t_index = test_index;
    user_action action;

    int quit = 0;

    while(!quit){
        t_steps = 0;
        action = A_WAIT;

        uint16_t pc;
        uint8_t s, a, x, y, p;
        etb_loader loader;
        etb_init_loader_threaded(&loader, 1);

        etb_test *tests = load_6502_test(test_path);
        etb_test test = tests[test_index];

        // Initialize the registers to the Initial registers state of the test.
        set_regs(
            emu,
            test.initial->pc,
            test.initial->s,
            test.initial->a,
            test.initial->x,
            test.initial->y,
            test.initial->p
        );

        // Same with ram
        for(int j=0; j<test.initial->mem_len; j++){
            int addr = test.initial->mem[j].addr;
            int val = test.initial->mem[j].val;
            etb_emu_6502_ram_set(emu, addr, val);
        }

        // Get the current registers state.
        pc = (uint16_t)etb_emu_6502_reg_get(emu, REG_6502_PC);
        s = (uint8_t)etb_emu_6502_reg_get(emu, REG_6502_S);
        a = (uint8_t)etb_emu_6502_reg_get(emu, REG_6502_A);
        x = (uint8_t)etb_emu_6502_reg_get(emu, REG_6502_X);
        y = (uint8_t)etb_emu_6502_reg_get(emu, REG_6502_Y);
        p = (uint8_t)etb_emu_6502_reg_get(emu, REG_6502_P);


        while(!quit){
            while(action == A_WAIT || action == A_INVALID){
                action = etb_get_event(emu);
            }

            // Step emulator
            if(action == A_STEP){
                (*emu->step)();
                t_steps++;
            }

            // Reset emulator
            if(action == A_RESET){
                break;
            }

            // Quit
            if(action == A_QUIT){
                quit = 1;
                break;
            }

            action = A_WAIT;
        }
    }

    return 1;
}
