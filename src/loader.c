#include "inc/loader.h"

#include <stdlib.h>
#include <stdio.h>
#include <jansson.h>

// Prepares an etb_loader struct for multithreaded use.
void init_etb_loader_threaded(etb_loader *loader, int num_threads){
    loader->num_threads = num_threads;
    loader->files = malloc(sizeof(FILE*) * num_threads);
    loader->json_root = malloc(sizeof(json_t) * num_threads);
    loader->json_err = malloc(sizeof(json_error_t) * num_threads);
}

// Wrapper around loader_threaded but with num_threads = 1.
void init_etb_loader(etb_loader *loader){ init_etb_loader_threaded(loader, 1); }

// Destroys the loader.
void etb_destroy_loader(etb_loader *loader){
    free(loader->files);
    free(loader->json_root);
    free(loader->json_err);
}

// Load a single SingleStepTest json file.
etb_test *etb_load_6502_test(char *t_path, FILE *fp, json_t *root, json_error_t *err){
    // Prepare space for tests
    etb_test *t = malloc(sizeof(etb_test));

    // Open json file
    fp = fopen(t_path, "r");

    if(fp == NULL){
        printf("Unable to open %s!\n", t_path);
        exit(-1);
    }

    // Get file size
    fseek(fp, 0, SEEK_END);
    size_t file_len = ftell(fp);
    rewind(fp);

    // Read file
    char *buffer = malloc(sizeof(char) * file_len);

    fgets(buffer, file_len, fp);

    // Parse json
    root = json_loads(buffer, 0, err);
    free(buffer);

    if(!root){
        printf("Could not read json! [Line %d]: %s\n", err->line, err->text);
        exit(-1);
    }

    if(!json_is_array(root)){
        printf("Root is not an array!\n");
        exit(-1);
    }

    for(int i=0; i<json_array_size(root); i++){
        json_t *data = json_array_get(root, i);

        if(!json_is_object(data)){
            printf("Index: %d Error: Not an object :(\n", i);
            exit(-1);
        }
    }
}

// Destroy 6502 test.
void etb_destroy_6502_test(etb_test *t){
    free(t->cycles);
    free(t);
}
