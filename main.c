#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef enum value_type { string, number } value_type;

typedef struct json_value {
    value_type type;
    union {
        char* string;
        long number;
    } value;
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
    json_value* json_values = NULL;
    size_t values_len = 0;

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

            if(*value = '"') {
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
                printf("getting number!\n");
                long number_value = strtol(number_ptr, &value, 10);
                printf("got number: %ld\n", number_value);
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

    if(target_key == NULL) {
        for(size_t i = 0; i < keys_len; i++) {
            printf("\"%s\": ", keys[i]);
            switch(json_values[i].type) {
                case string: {
                    printf("\"%s\"\n", json_values[i].value.string);
                    break;
                }
                case number: {
                    printf("%ld\n", json_values[i].value.number);
                    break;
                }
            }
        }
    } else {
        size_t i;
        for(i = 0; i < keys_len; i++) {
            if(!strcmp(target_key, keys[i])) {
                printf("\"%s\": ", keys[i]);
                switch(json_values[i].type) {
                    case string: {
                        printf("\"%s\"\n", json_values[i].value.string);
                        break;
                    }
                    case number: {
                        printf("%ld\n", json_values[i].value.number);
                        break;
                    }
                }
                break;
            }
        }
        if(i == keys_len) {
            fprintf(stderr, "Unable to find key \"%s\"!\n", target_key);
        }
    }

    for(size_t i = 0; i < keys_len; i++) {
        free(keys[i]);
    }
    free(keys);

    for(size_t i = 0; i < values_len; i++) {
        if(json_values[i].type == string) {
            free(json_values[i].value.string);
        }
    }
    free(json_values);

    return 0;
}