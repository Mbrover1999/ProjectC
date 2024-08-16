#ifndef C_PROJECT_FRONT_H
#define C_PROJECT_FRONT_H
#include "util.h"

#define MAX_LABEL_LENGTH 31
#define MAX_LINE_LENGTH 81
#define BIG_NUMBER_CONST 100
#define OPCODES_COUNT 16
#define REG_COUNT 8
#define WORD_LEN 12
#define INSTRUCTIONS_COUNT 4
#define MAX_NUM ((1 << (WORD_LEN-1)) - 1)
#define MIN_NUM (-(1 << (WORD_LEN-1)))

typedef struct location {
    char *file_name;
    int line_num;
} location;




typedef struct op_code {
    char *opcode;    /* The opcode corresponding to the operation */
    int arg_num;     /* The number of arguments for the operation */
} op_code;


typedef struct command_parts {
    char *label;     /* The label associated with the command */
    int opcode;      /* The opcode of the command */
    char *source;    /* The source operand of the command */
    char *dest;      /* The destination operand of the command */
    int are;
} command_parts;


typedef struct inst_parts {
    char *label;       /* The label associated with the instruction */
    short *nums;       /* An array of short integers representing instruction data */
    int len;           /* The number of data in *nums including '0' at the end of a string */
    char *arg_label;   /* The label associated with an argument, if applicable */
    int is_extern;     /* Indicates whether the instruction is marked as external */
} inst_parts;




/**
 * Reads an entry or extern directive from a string.
 *
 */
inst_parts *read_entry_or_extern(char *str ,int *error_code);


/**
 * This function checks whether any line in the provided file exceeds the maximum length limit.
 *
 */
int lines_too_long(char *file_name);


/**
 * Checks if the provided string is an instruction.
 *
 */
int is_instr(char *str);


/**
 * This function returns the index of the opcode in the predefined opcode list.
 *
 **/
int what_opcode(char *str);


/**
 * This function returns the index of the register in the predefined register list.
 *
**/
int what_reg(char *str);


/**
 * This function checks if the provided string is a valid label declaration.
 *
 */
int legal_label_decl(char *str, int * error_code);


/**
 * This function checks if the provided string is a valid label.
 *
 */
int legal_label(char *str);


/**
 * This function checks if there is any extra text after a newline character in the token stream.
 *
 */
int extra_text();


/**
 * Adds a space character after the colon (:) in the given string.
 *
 */
int add_space_after_colon(char **str, int *error_code);


/**
 * This function checks if the provided string is either a valid register or a valid label.
 *
 */
int is_reg_or_label(char *str);


/**
 * Checks for errors related to register usage in a string.
 *
 */
void check_reg_error(int *error_code, char *str);


/**
 * This function checks if the provided string is a valid integer number.
 *
 */
int is_num(char *str);


/**
 * This function checks if the provided string is a valid register, label, or numeric string.
 *
 */
int is_reg_or_label_or_num(char *str);


/**
 * This function checks the legality of an argument for a given command.
 */
int legal_arg(char *str, command_parts *command, int *error_code);

/**
 * This function reads a command from the input, parses it, and stores it in a command_parts structure.
 */
command_parts *read_command(char *str, int *error_code);


/**
 * This function captures numbers from a string and stores them into the nums array of inst_parts structure.
 *
 */
int capture_nums(char *str, char *token_copy, inst_parts *inst, int *error_code);


/**
 * This function reads an instruction from the input, parses it and stores it in an inst_parts structure.
 *
 */
inst_parts *read_instruction(char *str, int *error_code);


/**
 * This function increments the size of the array (*inst)->nums.
 *
 */
int inc_array(inst_parts **inst, int len);


/**
 * This function checks for errors related to opcode parsing.
 *
 */
int opcode_err_check(char *str);


/**
 * This function counts the occurrence of a character in a given string.
 *
 */
int count_occurr(char *str, char ch);


/**
 * Checks if a comma appears after a directive in a string.
 *
 */
int is_comma_after_directive(char *str, int *error_code);


/**
 * Checks if a given string represents a legal sequence of integers separated by commas.
 */
int is_string_legal(const char *str);


/**
 *  Checks the validity of the first argument in a given string.
 *
 * This function extracts the first argument from the input string and checks
 * whether it is a valid register, label, or number using the `is_reg_or_label_or_num` function.
 *
 * @param str The input string containing the complete argument list.
 * @param ptr A pointer to the character in `str` where the first argument ends.
 * @return Returns the result of the `is_reg_or_label_or_num` function, which will be:
 *         - 1 if the first argument is a valid register.
 *         - 2 if the first argument is a valid label.
 *         - 3 if the first argument is a valid number.
 *         - 0 if the first argument is not a valid register, label, or number.
 */
int check_first_arg(char *str, char *ptr);


/**
 *  This function captures a string from the input and stores it as an array of shorts in the inst_parts structure.
 *
 */
int capture_string(char *str, inst_parts *inst, int *error_code);

#endif