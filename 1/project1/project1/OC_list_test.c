//
//  OC_list_test.c
//  project1
//
//  Created by Yi Jie Tan on 11/15/16.
//  Copyright © 2016 eecs381. All rights reserved.
//

#include <stdio.h>
#include "Ordered_container.h"


#include "Utility.h"

int compare_rm_num (int a, int b);

int main() {
    struct Ordered_container* container1 = OC_create_container((OC_comp_fp_t) compare_rm_num);
    
    int *data1, *data2;
    int a = 3, b = 2;
    data1 = &a;
    data2 = &b;
    OC_insert(container1, data1);
    //print_container(container1);
    OC_insert(container1, data2);
    print_container(container1);
    
    
}


int compare_rm_num (int a, int b) {
    if (a > b) return 1;
    if (a < b) return -1;
    return 0;
}


