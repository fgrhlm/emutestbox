#include "inc/loader.h"

#include <stdlib.h>
#include <stdio.h>
#include <jansson.h>

void is_array(json_t *jt){
    if(!json_is_array(jt)){
        printf("not an array!\n");
        json_decref(jt);
        exit(1);
    }
}

void is_obj(json_t *jt){
    if(!json_is_object(jt)){
        printf("not an obj!\n");
        json_decref(jt);
        exit(1);
    }
}

unsigned long get_file_size(FILE *fp){
    unsigned long len;

    // Get file size
    fseek(fp, 0, SEEK_END);
    len = ftell(fp);
    rewind(fp);

    return len;
}

char *read_json_file(char *fn){
    FILE *fp;

    fp = fopen(fn, "r");

    if(fp == NULL){
        printf("Could not open: %s\n", fn);
        exit(-1);
    }

    unsigned long file_len = get_file_size(fp);

    char *buffer = malloc(file_len);

    fread(buffer, sizeof(char), file_len, fp);
    fclose(fp);

    return buffer;
}


etb_state *read_state(json_t* cur_a){
    json_t *cur_x, *cur_y, *cur_z;
    etb_state *state = malloc(sizeof(etb_state));

    cur_x = json_object_get(cur_a, "initial");
    cur_y = json_object_get(cur_x, "pc");

    state->pc = json_integer_value(cur_y);

    cur_y = json_object_get(cur_x, "s");
    state->s = json_integer_value(cur_y);

    cur_y = json_object_get(cur_x, "a");
    state->a = json_integer_value(cur_y);

    cur_y = json_object_get(cur_x, "x");
    state->x = json_integer_value(cur_y);

    cur_y = json_object_get(cur_x, "y");
    state->y = json_integer_value(cur_y);

    cur_y = json_object_get(cur_x, "p");
    state->p = json_integer_value(cur_y);

    cur_y = json_object_get(cur_x, "ram");
    unsigned long init_ram_len = json_array_size(cur_y);

    state->mem_len = init_ram_len;
    state->mem = malloc(sizeof(etb_mem)*init_ram_len);

    for(int j=0; j<init_ram_len; j++){
        json_t *r = json_array_get(cur_y, j);

        json_t *jaddr = json_array_get(r, 0);
        json_t *jval = json_array_get(r, 1);

        uint16_t addr = (uint16_t)json_integer_value(jaddr);
        uint8_t val = (uint16_t)json_integer_value(jval);

        state->mem[j].addr = addr;
        state->mem[j].val = val;
    }

    return state;
}

void etb_free_state(etb_state *st){
    free(st->mem);
    free(st);
}

void etb_free_test(etb_test *t){
    etb_free_state(t->initial);
    etb_free_state(t->final);
}

void etb_print_state(etb_state st){
    printf(
        "pc:%d | s:%d | a:%d | x:%d | y:%d | p:%d",
        st.pc, st.s, st.a, st.x, st.y, st.p
    );

    printf("\nmem:\n");
    for(int i=0; i<st.mem_len; i++){
        printf("\t%d -> %d\n", st.mem[i].addr, st.mem[i].val);
    }
}

etb_test *load_6502_test(char *path){
    json_error_t err;
    json_t *root, *cur_x, *data;

    char *buffer = read_json_file(path);
    root = json_loads(buffer, (JSON_DISABLE_EOF_CHECK), &err);

    if(root == NULL){
        printf("Could not parse json: %s\nError:%s\n", path, err.text);
        exit(-1);
    }

    is_array(root);

    free(buffer);

    unsigned long root_len = json_array_size(root);

    etb_test *tests = malloc(sizeof(etb_test) * root_len);

    for(int i=0; i<root_len; i++){
        data = json_array_get(root, i);
        is_obj(data);

        cur_x = json_object_get(data, "name");

        tests[i].name = json_string_value(cur_x);
        tests[i].initial = read_state(data);
        tests[i].final = read_state(data);
        tests[i].len = root_len;

        //printf("== INITIAL ==\n");
        //etb_print_state(*tests[i].initial);
        //printf("== FINAL ==\n");
        //etb_print_state(*tests[i].final);
    }

    json_decref(root);
    return tests;
}
// Prepares an etb_loader struct for multithreaded use.
void etb_init_loader_threaded(etb_loader *loader, int num_threads){
    loader->num_threads = num_threads;
}

// Wrapper around loader_threaded but with num_threads = 1.
void etb_init_loader(etb_loader *loader){ etb_init_loader_threaded(loader, 1); }

// Destroys the loader.
void etb_destroy_loader(etb_loader *loader){ return; }
