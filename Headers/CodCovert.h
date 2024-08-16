#ifndef COD_CONVER_H
#define COD_CONVER_H


#include "front.h"
#include "util.h"


#define OPCODE_BITS_SHIFT 11  /* Shift left needed to place opcode in bits 14-11 */
#define SOURCE_BITS_SHIFT 7   /* Shift left needed to place source operand in bits 10-7 */
#define DEST_BITS_SHIFT 3     /* Shift left needed to place destination operand in bits 6-3 */
#define UNUSED_BIT 15 /* bit position for unused field */

#define SOURCE_BITS_SHIFT_REG 7 /* Shift left needed to place source register operand in bits 10-7 */
#define DEST_BITS_SHIFT_REG 3   /* Shift left needed to place destination register operand in bits 6-3 */


/* A,R,E for addressing methods values  */
#define DIRECT_ADDRESSING 1
#define LABEL_ADDRESSING 3
#define REG_ADDRESSING 5

/* value to indicate that both registers have been dealt with already to avoid dealing twice with dest register */
#define DOUBLE_REGS_VALUE 10000

#define ARE_BITS 0
#define RELOCATABLE_VALUE 2

/* Memory limit */
#define IC_MAX 1023


typedef struct code_conv {
    unsigned short short_num; /* The numerical representation of the converted line */
    char *label; /* An optional label associated with the line */
    int assembly_line; /* The corresponding assembly line number */
} code_conv;


/**
 * This function increments the memory allocation for a code_conv array.
 *
 */
int inc_mem(code_conv **code, int IC);

/**
 * This function converts a command_parts structure to an unsigned short representing the machine code.
 *
 */
unsigned short command_to_short(command_parts *command);

/**
 * This function adds machine code data to the data array.
 *
 */
int add_machine_code_data(code_conv **data, inst_parts *inst, int *DC, location am_file);

/**
 * This function adds a machine code line to the code_conv array.
 *
 */
int add_machine_code_line(code_conv **code, unsigned short num, char *str, int *IC, location am_file);

/**
 * This function prints to stdout the machine words of the source file in binary representation
 *
 */
void print_binary_code(code_conv *code, int IC_len);

/**
 * This function merges the code and data arrays into a single code array.
 *
 */
int merge_code(code_conv **code, code_conv *data, int IC, int DC);

/**
 * This function converts the optional second and third code words from a command line to machine
 * wards and stores them in the array of struct which contains the machine code of the command lines
 */
int add_extra_machine_code_line(code_conv **code, command_parts *command, int *IC, int is_src, location am_file);

/**
 * This function converts register operands to their corresponding bit representation in the machine code.
 *
 */
unsigned short reg_to_short(command_parts *command, int reg_src);

/**
 * This function frees the memory allocated for the code_conv code array.
 *
 */
void free_code(code_conv *code, int code_count);


#endif