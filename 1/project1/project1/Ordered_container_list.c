/* skeleton file for Ordered_container_list.c
The struct declarations below must be used for the linked-list node and Ordered_container objects.
Remove this comment and complete this file with all necessary code.
*/
#include "Ordered_container.h"
#include "Utility.h"
#include <stdlib.h>
/* struct LL_Node structure declaration. This declaration is local to this file. 
This is a two-way or doubly-linked list. Each node has a pointer to the previous 
node and another pointer to the next node in the list. This means insertions or
removals of a node can be made in constant time, once the location has been
determined. */
struct LL_Node { 
    struct LL_Node* prev;      /* pointer to the previous node */
	struct LL_Node* next;		/* pointer to the next node */
	void* data_ptr; 			/* uncommitted pointer to the data item */
};

/* Declaration for Ordered_container. This declaration is local to this file.  
A pointer is maintained to the last node in the list as well as the first, 
meaning that additions to the end of the list can be made in constant time. 
The number of nodes in the list is kept up-to-date in the size member
variable, so that the size of the list can be accessed in constant time. */
struct Ordered_container {
	OC_comp_fp_t comp_func;
	struct LL_Node* first;
	struct LL_Node* last;
	int size;
};


/*
 Functions for the entire container.
 */

/* Create an empty container using the supplied comparison function, 
and return the pointer to it. */
struct Ordered_container* OC_create_container(OC_comp_fp_t f_ptr) {
    struct Ordered_container* new_Container = (struct Ordered_container*) malloc_with_error_handling(sizeof(struct Ordered_container));
    new_Container->first = NULL;
    new_Container->last = NULL;
    new_Container->size = 0;
    new_Container->comp_func = f_ptr;
    
    return new_Container;
}

/* Destroy the container and its items; caller is responsible for
 deleting all pointed-to data before calling this function.
 After this call, the container pointer value must not be used again. */
void OC_destroy_container(struct Ordered_container* c_ptr) {
    OC_clear(c_ptr);
    free(c_ptr);
    c_ptr = NULL;
}


/* Delete all the items in the container and initialize it.
 Caller is responsible for deleting any pointed-to data first. */
void OC_clear(struct Ordered_container* c_ptr) {
    struct LL_Node* current_node = c_ptr->first;
    while(current_node) {
        if (current_node->next) {
            current_node = current_node->next;
            current_node->prev = NULL;
        } else {
            free(current_node);
            current_node = NULL;
        }
        
    }
    c_ptr->size = 0;
    c_ptr->first = c_ptr->last = NULL;
}


/*
 Functions for working with individual items in the container.
 */

/* Get the data object pointer from an item. */
void* OC_get_data_ptr(const void* item_ptr);


/* Delete the specified item.
 Caller is responsible for any deletion of the data pointed to by the item. */
void OC_delete_item(struct Ordered_container* c_ptr, void* item_ptr);



/* Functions that traverse the items in the container, processing each item in order. */

/* Type of a function used by OC_apply.
 An apply function takes a data pointer as an argument, and returns void.
 It is allowed to modify the data object, but if the ordering information is changed,
 the effects of attempting to search the container afterwards with OC_find are undefined. */
typedef void (*OC_apply_fp_t) (void* data_ptr);

/* Apply the supplied function to the data pointer in each item of the container.
 The contents of the container cannot be modified. */
void OC_apply(const struct Ordered_container* c_ptr, OC_apply_fp_t afp) {
    struct LL_Node* current_node = c_ptr->first;
    
    while(current_node) {
        afp(current_node->data_ptr);
        current_node = current_node->next;
    }
    
}

