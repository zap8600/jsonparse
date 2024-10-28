#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef enum { STRING, NUM, ARRAY } vtypei;

typedef struct {
    char* key;
    vtypei valuetype;
    int arraycount;
    union vtype {
        int num;
        char* string;
        JSON* array;
    } value;
} JSON;

JSON jsonfilecon;
int arraydepth;

int main(int argc, char** argv) {
    if(argc != 2) {
        fprintf(stderr, "Usage: %s [JSON file]\n", argv[0]);
        return -1;
    }
    FILE* jsonfile = fopen(argv[1], "rb");
    int ch = 0;
    while((ch = getc(jsonfile)) != EOF) {
        switch(ch) {
            case '{':
            {
                if(jsonfilecon.arraycount = 0) {
                    //
                }
            }
        }
    }
}
