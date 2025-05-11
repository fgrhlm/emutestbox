#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <setjmp.h>
#include <string.h>
#include <cmocka.h>
#include <dirent.h>

#include "6502.h"

#include "api.h"
#include "loader.h"
#include "runner.h"

// Null case
static void null_case(void **state){
    (void) state;
}

static void test_ref(void **state){
    (void) state;

    int pass=0, fail=0;

    //cycles += step_cpu(verbose);
    char dir_path[] = "./dummy_tests/";

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

        // Reference emu setup
        reset_cpu(0,0,0,0,0,0);
        memset(memory, 0, sizeof(memory));
        etb_emu_6502_hook_pc(&emu, &PC);
        etb_emu_6502_hook_s(&emu, &SP);
        etb_emu_6502_hook_a(&emu, &A);
        etb_emu_6502_hook_x(&emu, &X);
        etb_emu_6502_hook_y(&emu, &Y);
        etb_emu_6502_hook_p(&emu, &SR.byte);
        etb_emu_6502_hook_ram(&emu, memory);
        etb_emu_6502_hook_step(&emu, (void*)step_cpu);

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
