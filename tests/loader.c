#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <setjmp.h>
#include <cmocka.h>

#include "loader.h"

#define T_JSON_TEST_FILE "test.json"

// Based on examples from here:  https://api.cmocka.org

// Null case
static void null_case(void **state){
    (void) state;
}

static void t_etb_init_loader(void **state){
    (void) state;

    const int NUM_THREADS = 1;

    etb_loader loader;
    etb_init_loader(&loader);

    assert_int_equal(loader.num_threads, NUM_THREADS);

    etb_destroy_loader(&loader);
}

static void t_etb_init_loader_threaded(void **state){
    (void) state;

    const int NUM_THREADS = 4;

    etb_loader loader;
    etb_init_loader_threaded(&loader, 4);

    assert_int_equal(loader.num_threads, NUM_THREADS);

    etb_destroy_loader(&loader);
}

static void t_etb_destroy_loader(void **state){
    (void) state;

    etb_loader loader;

    etb_init_loader(&loader);
    //etb_init_loader_threaded(&loader, 4);

    etb_destroy_loader(&loader);

    assert_true(1);
}

static void t_etb_load_6502_test(void **state){
    (void) state;

    const char* NAME_STRING = "JSON_TEST_FILE";

    etb_loader loader;
    etb_test t_test[10000]; // All 6502 singlesteptests are this long

    etb_init_loader(&loader);
    etb_load_6502_json(&loader, t_test, "./build/test.json");

    assert_string_equal(NAME_STRING, t_test[0].name);
    assert_string_equal(NAME_STRING, t_test[1].name);

    etb_destroy_loader(&loader);
}

int main(void){
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(null_case),
        cmocka_unit_test(t_etb_init_loader),
        cmocka_unit_test(t_etb_init_loader_threaded),
        cmocka_unit_test(t_etb_destroy_loader),
        cmocka_unit_test(t_etb_load_6502_test)
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
