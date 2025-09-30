#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include "json.h"

char** keys = NULL;
size_t keys_len = 0;
json_value* json_values = NULL;
size_t values_len = 0;

void deinit_parsed_data() {
    if(keys != NULL) {
        for(size_t i = 0; i < keys_len; i++) {
            free((keys[i]));
        }
        free(keys);
        keys = NULL;
    }
    if(json_values != NULL) {
        for(size_t i = 0; i < values_len; i++) {
            if(json_values[i].type == string) {
                free(json_values[i].value.string);
            }
        }
        free(json_values);
        json_values = NULL;
    }
    keys_len = 0;
    values_len = 0;
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
    fprintf(stderr, "Failed to find key!\n");
    response = (json_value){0};
    response.error = true;
    return &response;
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

    json_data++;
    json_len--;
    json_data[json_len - 1] = '\0';

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

char* create_json_data(char** keys_l, json_value* values_array, size_t amt) {
    char* json_data = (char*)calloc(1, 4);
    size_t json_data_len = 3;
    char* json_ptr = json_data + 1;
    size_t json_ptr_dis = 1;
    for(size_t i = 0; i < amt; i++) {
        json_data_len += 3;
        json_ptr_dis = json_ptr - json_data;
        json_data = (char*)realloc(json_data, json_data_len + 1);
        json_ptr = json_data + json_ptr_dis;
        *json_ptr++ = '"';
        size_t key_len = strlen(keys_l[i]);
        for(size_t j = 0; j < key_len; j++) {
            char ch = keys_l[i][j];
            switch(ch) {
                case '"':
                case '\\': {
                    json_data_len += 2;
                    json_ptr_dis = json_ptr - json_data;
                    json_data = (char*)realloc(json_data, json_data_len + 1);
                    json_ptr = json_data + json_ptr_dis;
                    *json_ptr++ = '\\';
                    *json_ptr++ = ch;
                    break;
                }
                default: {
                    json_ptr_dis = json_ptr - json_data;
                    json_data = (char*)realloc(json_data, (++json_data_len) + 1);
                    json_ptr = json_data + json_ptr_dis;
                    *json_ptr++ = ch;
                    break;
                }
            }
        }
        *json_ptr++ = '"';
        *json_ptr++ = ':';
        switch(values_array[i].type) {
            case string: {
                json_data_len += 2;
                json_ptr_dis = json_ptr - json_data;
                json_data = (char*)realloc(json_data, json_data_len + 1);
                json_ptr = json_data + json_ptr_dis;
                *json_ptr++ = '"';
                size_t string_value_len = strlen(values_array[i].value.string);
                for(size_t j = 0; j < string_value_len; j++) {
                    char ch = values_array[i].value.string[j];
                    switch(ch) {
                        case '"':
                        case '\\': {
                            json_data_len += 2;
                            json_ptr_dis = json_ptr - json_data;
                            json_data = (char*)realloc(json_data, json_data_len + 1);
                            json_ptr = json_data + json_ptr_dis;
                            *json_ptr++ = '\\';
                            *json_ptr++ = ch;
                            break;
                        }
                        default: {
                            json_ptr_dis = json_ptr - json_data;
                            json_data = (char*)realloc(json_data, (++json_data_len) + 1);
                            json_ptr = json_data + json_ptr_dis;
                            *json_ptr++ = ch;
                            break;
                        }
                    }
                }
                *json_ptr++ = '"';
                /*
                if(i != (amt - 1)) {
                    json_ptr_dis = json_ptr - json_data;
                    json_data = (char*)realloc(json_data, (++json_data_len) + 1);
                    json_ptr = json_data + json_ptr_dis;
                    *json_ptr++ = ',';
                }
                */
                break;
            }
            case number: {
                char* number_string = NULL;
                size_t number_string_len = 0;
                size_t number_len = 1;
                char* number_ptr = number_string;
                unsigned long number_to_write;
                unsigned long number_copy;
                if(values_array[i].value.number < 0) {
                    char* number_string = (char*)realloc(number_string, ++number_string_len);
                    *number_string = '-';
                    number_ptr = number_string + 1;
                    number_to_write = (unsigned long)(-(values_array[i].value.number));
                } else {
                    number_to_write = (unsigned long)(values_array[i].value.number);
                }
                number_copy = number_to_write;
                while(number_copy > 9) {
                    number_copy /= 10;
                    number_len++;
                }
                number_string_len += number_len;
                number_string = (char*)realloc(number_string, number_string_len);
                number_ptr = number_string + (number_string_len - number_len);
                for(size_t j = (number_len - 1); j >= 0; j--) {
                    uint8_t digit = (number_to_write % 10) + 48;
                    number_ptr[j] = (char)digit;
                    if(j == 0) break;
                    number_to_write /= 10;
                }

                json_data_len += number_string_len;
                json_ptr_dis = json_ptr - json_data;
                json_data = (char*)realloc(json_data, number_string_len + 1);
                json_ptr = json_data + json_ptr_dis;
                memcpy(json_ptr, number_string, number_len);
                json_ptr += number_len;
                free(number_string);
                break;
            }
        }
        if(i != (amt - 1)) {
            json_ptr_dis = json_ptr - json_data;
            json_data = (char*)realloc(json_data, (++json_data_len) + 1);
            json_ptr = json_data + json_ptr_dis;
            *json_ptr++ = ',';
        }
    }
    *json_ptr++ = '}';
    *json_data = '{';
    *json_ptr = '\0';
    return json_data;
}