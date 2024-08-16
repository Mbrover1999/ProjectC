#ifndef LABRATORY_C_FINAL_PROJECT_UTIL_H
#define LABRATORY_C_FINAL_PROJECT_UTIL_H
/**
 * This function handles changes to our allocated memory.
 *
 */
void *handle_malloc(long object_size);

/**
 * This function converts a short type number to an octal format.
 * @param num an unsigned short number to convert
 * @return a string with the converted value in base64
 */
char *short_to_octal(unsigned short num);

/**
* This function handles adding new files, with the correct ending and name.
*
*/


char *add_new_file(char *file_name, char *ending);

/**
 * This function creates an identical copy of a file
 *
 */
int copy_file(char *file_name_dest, char *file_name_orig);


#endif