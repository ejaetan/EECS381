//
//  OC_array_test.c
//  project1
//
//  Created by Yi Jie Tan on 12/1/16.
//  Copyright © 2016 eecs381. All rights reserved.
//

#include <stdio.h>
#include "Ordered_container.h"
#include "Utility.h"

int compare_int_ptr (int a, int b);

int main() {
    struct Ordered_container* container1 = OC_create_container((OC_comp_fp_t) compare_int_ptr);
    int a = 4, b = 4, c = 3, d = 5, e = 2;
    
    
    OC_insert(container1, a);
    print_container(container1);
    OC_insert(container1, c);
    print_container(container1);
    OC_insert(container1, b);
    print_container(container1);
    OC_insert(container1, d);
    print_container(container1);
    OC_insert(container1, e);
    print_container(container1);

    
    
    //printf("%d\n",OC_get_size(container1));
}



int compare_int_ptr (int a, int b) {
    return a - b;
}
