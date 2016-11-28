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
    
    FILE *ofp = fopen("person_out.txt", "w");
    if (!ofp) {
        fprintf(stderr, "Can't open file");
        exit(1);
    }
    
    save_Person(p1, ofp);
    fclose(ofp);
    
    FILE *ifp = fopen("person_out.txt", "r");
    if (!ifp) {
        fprintf(stderr, "Can't open file");
        exit(1);
    }
    
    struct Person* p2 = load_Person(ifp);
    print_Person(p2);
    fclose(ifp);
}



