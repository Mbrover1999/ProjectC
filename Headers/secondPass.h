#ifndef C_PROJECT_SECOND_PASS_H
#define C_PROJECT_SECOND_PASS_H

#include "dataTable.h"
#include "CodCovert.h"
#include "util.h"


/**
 * Execute the second pass of the assembler.
 *
 *  This function executes the second pass of the assembler. It performs various tasks such as checking for errors, merging
 * code and data, replacing externs and labels, converting code to octal format, and printing output files.
 *
 */
int exe_second_pass(char *file_name, label_address *label_table, int IC, int DC, int label_table_line, \
    int externs_count, int entries_count, code_conv *code, code_conv *data, other_table *externs, other_table *entries, \
    int error_found);


/**
* Convert the binary code to octal format and write it to the output file.
*
*/
int conv_code_octal(code_conv *code, int count, char *file_name, int IC, int DC);


#endif