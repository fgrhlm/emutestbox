#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <setjmp.h>
#include <string.h>
#include <cmocka.h>
#include <dirent.h>

#include "cpu.h"

#include "api.h"
#include "loader.h"
#include "runner.h"

// f6502 setup
uint16_t pc;
cpu c;
mem *m;

void cpu_step_wrapper(){
    set_reg(&c, REG_PH, ((uint16_t)pc >> 8));
    set_reg(&c, REG_PL, ((uint16_t)pc & 0x00FF));
    cpu_tick(&c,m);
    pc = (*get_reg(&c, REG_PH) << 4) | *get_reg(&c, REG_PL);
}

// Null case
static void null_case(void **state){
    (void) state;
}

static void test_ref(void **state){
    (void) state;

    int pass=0, fail=0;

    //cycles += step_cpu(verbose);
    char dir_path[] = "/home/rcd/proj/65x02/6502/v2/";

    struct dirent *d;
    DIR *test_dir = opendir(dir_path);

    if(test_dir == NULL){
        printf("could not open test dir!\n");
        exit(1);
    }

    while ((d = readdir(test_dir)) != NULL) {
        if(!strcmp(d->d_name, ".") || !strcmp(d->d_name,"..")){
            continue;
        }

        etb_emu_6502 emu;

        // f6502 setup
        m = create_mem(65535);
        reset_cpu(&c);
        reset_mem(m);

        etb_emu_6502_hook_pc(&emu, &pc);
        etb_emu_6502_hook_s(&emu, get_reg(&c, REG_S));
        etb_emu_6502_hook_a(&emu, get_reg(&c, REG_A));
        etb_emu_6502_hook_x(&emu, get_reg(&c, REG_X));
        etb_emu_6502_hook_y(&emu, get_reg(&c, REG_Y));
        etb_emu_6502_hook_p(&emu, get_reg(&c, REG_P));
        etb_emu_6502_hook_ram(&emu, m->bytes);
        etb_emu_6502_hook_step(&emu, (void*)cpu_step_wrapper);

        char t_path[strlen(dir_path)];
        strcpy(t_path, dir_path);

        strcat(t_path, d->d_name);

        etb_test_status status = etb_run_6502_test(&emu, t_path, 1);

        switch(status){
            case TEST_PASSED:
                printf("PASSED\n");
                pass++;
                break;
        case TEST_FAILED:
                printf("FAILED\n");
                fail++;
                break;
            default:
                break;
        }
    }

    printf("PASSED: %d\nFAILED: %d\n", pass, fail);
    assert_int_equal(0,0);
    closedir(test_dir);
}

int main(void){
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(null_case),
        cmocka_unit_test(test_ref),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
