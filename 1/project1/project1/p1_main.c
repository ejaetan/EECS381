//
//  p1_main.c
//  project1
//
//  Created by Yi Jie Tan on 11/20/16.
//  Copyright © 2016 eecs381. All rights reserved.
//

#include <stdio.h>
#include "Ordered_container.h"
#include "Room.h"
#include "Meeting.h"
#include "Utility.h"

#define MAX_CHAR 64

/* function prototypes */
void add_individual(struct Ordered_container* c_ptr);
char input_str[MAX_CHAR];


int main() {
    struct Ordered_container* people_list = OC_create_container((OC_comp_fp_t)cmp_person_lastname);
    char command1, command2;

    int c1_result = scanf(" %c", &command1);
    int c2_result = scanf(" %c", &command2);
    
    if (c1_result > 0 && c2_result > 0) {
        switch (command1) {
            case 'a':
                switch (command2) {
                    case 'i':
                        add_individual(people_list);
                        break;
                        
                    default:
                        printf("Unrecognized command\n");
                        break;  // default break for command2
                }
                break;  //break for 'a'
                
            default:
                printf("Unrecognized command\n");
                break;      // default break for command1
        }
        
        

    }
    
    
    
    
    return 0;
}

/* function definition */
void add_individual(struct Ordered_container* c_ptr) {
    const char *firstname, *lastname, *phoneno;
    for (int i = 0; i < 3, scanf(" %s", input_str) > 0; i++) {
        switch (i) {
            case 1:
                firstname = input_str;
                printf("%s\n", firstname);
                break;
            case 2:
                lastname = input_str;
                printf("%s\n", lastname);
                break;
            case 3:
                phoneno = input_str;
                printf("%s\n", phoneno);
                break;
        }
    }
    struct Person *new_person = create_Person(firstname, lastname, phoneno);
    OC_insert(c_ptr, new_person);
 
}
