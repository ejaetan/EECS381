/* Complete this header file with include guards and the full declaration for
the int global variable named below. 

Add any additional definitions or declarations needed for the project. 
(See the Project Document for requirements and suggestions.)

Rename this file to "Utility.h" for use in your project. 

Then delete this comment - it is not part of the finished code */

#include "string.h"
#include "Person.h"


#define MAX_CHAR 64
#define MAX_INPUT 63
#define STR2(x)
#define STR(X) STR2(X)
/* global variable declarations for this module */
extern int g_string_memory;				/* number of bytes used in C-strings */

/* Your own decleration and definitions */

void* malloc_with_error_handling(size_t size);
int cmp_person_lastname(struct Person* person1, struct Person* person2);
int convert_time(int a);
int cmp_person_lastname_arg(char *lastname, struct Person * person_ptr);
