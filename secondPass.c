#include <stdio.h>
#include <stdlib.h>
#include "Headers/dataTable.h"
#include "Headers/secondPass.h"


void free_all_memory(code_conv *code, label_address *label_table, other_table *entries, other_table *externs, \
    int code_count, int label_table_line, int entries_count, int externs_count) {
    free_code(code, code_count);
    free_label_table(label_table, label_table_line);
    free_other_table(entries, entries_count);
    free_other_table(externs, externs_count);
}

int conv_code_octal(code_conv *code, int count, char *file_name, int IC, int DC) {
    int i;
    FILE *fp;
    char *ob_file_name, *octal_char;

    ob_file_name = add_new_file(file_name, ".ob");

    fp = fopen(ob_file_name, "w");
    if (fp == NULL) {
        printf("Error - Opening file - ob\n");
        return 0;
    }

    /* Write the values of IC and DC to the first line of the output file */
    fprintf(fp, "%d %d\n", IC + 1, DC);

    /* Convert each machine code to octal and write it to the output file */
    for (i = 0; i <= count; i++) {
        octal_char = short_to_octal((code + i)->short_num);
        fprintf(fp, "0%d    %s\n",IC_INIT_VALUE+i, octal_char);
        free(octal_char);
    }


    free(ob_file_name);


    fclose(fp);

    return 1;
}


int exe_second_pass(char *file_name, label_address *label_table, int IC, int DC, int label_table_line, \
    int externs_count, int entries_count, code_conv *code, code_conv *data, other_table *externs, other_table *entries, \
    int error_found) {

    /* Check if the Instruction Counter (IC) exceeds the maximum value */
    if (IC > IC_MAX) {
        printf("Too much commands\n");
        error_found = 1;
    }

    /* Check if each label in the label_table appears only once (no duplicate labels) */
    if (check_each_label_once(label_table, label_table_line, file_name) == 0) {
        printf("Error - duplicates - second pass\n");
        error_found = 1;
    }

    /* Check if any extern label is defined in the assembly file (not allowed!) */
    if (is_extern_defined(externs, externs_count, label_table, label_table_line, file_name) == 1) {
        printf("Error - label is defined in the assembly file (not allowed!)\n");
        error_found = 1;
    }

    /* Change the data count in the label_table to the appropriate address */
    change_label_table_data_count(label_table, label_table_line, IC);

    /* Reset the addresses in the label_table to the appropriate addresses after merging the code and data */
    reset_labels_address(label_table, label_table_line);

    /* Merge the code and data arrays into a single array */
    if (merge_code(&code, data, IC, DC) == 0) {
        printf("Error - could not merge\n");
        error_found = 1;
    }

    /* Replace the extern labels with the appropriate machine code (short_num = 1) in the code array */
    replace_externs(code, externs, externs_count, IC + DC, file_name);

    /* Replace the labels in the code array with their corresponding addresses in the label_table */
    if (replace_labels(code, label_table, label_table_line, IC, file_name) == 0) {
        printf("Error - replacing labels\n");
        error_found = 1;
    }


    /* Convert the assembled code to octal format and print it to the output file */
    if (error_found == 0) {
        conv_code_octal(code, IC + DC, file_name, IC, DC);

        print_externs(code, IC + DC, externs, externs_count, file_name);

        print_entries(label_table, label_table_line, entries, entries_count, file_name);
    }

    /* Free all the allocated memory and resources used during the second pass */
    free_all_memory(code, label_table, entries, externs, IC + DC, label_table_line, entries_count, externs_count);

    /* Return 1 if the second pass is executed successfully without errors, 0 otherwise */
    return !error_found;
}