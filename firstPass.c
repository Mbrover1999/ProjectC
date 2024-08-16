/* Contains major function that are related to the first pass */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "Headers/firstPass.h"
#include "Headers/dataTable.h"
#include "Headers/front.h"
#include "Headers/CodCovert.h"
#include "Headers/secondPass.h"



int handle_allocation(other_table **externs, other_table **entries, code_conv **code, code_conv **data) {
    int error_found = 0;

    *externs = handle_malloc(sizeof(other_table));
    if (*externs == NULL) {
        error_found = 1;
        printf("Error - failed to allocate memory for externs\n");
        return error_found;
    }

    *entries = handle_malloc(sizeof(other_table));
    if (*entries == NULL) {
        error_found = 1;
        printf("Error - failed to allocate memory for entries\n");
        return error_found;
    }

    *code = handle_malloc(sizeof(code_conv));
    if (*code == NULL) {
        error_found = 1;
        printf("Error - failed to allocate memory for code\n");
        return error_found;
    }

    *data = handle_malloc(sizeof(code_conv));
    if (*code == NULL) {
        error_found = 1;
        printf("Error - failed to allocate memory for data\n");
        return error_found;
    }

    /* If the program gets to this point, then all memory allocations were successful.*/
    /* Return the error flag (should be 0 at this point). */
    return error_found;
}



int exe_first_pass(char *file_name) {
    int error_code, IC, DC, error_found, label_table_line, externs_count, entries_count, inst_created = 1;

    code_conv *code, *data;
    other_table *externs;
    other_table *entries;
    command_parts *command;
    inst_parts *inst;
    location am_file;
    char str[MAX_LINE_LENGTH];
    char str_copy[MAX_LINE_LENGTH];


    FILE *fp;
    label_address *label_table;

    error_code = error_found = 0;

    if (lines_too_long(file_name)) {
        error_found = 1;
    }

    fp = fopen(file_name, "r");
    am_file.file_name = file_name;
    am_file.line_num = 0;

    label_table_line = externs_count = entries_count = 0;
    label_table = NULL;

    /* Initialize the instruction counter and data counter */
    IC = -1;
    DC = 0;

    /* Handle the memory allocation for externs, entries, code, and data */
    error_found = handle_allocation(&externs, &entries, &code, &data);

    /* Start reading the file line by line */
    while (fgets(str, MAX_LINE_LENGTH, fp) != NULL && IC + DC <= IC_MAX - IC_INIT_VALUE) {
        error_code = 0;
        (am_file.line_num)++;
        if (strcmp(str, "\n") == 0) {
            continue;
        }

        if (strchr(str, '.')) {
            /*Copy the str */
            strcpy(str_copy, str);
            if (strstr(str_copy, ".entry") || strstr(str_copy, ".extern")) {
                inst = read_entry_or_extern(str_copy, &error_code);
                if (inst->is_extern == 0)
                    insert_other_labels(&entries, ++entries_count, inst, am_file, &error_code);


                    /* is a .extern line */
                else if (inst->is_extern == 1)
                    insert_other_labels(&externs, ++externs_count, inst, am_file, &error_code);

            } else if (strstr(str_copy, ".data") != NULL || strstr(str_copy, ".string") != NULL) {
                /* Parse the instruction */
                inst = read_instruction(str_copy, &error_code);
                if (inst->label != NULL) {
                    insert_label_table(&label_table, ++label_table_line, inst->label, DC, am_file, 1, &error_code);
                }
            } else {
                printf("Error - Illegal data line directive - must be .data or .string or .extern or . entry\n");
                inst_created = 0;
                error_code =-1;
            }

            if (error_code != 0) {
                printf("Error - in first pass.\n");
                if (inst_created)
                    free(inst);
                error_found = 1;
                continue;
            } else {

                if (inst_created) {
                    if (add_machine_code_data(&data, inst, &DC, am_file) == 0) {
                        printf("Error - is not .entry nor .extern --> is .data or .string\n");
                        error_found = 1;
                        continue;
                    }
                }
            }
            if (inst_created) {
                if (inst->nums)
                    free(inst->nums);
                free(inst);
            }
        } else {
            command = read_command(str, &error_code);
            if (error_code == 0) {
                IC++;
                if (command != NULL && command->label != NULL) {
                    insert_label_table(&label_table, ++label_table_line, command->label, IC, am_file, 0, &error_code);
                }
            } else {
                printf("Error");
                free(command);
                error_found = 1;
                continue;
            }

            if (add_machine_code_line(&code, command_to_short(command), NULL, &IC, am_file) == 0) {
                free(command);
                error_found = 1;
                continue;
            }
            /*Try to add the additional machine code lines*/
            if (add_extra_machine_code_line(&code, command, &IC, 1, am_file) == 0 || \
            add_extra_machine_code_line(&code, command, &IC, 0, am_file) == 0) {
                free(command);
                error_found = 1;
                continue;
            }
            free(command);
        }
    }

/* Execute the second pass. If it fails, set error_found to 1 */
    if (exe_second_pass(file_name, label_table, IC, DC, label_table_line, externs_count, entries_count, code, data,
                        externs, entries, error_found) == 0) {
        error_found = 1;
    }

    fclose(fp);


    return
            error_found;
}

