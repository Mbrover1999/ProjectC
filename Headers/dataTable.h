#ifndef DATA_TABLE_H
#define DATA_TABLE_H

#include "front.h"
#include "CodCovert.h"
#include "util.h"

#define IC_INIT_VALUE 100
#define exe 1000



typedef struct label_address {
    int address;
    char *label_name;
    int assembly_line;
    int is_data_line;
} label_address;



typedef struct other_table {
    char *label_name;
    int assembly_line;
} other_table;


/**
 * This function inserts other labels into the other_table.
 *
 */
int insert_other_labels(other_table **table, int count, inst_parts *inst, location am_file, int *error_code);


/**
 * This function inserts a label entry into the label_table.
 */
int
insert_label_table(label_address **label_table, int lines, char *label, int counter, location am_file, int is_data_line,
                   int *error_code);

/**
 * This function checks each label in the label_table to ensure uniqueness.
 *
 */
int check_each_label_once(label_address *label_table, int lines, char *file_name);


/**
 * This function updates the addresses in the label_table for data lines.
 *
 */
void change_label_table_data_count(label_address *label_table, int table_lines, int IC);


/**
 * This function resets the addresses in the label_table to their initial values.
 *
 */
void reset_labels_address(label_address *label_table, int table_lines);

/**
 * This function replaces the externs in the code with the appropriate values.
 *
 */
void replace_externs(code_conv *code, other_table *externs, int externs_count, int count, char *file_name);


/**
 * This function checks if any externs are defined in the label table.
 *
 */
int is_extern_defined(other_table *externs, int externs_count, label_address *label_table, int label_table_line,
                      char *file_name);

/**
 *  This function replaces labels in the code with their corresponding addresses.
 *
 */
int replace_labels(code_conv *code, label_address *label_table, int label_table_line, int IC_len, char *file_name);

/**
 * This function prints the external symbols to an external file.
 */
int print_externs(code_conv *code, int count, other_table *externs, int externs_count, char *file_name);


/**
 *  This function prints the entry symbols to an entry file.
 *
 */
int print_entries(label_address *label_table, int label_table_line, other_table *entries, int entries_count,
                  char *file_name);

/**
 * Frees the memory allocated for the other_table structure
 */
void free_other_table(other_table *table, int count);

/**
 * Frees the memory allocated for the label_table structure.
 */
void free_label_table(label_address *label_table, int label_table_line);

#endif