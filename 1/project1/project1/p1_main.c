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
void print_group(struct Ordered_container* c_ptr);


int main() {
    struct Ordered_container* people_list = OC_create_container((OC_comp_fp_t)cmp_person_lastname);
    char command1, command2;
    
    while (1) {
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
                    break;  //break for command1 'a'
                case 'p':
                    switch (command2) {
                        case 'g':
                            print_group(people_list);
                            break;
                            
                        default:
                            break;
                    }
                    break;  // break for command1 'p'
                default:
                    printf("Unrecognized command\n");
                    break;      // default break for command1
            }
            
            
            
        }
    }
    return 0;
}

/* function definition */
void add_individual(struct Ordered_container* c_ptr) {
    char firstname[MAX_CHAR], lastname[MAX_CHAR], phoneno[MAX_CHAR];
    
    
    int scan_firstname = scanf(" %63s", firstname);
    int scan_lastname = scanf(" %63s", lastname);
    int scan_phoneno = scanf(" %63s", phoneno);
    
    if (scan_firstname > 0 && scan_lastname > 0 && scan_phoneno) {
        struct Person *new_person = create_Person(firstname, lastname, phoneno);
        OC_insert(c_ptr, new_person);
    }
    
}

void print_group(struct Ordered_container* c_ptr) {
    OC_apply(c_ptr, (OC_apply_fp_t) print_Person);
}
