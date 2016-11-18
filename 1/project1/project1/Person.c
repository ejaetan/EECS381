#include "Person.h"
#include "Utility.h"
#include <string.h>
#include <stdlib.h>

/* a Person consist of pointers to C-strings for names and phone number */
struct Person {
	char* firstname;
	char* lastname;
	char* phoneno;
};

/* Create a Person object.
 This is the only function that allocates dynamic memory for a Person
 and the contained data. */
struct Person* create_Person(const char* firstname, const char* lastname, const char* phoneno) {
    struct Person* new_person = (struct Person*) malloc_with_error_handling(sizeof(struct Person));
    new_person->firstname = (char*) malloc_with_error_handling(sizeof(strlen(firstname) + 1));
    new_person->lastname = (char*) malloc_with_error_handling(sizeof(strlen(lastname) + 1));
    new_person->phoneno = (char*) malloc_with_error_handling(sizeof(strlen(phoneno) + 1));
    
    new_person->firstname = strcpy(new_person->firstname, firstname);
    new_person->lastname =  strcpy(new_person->lastname, lastname);
    new_person->phoneno =  strcpy(new_person->phoneno, phoneno);
    
    g_string_memory += strlen(firstname) + strlen(lastname) + strlen(phoneno) + 3;
    return new_person;
    
}

/* Destroy a Person object
 This is the only function that frees the memory for a Person
 and the contained data. */
void destroy_Person(struct Person* person_ptr) {
    g_string_memory -= (strlen(person_ptr->firstname) +
                       strlen(person_ptr->lastname) +
                       strlen(person_ptr->phoneno) + 3);
    
    free(person_ptr->firstname);
    free(person_ptr->lastname);
    free(person_ptr->phoneno);
    person_ptr->firstname = NULL;
    person_ptr->lastname = NULL;
    person_ptr->phoneno = NULL;
    free(person_ptr);
    person_ptr = NULL;
    
}

/* Return the lastname pointer as a pointer to unmodifiable characters. */
const char* get_Person_lastname(const struct Person* person_ptr) {
    return person_ptr->lastname;
}

/* Print a Person data item to standard output with a final \n character. */
void print_Person(const struct Person* person_ptr) {
    printf("%s\t%s\t%s\t\n", person_ptr->firstname, person_ptr->lastname, person_ptr->phoneno);
}
