#ifndef LOADER_H
#define LOADER_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <jansson.h>

typedef enum { READ, WRITE } etb_6502_cycle_type;

typedef struct {
    uint16_t addr;
    uint8_t val;
} etb_mem;

typedef struct {
    uint16_t pc;
    uint8_t s,a,x,y,p;
    etb_mem *mem;
    unsigned long mem_len;
} etb_state;

typedef struct {
    const char *name;
    unsigned long len;
    etb_state *initial;
    etb_state *final;
} etb_test;

typedef struct {
    uint16_t addr;
    uint8_t val;
    etb_6502_cycle_type cycle_type;
} etb_6502_cycle;

typedef struct {
    int num_threads;
} etb_loader;

void etb_init_loader_threaded(etb_loader *loader, int num_threads);
void etb_init_loader(etb_loader *loader);
void etb_destroy_loader(etb_loader *loader);
void is_array(json_t *jt);
void is_obj(json_t *jt);
unsigned long get_file_size(FILE *fp);
char *read_json_file(char *fn);
etb_state *read_state(json_t* cur_a);
void etb_free_state(etb_state *st);
void etb_free_test(etb_test *t);
void etb_print_state(etb_state st);
etb_test *load_6502_test(char *path);

#endif
