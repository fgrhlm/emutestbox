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
    uint8_t reg_pc;
    uint8_t reg_s;
    uint8_t reg_a;
    uint8_t reg_x;
    uint8_t reg_y;
    uint8_t reg_p;
    etb_mem *mem;
} etb_6502_state;

typedef struct {
    uint16_t addr;
    uint8_t val;
    etb_6502_cycle_type cycle_type;
} etb_6502_cycle;

typedef struct {
    char* file;
    int index;
    char* name;
    etb_6502_state begin_state;
    etb_6502_state end_state;
    etb_6502_cycle *cycles;
} etb_test;

typedef struct {
    int num_threads;
    FILE **files;
    json_t **json_root;
    json_error_t **json_err;
} etb_loader;

void init_etb_loader_threaded(etb_loader *loader, int num_threads);
void init_etb_loader(etb_loader *loader);

void etb_destroy_loader(etb_loader *loader);

etb_test *etb_load_6502_json(char *t_path);
void etb_destroy_6502_json(etb_test *t);

#endif
