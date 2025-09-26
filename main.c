#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char** argv) {
    if(argc != 3) {
        fprintf(stderr, "Usage: %s [key to find] [JSON data]\n", argv[0]);
        return 1;
    }

    char* json_data = argv[2];
    size_t json_len = strlen(json_data);
    if(*json_data != '{') {
        fprintf(stderr, "Invalid JSON object!\n");
        return 1;
    }
    if(json_data[json_len - 1] != '}') {
        fprintf(stderr, "Invalid JSON object!\n");
        return 1;
    }

    memcpy(json_data, json_data + 1, --json_len);
    json_data[json_len] = '\0';

    //
}