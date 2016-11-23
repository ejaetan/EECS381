//
//  Utility.c
//  project1
//
//  Created by Yi Jie Tan on 11/14/16.
//  Copyright © 2016 eecs381. All rights reserved.
//

#include "Utility.h"

#include <stdlib.h>


int g_string_memory = 0;


/* malloc might not able to service the request, it might return a null pointer.
 we will need an error handling */
void* malloc_with_error_handling(size_t size) {
    void* new_ptr = malloc(size);
    
    if (!new_ptr) {
        fprintf(stderr, "malloc failed\n");
        return NULL;
    }
    
    return new_ptr;
    
}

/* derived at Meeting.c as helper function */
int cmp_person_lastname(struct Person* person1, struct Person* person2) {
    return strcmp(get_Person_lastname(person1), get_Person_lastname(person2));
}

/* derived at Room.c as helper function */
int convert_time(int a) {
    if (a < 9) {
        a += 12;
    }
    return a;
}
