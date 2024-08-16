#include <stdio.h>
#include <stdlib.h>

#include "Headers/firstPass.h"
#include "Headers/preproc.h"



int main(int argc, char *argv[]) {
    char *as_file, *am_file;
    while (--argc > 0) {
        /* Generate a new file with the ".as" extension by adding it to the input filename. */
        printf("Start pre-proc\n");
        as_file = add_new_file(argv[argc], ".as");

        if (!preProc(as_file)) {
            printf("failure to complete the preproc function");

        }

        printf("Start first pass\n");
        /* Generate a new file with the ".am" extension by adding it to the input filename. */
        am_file = add_new_file(argv[argc], ".am");
        if (exe_first_pass(am_file)) {
            printf("Critical assembly - error");

        }

        free(as_file);
        free(am_file);
    }
    printf("end\n");
    return 0;
}