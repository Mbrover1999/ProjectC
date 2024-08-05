#include <stdio.h>

int main(void) {
    printf("Hello, World!\n");
    return 0;
}
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Assuming previous definitions and functions are available */
typedef struct code_conv {
    unsigned short short_num;
    char *label;
    int assembly_line;
} code_conv;

/* Function prototypes */
char* to_binary_string(unsigned short num);
void print_binary_code(code_conv *code, int code_count);
void save_binary_code_to_file(code_conv *code, int code_count, const char *file_name);

int main() {
    /* Example code_conv array for demonstration */
    code_conv code[] = {
            {0x1F4F, NULL, 1},
            {0x2A9C, "LABEL1", 2},
            {0x3E8D, NULL, 3}
    };

    int code_count = 2;  // Number of entries in the code array

    /* Print binary representation to console */
    print_binary_code(code, code_count);

    /* Save binary representation to file */
    save_binary_code_to_file(code, code_count, "machine_code.bin");

    return 0;
}

/* Function implementations */
char* to_binary_string(unsigned short num) {
    char *binary_str = malloc(17);
    if (!binary_str) {
        printf("Memory allocation failed.\n");
        exit(1);
    }
    binary_str[16] = '\0';
    for (int i = 15; i >= 0; i--) {
        binary_str[i] = (num & 1) ? '1' : '0';
        num >>= 1;
    }
    return binary_str;
}

void print_binary_code(code_conv *code, int code_count) {
    for (int i = 0; i <= code_count; i++) {
        char *binary_str = to_binary_string(code[i].short_num);
        printf("Line %d: %s\n", code[i].assembly_line, binary_str);
        free(binary_str);
    }
}

void save_binary_code_to_file(code_conv *code, int code_count, const char *file_name) {
    FILE *file = fopen(file_name, "w");
    if (!file) {
        printf("Failed to open file for writing.\n");
        return;
    }

    for (int i = 0; i <= code_count; i++) {
        char *binary_str = to_binary_string(code[i].short_num);
        fprintf(file, "Line %d: %s\n", code[i].assembly_line, binary_str);
        free(binary_str);
    }

    fclose(file);
}
