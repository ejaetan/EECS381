//
//  Person_test.c
//  project1
//
//  Created by Yi Jie Tan on 11/17/16.
//  Copyright © 2016 eecs381. All rights reserved.
//

#include <stdio.h>
#include "Person.h"
#include "Ordered_container.h"
#include "Utility.h"


int cmp_person_lastname(char* person1, char* person2);

int main() {
    
    struct Person* p1 = create_Person("Eugene", "Goh", "734");
    struct Ordered_container* container1 = OC_create_container((OC_comp_fp_t) cmp_person_lastname);
    OC_insert(container1, p1);
    print_Person(p1);
    printf("%s\n", get_Person_lastname(p1));
    destroy_Person(p1);
    //printf("%s\n", get_Person_lastname(p1));
}



int cmp_person_lastname(char* person1, char* person2) {
    return 0;
}
