#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define as_file_ext ".as"
#define am_file_ext ".am"
#define MAX_MACRO_LENGTH 80
#define MAX_MARCOS  200
#define MAX_LINE_LENGTH 80
#define AFTER_SPACE(s) while (*s && isspace(*s)) s++
#define WHITESPACES " \t\n\r"


struct Macro {
    char Mname[MAX_MACRO_LENGTH + 1];
    char Mlines[MAX_MARCOS][MAX_MACRO_LENGTH];
    int lineC;
};

char *strcatWithMaloc(const char *str1, const char *str2) {
    // Calculate the length of the concatenated string
    size_t len1 = strlen(str1);
    size_t len2 = strlen(str2);
    size_t total_len = len1 + len2 + 1;  // +1 for null terminator

    // Allocate memory for the concatenated string
    char *result = (char *)malloc(total_len * sizeof(char));
    if (result == NULL) {
        // Handle memory allocation failure
        return NULL;
    }

    // Copy str1 and str2 into the allocated memory
    strcpy(result, str1);
    strcat(result, str2);

    return result;
}

struct Macro * searchMacro(struct Macro * macro_table, const int tableSize, const char *name) {
    int i;
    for(i = 0; i < tableSize; i++) {
        if(strcmp(macro_table[i].Mname, name) == 0) {
            return &macro_table[i];
        }
    }
    return NULL;
}

int macro_line(char *s, struct Macro ** mcro, struct Macro * m_table, int * m_table_size) {
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

        if (searchMacro(m_table, *m_table_size, c1))
            return 4;

        if(c1 == "macr" || c1 == "endmacr" || c1 == "data" || c1 == "string" || c1 == "entry" || c1 == "extern")
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
    f = searchMacro(m_table, *m_table_size, s);
    if(f) {
        (*mcro) = f;
        return 2;
    }
    return 3;

}




char * preproc(char * bname) {
    char line[MAX_LINE_LENGTH] = {0};
    struct Macro macro_table[MAX_MARCOS];
    int macro_count = 0;
    int i;
    int line_co = 0;
    FILE * as_file;
    FILE * am_file;
    struct Macro *macro = NULL;
    char * asFileName = strcatWithMaloc(bname, ".as");
    char * amFileName = strcatWithMaloc(bname, ".am");
    am_file = fopen(amFileName, "w");
    as_file = fopen(asFileName, "r");
    if(!as_file || !am_file) {
        printf("Could not open the files! check files and try again.");
        return NULL;
    }
    while(fgets(line, MAX_LINE_LENGTH, as_file)!=0) {
        switch (macro_line(line, &macro, &macro_table[0], &macro_count)) {
            case 0://end of macro
                macro = NULL;
                break;

            case 1://defining macro
                break;

            case 2:
                for(i = 0; i < macro->lineC;i++) {
                    fputs(macro -> Mlines[i], am_file);
                }
            macro = NULL;
            case 3:
                if(macro) {
                    strcpy(macro -> Mlines[macro -> lineC], line);
                    macro -> lineC++;
                }else {
                    fputs(line, am_file);
                }
            case 4:
                printf("ERROR");
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