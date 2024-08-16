#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Headers/preproc.h"


#define MAX_MACRO_LENGTH 80
#define MAX_MARCOS  200
#define MAX_LINE_LENGTH_PREPROC 81
#define AFTER_SPACE(s) while (*s && isspace(*s)) s++
#define WHITESPACES " \t\n\r"


struct Macro {
    char Mname[MAX_MACRO_LENGTH + 1];
    char Mlines[MAX_MARCOS][MAX_MACRO_LENGTH];
    int lineC;
};

char *strcatWithMalocPre(const char *str1, const char *str2) {
    /* Calculate the length of the concatenated string */
    size_t len1 = strlen(str1);
    size_t len2 = strlen(str2);
    size_t total_len = len1 + len2 + 1;  /* +1 for null terminator */

    /* Allocate memory for the concatenated string */
    char *result = (char *)malloc(total_len * sizeof(char));
    if (result == NULL) {
        /* Handle memory allocation failure */
        return NULL;
    }

    /* Copy str1 and str2 into the allocated memory */
    strcpy(result, str1);
    strcat(result, str2);

    return result;
}

struct Macro * searchMacroPre(struct Macro * macro_table, const int tableSize, const char *name) {
    int i;
    for(i = 0; i < tableSize; i++) {
        if(strcmp(macro_table[i].Mname, name) == 0) {
            return &macro_table[i];
        }
    }
    return NULL;
}

int macro_linePre(char *s, struct Macro ** mcro, struct Macro * m_table, int * m_table_size) { /* Check macro line */
    char *c1 = s, *c2;
    struct Macro *f;
    AFTER_SPACE(s);
    c1 = strstr(s, "endmcro");
    if(c1){
        mcro = NULL;
        return 0;
    }
    c1 = strstr(s, "mcro");
    if(c1){
        c1+=4;
        AFTER_SPACE(c1);
        c2 = strpbrk(c1, WHITESPACES);

        if(c2) *c2 = '\0';

        if (searchMacroPre(m_table, *m_table_size, c1))
            return 4;

        if (strcmp(c1, "macr") == 0 || strcmp(c1, "endmacr") == 0 ||
            strcmp(c1, "data") == 0 || strcmp(c1, "string") == 0 ||
            strcmp(c1, "entry") == 0 || strcmp(c1, "extern") == 0)
            return 4;


        strcpy(m_table[*m_table_size].Mname, c1);
        *mcro = &m_table[*m_table_size];
        (*m_table_size)++;
        return 1;
    }
    c2 = strpbrk(s, WHITESPACES);
    if(c2) {
        c1 = c2;
        AFTER_SPACE(c2);
        if(*c2 != '\0')
            return 3;
        *c1 = '\0';
    }
    f = searchMacroPre(m_table, *m_table_size, s);
    if(f) {
        (*mcro) = f;
        return 2;
    }
    return 3;

}




char * preProc(char * bname) { /* Main function */
    char line[MAX_LINE_LENGTH_PREPROC] = {0};
    struct Macro macro_table[MAX_MARCOS];
    int macro_count = 0;
    int i;
    int line_co = 0;
    FILE * as_file;
    FILE * am_file;
    struct Macro *macro = NULL;
    char * asFileName = strcatWithMalocPre(bname, ".as");
    char * amFileName = strcatWithMalocPre(bname, ".am");
    am_file = fopen(amFileName, "w");
    as_file = fopen(asFileName, "r");
    if(!as_file || !am_file) {
        printf("Could not open the files! check files and try again.");
        return NULL;
    }
    while(fgets(line, MAX_LINE_LENGTH_PREPROC, as_file)!=0) { /* Read the file, and act according to our cases */
        switch (macro_linePre(line, &macro, &macro_table[0], &macro_count)) {
            case 0:/*end of macro */
                macro = NULL;
                break;

            case 1:/*defining macro */
                break;

            case 2:/* printing the macros into the file */
                for(i = 0; i < macro->lineC;i++) {
                    fputs(macro -> Mlines[i], am_file);
                }
            macro = NULL;
            case 3:/* putting the macro into the macro table */
                if(macro) {
                    strcpy(macro -> Mlines[macro -> lineC], line);
                    macro -> lineC++;
                }else {
                    fputs(line, am_file);
                }
            case 4:
                printf("Error - in processing preproc");
            macro = NULL;
            break;

        }
        line_co++;
    }
    fclose(am_file);
    fclose(as_file);
    free(asFileName);
    return amFileName;
}