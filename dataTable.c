#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "Headers/CodCovert.h"
#include "Headers/dataTable.h"


char *add_new_file(char *file_name, char *ending) {
    char *c, *new_file_name;
    new_file_name = handle_malloc(MAX_LINE_LENGTH * sizeof(char));
    strcpy(new_file_name, file_name);
    /* deleting the file name if a '.' exists making sure that our path stays clear */
    if ((c = strchr(new_file_name, '.')) != NULL) {
        *c = '\0';
    }
    strcat(new_file_name, ending);
    return new_file_name;
}



int insert_other_labels(other_table **table, int count, inst_parts *inst, location am_file, int *error_code) {
    other_table *ptr;
    int label_length;

    if(inst->arg_label == NULL)
        return 0;

    ptr = *table;

    /* Update the assembly_line field of the last entry in the table */
    (*table + count - 1)->assembly_line = am_file.line_num;

    /* Calculate the length of the label_name */
    label_length = strlen(inst->arg_label) + 1;

    (*table + count - 1)->label_name = handle_malloc(label_length * sizeof(char));
    if ((*table + count - 1)->label_name == NULL) {
        *error_code = -1;
        return 0;
    }

    strcpy((*table + count - 1)->label_name, inst->arg_label);

    *table = realloc(*table, (count + 1) * sizeof(other_table));
    if (table == NULL) {
        printf("Failed to allocate memory ");
        *error_code = -1;
        free(ptr);
        return 0;
    }

    return 1;
}


int
insert_label_table(label_address **label_table, int lines, char *label, int counter, location am_file, int is_data_line,
                   int *error_code) {
    label_address *p_temp;

    p_temp = *label_table;

    *label_table = realloc(*label_table, lines * sizeof(label_address));
    if (*label_table == NULL) {
        *error_code = -1;
        free(p_temp);
        return 0;
    }

    /* Set the values for the new entry in the label_table */
    (*label_table + lines - 1)->is_data_line = is_data_line;
    (*label_table + lines - 1)->address = counter;
    (*label_table + lines - 1)->assembly_line = am_file.line_num;


    (*label_table + lines - 1)->label_name = malloc((strlen(label) + 1) * sizeof(char));
    if ((*label_table + lines - 1)->label_name == NULL) {
        *error_code = -1;
        return 0;
    }
    strcpy((*label_table + lines - 1)->label_name, label);


    return 1;
}


int check_each_label_once(label_address *label_table, int table_lines, char *file_name) {
    int i, j;

    /* Iterate through each label in the label_table, compare, and print error in case of dupe */
    for (i = 0; i < table_lines - 1; i++) {
        for (j = i + 1; j < table_lines; j++) {
            if (strcmp((label_table + i)->label_name, (label_table + j)->label_name) == 0) {
                location am_file;
                am_file.file_name = file_name;
                am_file.line_num = (label_table + j)->assembly_line;
                printf("Error - duplicate found!");
                return 0;
            }
        }
    }
    return 1;
}


void change_label_table_data_count(label_address *label_table, int table_lines, int IC) {
    int i;
    for (i = 0; i < table_lines; i++) {
        if ((label_table + i)->is_data_line) {
            (label_table + i)->address += IC + 1;
        }
    }
}


void reset_labels_address(label_address *label_table, int table_lines) {
    int i;
    for (i = 0; i < table_lines; i++) {
        /*Set the address to IC Init value (100) */
        (label_table + i)->address += IC_INIT_VALUE;
    }
}


void replace_externs(code_conv *code, other_table *externs, int externs_count, int count, char *file_name) {
    int i, j, found;

    /* Iterate through each entry in the 'code' array, check if label */
    for (i = 0; i <= count; i++) {
        found = 0;
        if ((code + i)->label != NULL) {

            /* Compare the label with each extern in the 'externs' array */
            for (j = 0; j < externs_count && found == 0; j++) {
                if (strcmp((code + i)->label, (externs + j)->label_name) == 0) {
                    (code + i)->short_num -= 1;
                    found = 1;
                }
            }
        }
    }
}


int is_extern_defined(other_table *externs, int externs_count, label_address *label_table, int label_table_line,
                      char *file_name) {
    int i, j, found, extern_defined;
    location am_file;
    extern_defined = 0;

    /* Iterate through each entry in the 'externs' array, compare and check the appropriate storage location */
    for (i = 0; i < externs_count; i++) {
        found = 0;

        for (j = 0; j < label_table_line && found == 0; j++) {

            if (strcmp((externs + i)->label_name, (label_table + j)->label_name) == 0) {
                extern_defined = 1;
                found = 1;
                am_file.file_name = file_name;
                am_file.line_num = (label_table + j)->assembly_line;
                printf("Error - extern is stored in the label_table (not allowed!)");
            }
        }
    }
    return extern_defined;
}

int replace_labels(code_conv *code, label_address *label_table, int label_table_line, int IC_len, char *file_name) {
    int i, j, found, error_found;
    error_found = 0;

    /* Iterate through each entry in the 'code' array, check label and compare with others */
    for (i = 0; i <= IC_len; i++) {
        found = 0;

        /* Check if the current code entry has a label and its short_num is not 1 (extern label with machine code '1') */
        if ((code + i)->label != NULL && (code + i)->short_num != 1) {

            /* Compare the label with each entry in the 'label_table' array */
            for (j = 0; j < label_table_line && found == 0; j++) {
                if (strcmp((code + i)->label, (label_table + j)->label_name) == 0) {

                    (code + i)->short_num |= ((label_table + j)->address) << ARE_BITS;

                    /* found is 1 if we find a match */
                    found = 1;
                }
            }
            if (!found) {
                location am_file;
                am_file.file_name = file_name;
                am_file.line_num = (code + i)->assembly_line;
                printf("Error - label is not defined in the assembly file");
                error_found = 1;
            }
        }
    }
    return !error_found;
}


int print_externs(code_conv *code, int count, other_table *externs, int externs_count, char *file_name) {
    FILE *fp;
    int i, j, found, empty;
    char *temp;

    fp = fopen((temp = add_new_file(file_name, ".ext")), "w");

    empty = 1;

    if (fp == NULL) {
        printf("Error opening file");
        free(temp);
        return 0;
    }

    /* Iterate through each entry in the 'code' array, check if entry has a label and compare*/
    for (i = 0; i <= count; i++) {
        found = 0;

        if ((code + i)->label != NULL) {
            for (j = 0; j < externs_count && found == 0; j++) {
                if (strcmp((code + i)->label, (externs + j)->label_name) == 0) {
                    fprintf(fp, "%s\t%04d\n", (externs + j)->label_name, IC_INIT_VALUE + i);
                    found = 1;
                    empty = 0;
                }
            }
        }
    }
    fclose(fp);

    /* If no extern labels are found, remove the temporary file and free the memory */
    if (empty) {
        remove(temp);
    }

    free(temp);

    return 1;
}


int print_entries(label_address *label_table, int label_table_line, other_table *entries, int entries_count, char *file_name) {
    FILE *fp;
    int i, j, found, empty;
    char *temp;

    fp = fopen((temp = add_new_file(file_name, ".ent")), "w");
    if (fp == NULL) {
        printf("Error opening file");
        free(temp);
        return 0;
    }


    empty = 1;

    /* Iterate through each entry in the 'label_table' array and compare */
    for (i = 0; i < label_table_line; i++) {
        found = 0;

        for (j = 0; j < entries_count && found == 0; j++) {
            if (strcmp((label_table + i)->label_name, (entries + j)->label_name) == 0) {
                fprintf(fp, "%s\t%d\n", (entries + j)->label_name, (label_table + i)->address);
                found = 1;
                empty = 0;
            }
        }
    }

    fclose(fp);

    /* If no entry labels are found, remove the temporary file */
    if (empty) {
        remove(temp);
    }

    free(temp);

    return 1;
}


void free_label_table(label_address *label_table, int label_table_line) {
    int i;
    /* Iterate through each entry in the 'label_table' array, compare and free the allocated memory */
    for (i = 0; i < label_table_line; i++) {
        free((label_table + i)->label_name);
    }

    free(label_table);
}


void free_other_table(other_table *table, int count) {
    int i;
    /* Iterate through each entry in the 'other_table' array, compare and free the allocated memory */
    for (i = 0; i < count; i++) {
        free((table + i)->label_name);
    }


    free(table);
}
