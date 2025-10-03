#include <stddef.h>
#include <stdbool.h>
#ifndef JSON_H
#define JSON_H

typedef enum json_type_t { json_string, json_number, json_array, json_object, json_boolean, json_null } json_type_t;

typedef struct json_object_t json_object_t;
typedef struct json_value_t json_value_t;
typedef struct json_array_t json_array_t;

/**
 * JSON Array structure
 */
typedef struct json_array_t {
    json_value_t* values;
    size_t len;
} json_array_t;

/**
 * JSON Value structure
 */
typedef struct json_value_t {
    json_type_t type;
    union {
        char* string;
        long number;
        json_array_t array;
        json_object_t* object;
        bool boolean;
    } value;
} json_value_t;

/**
 * JSON Object structure
 */
typedef struct json_object_t {
    char** keys;
    json_value_t* values;
    size_t len;
} json_object_t;

/**
 * Parse a string containing JSON data
 */
json_object_t* json_parse_string(const char* data);

/*
void json_deinit_data();
json_value* json_get_value(char* target_key);
bool json_parse_data(char* json_data);
char* json_create_data(char** keys_l, json_value* values_array, size_t amt);
*/

#endif