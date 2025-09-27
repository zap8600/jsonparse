#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include "json.h"

static char** keys = NULL;
static size_t keys_len = 0;
static json_value* json_values = NULL;
static size_t values_len = 0;

void deinit_parsed_data() {
    if(keys != NULL) {
        for(size_t i = 0; i < keys_len; i++) {
            free(keys[i]);
        }
        free(keys);
    }
    if(json_values != NULL) {
        for(size_t i = 0; i < values_len; i++) {
            if(json_values[i].type == string) {
                free(json_values[i].value.string);
            }
        }
        free(json_values);
    }
}

json_value* get_json_value(char* target_key) {
    static json_value response = {0};
    if(target_key == NULL) {
        fprintf(stderr, "Need a target key!\n");
        response = (json_value){0};
        response.error = true;
        return &response;
    }

    size_t i;
    for(i = 0; i < keys_len; i++) {
        if(!strcmp(target_key, (keys[i]))) {
            return &(json_values[i]);
            break;
        }
    }
    if(i == keys_len) {
        fprintf(stderr, "Failed to find key!\n");
        response = (json_value){0};
        response.error = true;
        return &response;
    }
}

bool parse_json_data(char* json_data) {
    if((keys != NULL) || (json_values != NULL)) {
        deinit_parsed_data();
    }

    size_t json_len = strlen(json_data);
    if(*json_data != '{') {
        fprintf(stderr, "Invalid JSON object!\n");
        return false;
    }
    if(json_data[json_len - 1] != '}') {
        fprintf(stderr, "Invalid JSON object!\n");
        return false;
    }

    memcpy(json_data, json_data + 1, json_len--);
    json_data[json_len-- - 1] = '\0';

    if(*json_data == '\0') {
        printf("Empty JSON object!\n");
        return false;
    }

    char* current_point = json_data;

    while(1) { // TODO: Handle whitespace

        char* key = strchr(current_point, '"');
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

        // TODO: Make this check better
        char* value = strchr(key, ':');
        if(value++ == NULL) { // TODO: properly handle this
            fprintf(stderr, "No value indicator!\n");
        }

        while(1) {
            if(isspace(*value)) {
                value++;
                continue;
            }

            value_type type;

            if(*value == '"') {
                type = string;
                char* string_value = NULL;
                size_t string_len = 0;
                char* string_ptr = value + 1;
                while(1) {
                    if(*string_ptr != '"') {
                        string_value = (char*)realloc(string_value, (++string_len) + 1);
                        if(*string_ptr == '\\') {
                            switch(*(++string_ptr)) { // Made this a switch statement so I can easily add more values later on
                                case '\\':
                                case '"':
                                {
                                    string_value[string_len - 1] = *(string_ptr++);
                                    break;
                                }
                            }
                        } else {
                            string_value[string_len - 1] = *(string_ptr++);
                        }
                    } else {
                        string_value[string_len] = '\0';
                        value = string_ptr + 1;
                        break;
                    }
                }
                json_values = (json_value*)realloc(json_values, (++values_len) * sizeof(json_value));
                json_value new_value = {0};
                new_value.type = type;
                new_value.value.string = string_value;
                json_values[values_len - 1] = new_value;
            } else if((*value == '-') || isdigit(*value)) {
                type = number;
                char* number_ptr = value;
                long number_value = strtol(number_ptr, &value, 10);
                json_values = (json_value*)realloc(json_values, (++values_len) * sizeof(json_value));
                json_value new_value = {0};
                new_value.type = type;
                new_value.value.number = number_value;
                json_values[values_len - 1] = new_value;
            }
            break;
        }

        char* is_more = strchr(value, ',');
        if(!is_more) {
            break;
        }
        current_point = is_more + 1;
    }
    /*
    size_t i;
    for(i = 0; i < keys_len; i++) {
        if(!strcmp(target_key, keys[i])) {
            response = json_values[i];
        }
    }
    if(i == keys_len) {
        fprintf(stderr, "Unable to find key \"%s\"!\n", target_key);
        response = (json_value){0};
        response.error = true;
    }
    */

    /*
    for(i = 0; i < keys_len; i++) {
        free(keys[i]);
    }
    free(keys);
    for(i = 0; i < values_len; i++) {
        if(json_values[i].type == string) {
            free(json_values[i].value.string);
        }
    }
    */

    return true;
}