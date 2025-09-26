#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef union json_value {
    char* string;
    signed long int number;
} json_value;

int main(int argc, char** argv) {
    if(argc < 2) {
        fprintf(stderr, "Usage: %s [JSON data] [key to find]\n", argv[0]);
        return 1;
    }

    char* target_key = ((argc == 3)?argv[2]:NULL);

    char* json_data = argv[1];
    size_t json_len = strlen(json_data);
    if(*json_data != '{') {
        fprintf(stderr, "Invalid JSON object!\n");
        return 1;
    }
    if(json_data[json_len - 1] != '}') {
        fprintf(stderr, "Invalid JSON object!\n");
        return 1;
    }

    memcpy(json_data, json_data + 1, json_len--);
    json_data[json_len-- - 1] = '\0';

    if(*json_data == '\0') {
        printf("Empty JSON object!\n");
        return 0;
    }

    char** keys = NULL;
    size_t keys_len = 0;
    //

    while(1) { // TODO: Handle whitespace
        char* key = strchr(json_data, '"');
        if(key++ == NULL) { // TODO: properly handle this
            fprintf(stderr, "No keys but not empty!\n");
        }
        char* key_start = key;
        size_t key_len = 0;
        
        keys = (char**)realloc(keys, (++keys_len) * sizeof(char*));
        keys[keys_len - 1] = NULL;

        while(1) {
            if(*key != '"') {
                keys[keys_len - 1] = (char*)realloc((keys[keys_len - 1]), (++key_len) + 1);
                if(*key == '\\') {
                    switch(*(++key)) { // Made this a switch statement so I can easily add more values later on
                        case '\\':
                        case '"':
                        {
                            keys[keys_len - 1][key_len - 1] = *(key++);
                            break;
                        }
                    }
                } else {
                    keys[keys_len - 1][key_len - 1] = *(key++);
                }
            } else {
                keys[keys_len - 1][key_len] = '\0';
                key++;
                break;
            }
        }

        // *key should be ':'
    }
}