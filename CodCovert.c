#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "Headers/front.h"
#include "Headers/CodCovert.h"

int inc_mem(code_conv **code, int counter) {
    code_conv *ptr;
    ptr = *code;
    *code = realloc(*code, (counter + 1) * sizeof(code_conv));
    if (*code == NULL) {
        printf("Error");
        free(ptr);
        return 0;
    }
    return 1;
}

unsigned short command_to_short(command_parts *command) {
    unsigned short n_src, n_op, n_dest, n_are;
    n_src = n_op = n_dest = n_are = 0;

    /* Check if the source is a register and set the corresponding bits. */
    if (what_reg(command->source) >= 0) {
        n_src = REG_ADDRESSING << SOURCE_BITS_SHIFT;  /* Fill in register addressing bits.*/
    }
        /* Check if the source is a legal label and set the corresponding bits. */
    else if (legal_label(command->source)) {
        n_src = LABEL_ADDRESSING << SOURCE_BITS_SHIFT;  /* Fill in label addressing bits. */
    }
        /* Check if the source is a numerical value and set the corresponding bits. */
    else if (is_num(command->source)) {
        n_src = DIRECT_ADDRESSING << SOURCE_BITS_SHIFT;  /* Fill in direct addressing bits. */
    }

    /* Check if the destination is a register and set the corresponding bits. */
    if (what_reg(command->dest) >= 0) {
        n_dest = REG_ADDRESSING << DEST_BITS_SHIFT;  /* Fill in register addressing bits. */
    }
        /* Check if the destination is a legal label and set the corresponding bits. */
    else if (legal_label(command->dest)) {
        n_dest = LABEL_ADDRESSING << DEST_BITS_SHIFT;  /* Fill in label addressing bits. */
    }
        /* Check if the destination is a numerical value and set the corresponding bits. */
    else if (is_num(command->dest)) {
        n_dest = DIRECT_ADDRESSING << DEST_BITS_SHIFT;  /* Fill in direct addressing bits. */
    }

    /* Set the opcode bits. */
    n_op = (command->opcode) << OPCODE_BITS_SHIFT;  /* Fill in opcode bits. */

    /* Set the ARE bits. */
    n_are = (command->are & ((1 << 3) - 1));  /* Adjust the ARE bits mask as needed. */

    /* Combine the source, opcode, and destination bits to obtain the final unsigned short representation. */
    return (n_op | n_src | n_dest | n_are);  /* Bit 15 remains 0 as it is unused. */
}

unsigned short reg_to_short(command_parts *command, int reg_src) {
    static int already_done;
    int reg1, reg2;
    unsigned short n_reg_src, n_reg_dest;
    n_reg_src = n_reg_dest = 0;

    if (reg_src) {
        /* if the register we are converting is a source argument */
        if ((reg1 = what_reg(command->source)) >= 0) {
            n_reg_src = reg1 << SOURCE_BITS_SHIFT_REG;
        }
        /* if the dest argument is also a register */
        if ((reg2 = what_reg(command->dest)) >= 0) {
            n_reg_dest = reg2 << DEST_BITS_SHIFT_REG;
        }
        already_done = 1;
        return (n_reg_src | n_reg_dest);
    }
        /* Dealing with dest register, checking we didn't deal with it before we need to check if it's a register */
    else if (already_done == 0) {
        if ((reg2 = what_reg(command->dest)) >= 0) {
            n_reg_dest = reg2 << DEST_BITS_SHIFT_REG;
        }
        return n_reg_dest;
    }
    already_done = 0;
    return DOUBLE_REGS_VALUE;
}

int add_machine_code_line(code_conv **code, unsigned short num, char *str, int *IC, location am_file) {
    /* Check if memory allocation increase for the code_conv array succeeded */
    if (inc_mem(code, *IC) == 0) {
        return 0;  /* Return 0 if memory allocation fails */
    }

    /* Set the numerical value of the assembly line */
    (*code + *IC)->short_num = num;

    /* Set the assembly line number */
    (*code + *IC)->assembly_line = am_file.line_num;

    /* Check if a label is available in this line, and handle memory allocation for it. */
    if (str == NULL) {
        (*code + *IC)->label = NULL;
    } else {
        (*code + *IC)->label = handle_malloc((strlen(str) + 1) * sizeof(char));
        if ((*code + *IC)->label == NULL) {
            return 0;
        }
        strcpy((*code + *IC)->label, str);
    }

    return 1;
}

int add_extra_machine_code_line(code_conv **code, command_parts *command, int *IC, int is_src, location am_file) {
    unsigned short num;
    char *arg;
    /* Determine the argument based on whether the source or destination operand is being processed */
    arg = (is_src) ? command->source : command->dest;
    if (what_reg(arg) > 0 && (num = reg_to_short(command, is_src)) != DOUBLE_REGS_VALUE) {
        (*IC)++;
        if (add_machine_code_line(code, num, NULL, IC, am_file) == 0) {
            return 0;
        }
    }
        /* If arg is not a register it checks if it's a legal label and codes it */
    else if (legal_label(arg)) {
        (*IC)++;
        if (add_machine_code_line(code, RELOCATABLE_VALUE, arg, IC, am_file) == 0) {
            return 0;
        }
    }
        /* Check if the argument is a numerical value */
    else if (is_num(arg)) {
        (*IC)++;
        /* representing number in 2-11 bits, therefore pushing the number ARE_BITS bits to the left */
        if (add_machine_code_line(code, atoi(arg) << ARE_BITS, NULL, IC, am_file) == 0) {
            return 0;
        }
    }

    return 1;

}

int add_machine_code_data(code_conv **data, inst_parts *inst, int *DC, location am_file) {
    int i;
    int inst_len;
    inst_len = inst->len;
    /*Check if data pointer is NULL before proceeding */
    if (data == NULL || *data == NULL) {
        return 0;
    }
    i = 0;
    for (i = 0; i < inst_len; i++) {
        /* Check if memory allocation for the code_conv array succeeded */
        if (inc_mem(data, *DC) == 0) {
            return 0;
        }
        /* Set the numerical value in the current code_conv entry */
        (*data + *DC)->short_num = *(inst->nums + i);

        /* a data line cannot include a label as an argument */
        (*data + *DC)->label = NULL;

        (*data + *DC)->assembly_line = am_file.line_num;
        (*DC)++;
    }
    return 1;
}

int merge_code(code_conv **code, code_conv *data, int IC, int DC) {
    int i;
    code_conv *ptr;
    ptr = *code;

    if (inc_mem(code, IC + DC) == 0) {
        free(ptr);
        free(data);
        return 0;
    }
    /* Coping the info from the data lines into the end of the command code lines */
    for (i = 0; i < DC; i++) {
        (*code + IC + i + 1)->label = (data + i)->label;
        (*code + IC + i + 1)->assembly_line = (data + i)->assembly_line;
        (*code + IC + i + 1)->short_num = (data + i)->short_num;
    }
    free(data);
    return 1;
}

void free_code(code_conv *code, int code_count) {
    int i;

    /* free all memory for label strings */
    for (i = 0; i <= code_count; i++) {
        if ((code + i)->label != NULL) {
            free((code + i)->label);
        }
    }

    free(code);
}