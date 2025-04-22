#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <setjmp.h>
#include <string.h>
#include <cmocka.h>
#include <dirent.h>

#include "api.h"

uint16_t dum_pc = 1;
uint8_t dum_s = 2;
uint8_t dum_a = 3;
uint8_t dum_x = 4;
uint8_t dum_y = 5;
uint8_t dum_p = 6;
uint8_t dum_ram[3] = {1,2,3};

void etb_emu_6502_step(){
    dum_pc = 12;
}

// Null case
static void null_case(void **state){
    (void) state;
}

static void t_hooks_get_set(void **state){
    (void) state;

    etb_emu_6502 emu;

    etb_emu_6502_hook_pc(&emu, &dum_pc);
    assert_int_equal(etb_emu_6502_reg_get(&emu, REG_6502_PC), 1);

    etb_emu_6502_hook_s(&emu, &dum_s);
    assert_int_equal(etb_emu_6502_reg_get(&emu, REG_6502_S), 2);

    etb_emu_6502_hook_a(&emu, &dum_a);
    assert_int_equal(etb_emu_6502_reg_get(&emu, REG_6502_A), 3);

    etb_emu_6502_hook_x(&emu, &dum_x);
    assert_int_equal(etb_emu_6502_reg_get(&emu, REG_6502_X), 4);

    etb_emu_6502_hook_y(&emu, &dum_y);
    assert_int_equal(etb_emu_6502_reg_get(&emu, REG_6502_Y), 5);

    etb_emu_6502_hook_p(&emu, &dum_p);
    assert_int_equal(etb_emu_6502_reg_get(&emu, REG_6502_P), 6);

    etb_emu_6502_hook_ram(&emu, dum_ram);

    assert_int_equal(etb_emu_6502_ram_get(&emu, 0), 1);
    assert_int_equal(etb_emu_6502_ram_get(&emu, 1), 2);
    assert_int_equal(etb_emu_6502_ram_get(&emu, 2), 3);

    etb_emu_6502_reg_set(&emu, REG_6502_PC, 101);
    etb_emu_6502_reg_set(&emu, REG_6502_S, 102);
    etb_emu_6502_reg_set(&emu, REG_6502_A, 103);
    etb_emu_6502_reg_set(&emu, REG_6502_X, 104);
    etb_emu_6502_reg_set(&emu, REG_6502_Y, 105);
    etb_emu_6502_reg_set(&emu, REG_6502_P, 106);

    etb_emu_6502_ram_set(&emu, 0, 101);
    etb_emu_6502_ram_set(&emu, 1, 102);
    etb_emu_6502_ram_set(&emu, 2, 103);

    assert_int_equal(etb_emu_6502_reg_get(&emu, REG_6502_PC), 101);
    assert_int_equal(etb_emu_6502_reg_get(&emu, REG_6502_S), 102);
    assert_int_equal(etb_emu_6502_reg_get(&emu, REG_6502_A), 103);
    assert_int_equal(etb_emu_6502_reg_get(&emu, REG_6502_X), 104);
    assert_int_equal(etb_emu_6502_reg_get(&emu, REG_6502_Y), 105);
    assert_int_equal(etb_emu_6502_reg_get(&emu, REG_6502_P), 106);

    assert_int_equal(etb_emu_6502_ram_get(&emu, 0), 101);
    assert_int_equal(etb_emu_6502_ram_get(&emu, 1), 102);
    assert_int_equal(etb_emu_6502_ram_get(&emu, 2), 103);
}


static void t_step(void **state){
    (void) state;

    etb_emu_6502 emu;

    etb_emu_6502_hook_pc(&emu, &dum_pc);
    assert_int_equal(etb_emu_6502_reg_get(&emu, REG_6502_PC), 101);

    etb_emu_6502_hook_step(&emu, etb_emu_6502_step);
    etb_emu_6502_step();
    assert_int_equal(etb_emu_6502_reg_get(&emu, REG_6502_PC), 12);
}

static void t_run_6502_tests(void **state){
    (void) state;

    char dir_path[] = "/home/rcd/proj/65x02/6502/v1/";

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
        int ram[UINT16_MAX] = {0};
        etb_emu_6502 emu;

        etb_emu_6502_hook_pc(&emu, &dum_pc);
        etb_emu_6502_hook_s(&emu, &dum_s);
        etb_emu_6502_hook_a(&emu, &dum_a);
        etb_emu_6502_hook_x(&emu, &dum_x);
        etb_emu_6502_hook_y(&emu, &dum_y);
        etb_emu_6502_hook_p(&emu, &dum_p);
        etb_emu_6502_hook_ram(&emu, dum_ram);
        etb_emu_6502_hook_step(&emu, etb_emu_6502_step);

        char t_path[strlen(dir_path)];
        strcpy(t_path, dir_path);

        strcat(t_path, d->d_name);

        etb_test_status status = etb_run_6502_test(&emu, t_path, 1);

        assert_int_equal(status, TEST_FAILED);
    }

    closedir(test_dir);
}

int main(void){
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(null_case),
        cmocka_unit_test(t_hooks_get_set),
        cmocka_unit_test(t_step),
        cmocka_unit_test(t_run_6502_tests)
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
