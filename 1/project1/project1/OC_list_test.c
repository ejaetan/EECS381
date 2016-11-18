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

int compare_int_ptr (void* a, void* b);

int main() {
    struct Ordered_container* container1 = OC_create_container((OC_comp_fp_t) compare_int_ptr);
    int a = 4, b = 4, c = 3;
    int *data1 = &a, *data2 = &b, *data3 = &c;

    OC_insert(container1, data1);
    print_container(container1);
    OC_insert(container1, data2);
    print_container(container1);
    OC_insert(container1, data3);
    print_container(container1);
    printf("AFter insert size: %d\n", OC_get_size(container1));
    
    
    OC_delete_item(container1, data1);
    printf("Size: %d\n", OC_get_size(container1));
    print_container(container1);
    
    OC_delete_item(container1, data2);
    printf("Size: %d\n", OC_get_size(container1));
    print_container(container1);
    
    OC_delete_item(container1, data3);
    printf("Size: %d\n", OC_get_size(container1));
    print_container(container1);

    
    OC_destroy_container(container1);
    printf("%d\n", g_Container_count);
    printf("%d\n", g_Container_items_in_use);
    printf("%d\n", g_Container_items_allocated);
    
}


int compare_int_ptr (void* a, void* b) {
    if (*(int*) a > *(int*)b) return 1;
    if (*(int*)a < *(int*)b) return -1;
    return 0;
}


