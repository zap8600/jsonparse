#include <stdbool.h>
#ifndef JSON_H
#define JSON_H

typedef enum json_type_t { json_string, json_number, json_array, json_object } json_type_t;
typedef enum json_error_t { json_ok } json_error_t;

typedef struct json_object_t {
    char** keys;
    size_t keys_len;
    json_value_t* values;
    size_t values_len;
} json_object_t;

typedef struct json_value_t {
    json_type_t type;
    union {
        char* string;
        long number;
        struct {
            json_value_t* values;
            size_t len;
        } array;
    } value;
    json_error_t error;
} json_value_t;

void json_deinit_data();
json_value* json_get_value(char* target_key);
bool json_parse_data(char* json_data);
char* json_create_data(char** keys_l, json_value* values_array, size_t amt);

#endif