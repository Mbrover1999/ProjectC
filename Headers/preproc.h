#ifndef PREP_ROC_H
#define PREP_ROC_H

/**
 *  This function allocates memory for two strings.
 *
 */
char *strcatWithMalocPre(const char *str1, const char *str2);

/**
 *  This function searches for a macro in a given table.
 *
 */
struct Macro * searchMacroPre(struct Macro * macro_table, const int tableSize, const char *name);

/**
 *  This function determines the macro line and the appropriate case.
 *
 */
int macro_linePre(char *s, struct Macro ** mcro, struct Macro * m_table, int * m_table_size);

/**
 *  This function is the main function, this function will use the above functions to read the macro file and act upon it.
 *
 */
char * preProc(char * bname);



#endif