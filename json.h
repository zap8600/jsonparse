#include <stdbool.h>
#ifndef JSON_H
#define JSON_H

typedef enum value_type { string, number } value_type;

typedef struct json_value {
    value_type type;
    union {
        char* string;
        long number;
    } value;
    bool error;
} json_value;

void deinit_parsed_data();
json_value* get_json_value(char* target_key);
bool parse_json_data(char* json_data);
char* create_json_data(char** keys_l, json_value* values_array, size_t amt);

#endif