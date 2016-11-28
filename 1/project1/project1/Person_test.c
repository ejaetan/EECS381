//
//  Person_test.c
//  project1
//
//  Created by Yi Jie Tan on 11/17/16.
//  Copyright © 2016 eecs381. All rights reserved.
//
#include <stdlib.h>
#include <stdio.h>
#include "Person.h"
#include "Ordered_container.h"
#include "Utility.h"


int main() {
    
    struct Person* p1 = create_Person("Eugene", "Goh", "734");
    struct Ordered_container* container1 = OC_create_container((OC_comp_fp_t) cmp_person_lastname);
    OC_insert(container1, p1);
    print_Person(p1);
    printf("%s\n", get_Person_lastname(p1));
    //destroy_Person(p1);
    //printf("%s\n", get_Person_lastname(p1));
    
    FILE *fp = fopen("person_out.txt", "w");
    if (!fp) {
        fprintf(stderr, "Can't open file");
        exit(1);
    }
    
    save_Person(p1, fp);
}



