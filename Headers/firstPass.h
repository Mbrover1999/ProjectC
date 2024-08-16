#ifndef FIRST_PASS_H
#define FIRST_PASS_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "dataTable.h"
#include "util.h"
#include "front.h"
#include "CodCovert.h"
#include "util.h"
#include "secondPass.h"

/**
 * Executes the first pass of the assembly file parsing and processing.
 */
int exe_first_pass(char *file_name);

/**
 * Handles memory allocation for data structures used in the first pass.
 *
 */
int handle_allocation(other_table **externs, other_table **entries, code_conv **code, code_conv **data);

#endif