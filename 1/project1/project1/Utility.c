//
//  Utility.c
//  project1
//
//  Created by Yi Jie Tan on 11/14/16.
//  Copyright © 2016 eecs381. All rights reserved.
//

#include "Utility.h"
#include <string.h>
#include <stdlib.h>
#include "stdio.h"


/* malloc might not able to service the request, it might return a null pointer.
 we will need an error handling */
void* malloc_with_error_handling(size_t size) {
    void* new_ptr = malloc(sizeof(size));
    
    if (!new_ptr) {
        fprintf(stderr, "malloc failed\n");
        return(-1);
    }
    
    return new_ptr;
    
}
