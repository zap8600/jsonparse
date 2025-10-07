#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "json.h"

// Challenge: Don't modify the user's JSON data string.
// Challenge: Single pass only. None of these parsing functions should ever need to go back;

static char* format_string(char** string_ptr) {
    char* string = *string_ptr;
    char* formatted_string = NULL;
    size_t formatted_string_len = 0;
    while(1) {
        if(*string != '"') {
            formatted_string = (char*)realloc(format_string, (++formatted_string_len) + 1);
            if(*string == '\\') {
                // Escape characters
                switch(*(++string)) {
                    case '\\':
                    case '"':
                    {
                        formatted_string[formatted_string_len - 1] = *string++;
                        break;
                    }
                }
            } else {
                formatted_string[formatted_string_len - 1] = *string++;
            }
        } else {
            formatted_string[formatted_string_len] = '\0';
            string++;
            break;
        }
    }
    *string_ptr = string;
    return formatted_string;
}

static json_array_t* parse_array(const char* data);
static json_object_t* parse_object(const char* data);

static void get_values(json_value_t** values, size_t len, char** data) {
    char* value = *data;
    while(1) {
        if(isspace(*value)) {
            value++;
            continue;
        }
        if(*value == '-' || isdigit(*value)) {
            // Number
        } else {
            switch(*value) {
                case '"': {
                    // String
                    value++;
                    const char* formatted_string = format_string(&value);
                    *values = (json_value_t*)realloc(*values, len * sizeof(json_value_t));
                    (*values)[len - 1].type = json_string;
                    (*values)[len - 1].value.string = formatted_string;
                    break;
                }
                case '[': {
                    // Array
                    break;
                }
                case '{': {
                    // Object
                    break;
                }
                case 't': {
                    // True
                    break;
                }
                case 'f': {
                    // False
                    break;
                }
                case 'n': {
                    // Null
                    break;
                }
            }
        }
    }
}

static json_array_t* parse_array(const char* data) {
    char* value = data;
    json_array_t* result_array = (json_array_t*)calloc(1, sizeof(json_array_t));
    while(1) {
        //get_values();
    }
}

static json_object_t* parse_object(const char* data) {
    // data cannot be NULL, that check was done by the main function
    json_object_t* result_object = (json_object_t*)calloc(1, sizeof(json_object_t));
    while(1) {
        char* key = strchr(data, '"') + 1;
        const char* formatted_key = format_string(&key);
        result_object->keys = (char**)realloc(result_object->keys, (++(result_object->len)) * sizeof(char*));
        result_object->keys[result_object->len] = formatted_key;
        char* value = strchr(key, ':') + 1;
        get_values(&(result_object->values), &data);
    }
}

json_object_t* json_parse_string(const char* data) {
    json_object_t* result_object = NULL;
    if(data == NULL) { // We actually need data to parse
        return NULL;
    }
    // Assume first character of data is a '{'
}
