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
    struct Person* new_person = malloc_with_error_handling(sizeof(struct Person));
    new_person->firstname = malloc_with_error_handling(sizeof(strlen(firstname) + 1));
    new_person->lastname = malloc_with_error_handling(sizeof(strlen(lastname) + 1));
    new_person->phoneno = malloc_with_error_handling(sizeof(strlen(phoneno) + 1));
    
    strcpy(new_person->firstname, firstname);
    strcpy(new_person->lastname, lastname);
    strcpy(new_person->phoneno, phoneno);
    
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
    printf("%s %s %s\n", person_ptr->firstname, person_ptr->lastname, person_ptr->phoneno);
}

/* Write a Person to a file stream, as a line containing
 firstname, lastname, phoneno,  whitespace-separated with a final \n character. */
void save_Person(const struct Person* person_ptr, FILE* outfile) {
    fprintf(outfile, "%s %s %s\n", person_ptr->firstname, person_ptr->lastname, person_ptr->phoneno);
}

/* Read a Person's data from a file stream, create the data object and
 return a pointer to it, NULL if invalid data discovered in file.
 No check made for whether the Person already exists or not. */
struct Person* load_Person(FILE* infile) {
    char firstname[MAX_CHAR], lastname[MAX_CHAR], phoneno[MAX_CHAR];
    fscanf(infile, " %"STR(X)"s %"STR(X)"s %"STR(X)"s\n", firstname, lastname, phoneno);
        
    struct Person* new_Person = create_Person(firstname, lastname, phoneno);
    return new_Person;
}
