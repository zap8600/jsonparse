#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

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
}