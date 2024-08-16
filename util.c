#include <stdio.h>
#include <stdlib.h>



void *handle_malloc(long object_size) {
    void *object_ptr = malloc(object_size);
    if (object_ptr == NULL) {
        printf("Error");
    }
    return object_ptr;
}

/* Function to convert an unsigned short integer to an octal string */
char *short_to_octal(unsigned short num) {
    char *res;
    int size;

    /* Calculate the required size of the string */
    size = snprintf(NULL, 0, "%o", num); /* Get the length of the octal representation */

    /* Allocate memory for the result string (+1 for the null terminator) */
    res = (char *)malloc((size + 1) * sizeof(char));
    if (res == NULL) {
        perror("Memory allocation failed");
        return NULL;
    }

    /* Format the number as an octal string */
    snprintf(res, size + 1, "%o", num);

    return res;
}
