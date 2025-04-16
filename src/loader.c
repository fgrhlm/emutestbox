
#include "inc/loader.h"

#include <stdlib.h>
#include <stdio.h>
#include <jansson.h>

// Prepares an etb_loader struct for multithreaded use.
void etb_init_loader_threaded(etb_loader *loader, int num_threads){
    loader->num_threads = num_threads;
}

// Wrapper around loader_threaded but with num_threads = 1.
void etb_init_loader(etb_loader *loader){ etb_init_loader_threaded(loader, 1); }

// Destroys the loader.
void etb_destroy_loader(etb_loader *loader){ return; }

// Load a single SingleStepTest json file.
void etb_load_6502_json(etb_loader *t_loader, etb_test *t_test, char *t_path){
    FILE *fp;
    json_t *json_root;
    json_error_t json_err;

    // Open json file
    fp = fopen(t_path, "r");

    if(fp == NULL){
        printf("Unable to open %s!\n", t_path);
        exit(-1);
    }

    // Get file size
    fseek(fp, 0, SEEK_END);
    long file_len = ftell(fp);
    rewind(fp);

    // Read file
    char *buffer = malloc(sizeof(char) * file_len);

    fread(buffer, sizeof(char), file_len, fp);
    fclose(fp);

    // Parse json
    json_root = json_loads(buffer, 0, &json_err);
    free(buffer);

    if(json_is_null(json_root)){
        printf("Could not read json!\n");
        exit(-1);
    }

    // Prepare space for tests
    int json_root_len = json_array_size(json_root);

    for(int i=0; i<json_root_len; i++){
        json_t *t_name, *t_initial, *t_final, *t_cycles, *t_initial_mem, *t_final_mem;
        json_t *data = json_array_get(json_root, i);

        if(!json_is_object(data)){
            printf("Index: %d Error: Not an object :(\n", i);
            json_decref(json_root);
            exit(-1);
        }

        // Parse -> Test name
        t_name = json_object_get(data, "name");
        if(!json_is_string(t_name)){ json_decref(json_root); printf("name is not a string!\n"); exit(-1); }
        t_test[i].name = (char*)json_string_value(t_name);

        // Parse -> Initial state
        t_initial = json_object_get(data, "initial");
        if(!json_is_object(t_initial)){ json_decref(json_root); printf("initial is not an object!\n"); exit(-1); }

        t_test[i].initial.reg_pc = json_integer_value(json_object_get(t_initial, "pc"));
        t_test[i].initial.reg_s = json_integer_value(json_object_get(t_initial, "s"));
        t_test[i].initial.reg_a = json_integer_value(json_object_get(t_initial, "a"));
        t_test[i].initial.reg_x = json_integer_value(json_object_get(t_initial, "x"));
        t_test[i].initial.reg_y = json_integer_value(json_object_get(t_initial, "y"));
        t_test[i].initial.reg_p = json_integer_value(json_object_get(t_initial, "p"));

        // Load ram state
        t_initial_mem = json_object_get(t_initial, "ram");
        if(!json_is_array(t_initial_mem)){ json_decref(json_root); printf("initial ram is not an array!\n"); exit(-1); }

        int t_initial_mem_len = json_array_size(t_initial_mem);
        t_test[i].initial.mem = malloc(sizeof(etb_mem) * t_initial_mem_len);

        for(int j=0; j<t_initial_mem_len; j++){
            json_t* entry = json_array_get(t_initial_mem, j);

            uint16_t addr = (uint16_t)json_integer_value(json_array_get(entry, 0));
            uint8_t val = (uint8_t)json_integer_value(json_array_get(entry, 1));

            t_test[i].initial.mem[j].addr = addr;
            t_test[i].initial.mem[j].val = val;
        }

        // Parse -> Final state
        t_final = json_object_get(data, "final");
        if(!json_is_object(t_final)){ json_decref(json_root); printf("final is not an object!\n"); exit(-1); }

        t_test[i].final.reg_pc = json_integer_value(json_object_get(t_final, "pc"));
        t_test[i].final.reg_s = json_integer_value(json_object_get(t_final, "s"));
        t_test[i].final.reg_a = json_integer_value(json_object_get(t_final, "a"));
        t_test[i].final.reg_x = json_integer_value(json_object_get(t_final, "x"));
        t_test[i].final.reg_y = json_integer_value(json_object_get(t_final, "y"));
        t_test[i].final.reg_p = json_integer_value(json_object_get(t_final, "p"));

        // Load ram state
        t_initial_mem = json_object_get(t_initial, "ram");
        if(!json_is_array(t_initial_mem)){ json_decref(json_root); printf("final ram is not an array!\n"); exit(-1); }

        int t_final_mem_len = json_array_size(t_initial_mem);
        t_test[i].final.mem = malloc(sizeof(etb_mem) * t_initial_mem_len);

        for(int j=0; j<t_final_mem_len; j++){
            json_t* entry = json_array_get(t_final_mem, j);

            uint16_t addr = (uint16_t)json_integer_value(json_array_get(entry, 0));
            uint8_t val = (uint8_t)json_integer_value(json_array_get(entry, 1));

            t_test[i].final.mem[j].addr = addr;
            t_test[i].final.mem[j].val = val;
        }

        // Parse -> Cycles
        t_cycles = json_object_get(data, "cycles");
        if(!json_is_array(t_cycles)){ json_decref(json_root); printf("cycles is not an array!\n"); exit(-1); }
    }
}

// Destroy 6502 test.
void etb_destroy_6502_test(etb_test *t){
    free(t->cycles);
    free(t);
}
