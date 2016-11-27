/* skeleton file for Ordered_container_list.c
The struct declarations below must be used for the linked-list node and Ordered_container objects.
Remove this comment and complete this file with all necessary code.
*/
#include "Ordered_container.h"
#include "Utility.h"
#include <stdlib.h>


int g_Container_count = 0;		/* number of Ordered_containers currently allocated */
int g_Container_items_in_use = 0;	/* number of Ordered_container items currently in use */
int g_Container_items_allocated = 0;	/* number of Ordered_container items currently allocated */

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
    struct Ordered_container* new_Container = malloc_with_error_handling(sizeof(struct Ordered_container));
    new_Container->first = new_Container->last = NULL;
    new_Container->size = 0;
    new_Container->comp_func = f_ptr;
    
    g_Container_count++;
    return new_Container;
}

/* Destroy the container and its items; caller is responsible for
 deleting all pointed-to data before calling this function.
 After this call, the container pointer value must not be used again. */
void OC_destroy_container(struct Ordered_container* c_ptr) {
    if (c_ptr) {
        OC_clear(c_ptr);
        free(c_ptr);
        c_ptr = NULL;
        g_Container_count--;
    }
    
}


/* Delete all the items in the container and initialize it.
 Caller is responsible for deleting any pointed-to data first. */
void OC_clear(struct Ordered_container* c_ptr) {
    if (c_ptr) {
        int size = OC_get_size(c_ptr);
        
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
        g_Container_items_in_use -= size;
        g_Container_items_allocated -= size;
        c_ptr->size = 0;
        c_ptr->first = c_ptr->last = NULL;
    }
    
}

/* Return the number of items currently stored in the container */
int OC_get_size(const struct Ordered_container* c_ptr) {
    return c_ptr->size;
}

/* Return non-zero (true) if the container is empty, zero (false) if the container is non-empty */
int OC_empty(const struct Ordered_container* c_ptr) {
    return (OC_get_size(c_ptr)) ? 0 : -1;
}

/*
 Functions for working with individual items in the container.
 */

/* Get the data object pointer from an item. */
void* OC_get_data_ptr(const void* item_ptr) {
    return ((struct LL_Node*) item_ptr)->data_ptr;
}


/* Delete the specified item.
 Caller is responsible for any deletion of the data pointed to by the item. */
void OC_delete_item(struct Ordered_container* c_ptr, void* item_ptr) {
    if (c_ptr && item_ptr) {
        struct LL_Node* current_node = (struct LL_Node*) item_ptr;
        int OC_size = OC_get_size(c_ptr);
        if(OC_size == 1) {
            c_ptr->first = c_ptr->last = NULL;
        } else if (OC_size > 1) {
            // delete item is the first item on the list
            if (!current_node->prev) {
                c_ptr->first = c_ptr->first->next;
                c_ptr->first->prev = NULL;
            } else if (!current_node->next) {
                c_ptr->last = c_ptr->last->prev;
                c_ptr->last->next = NULL;
            }
        } else if (OC_size > 2) {
            struct LL_Node *curr_prev_node = NULL, *curr_next_node = NULL;
            curr_prev_node = current_node->prev;
            curr_next_node = current_node->next;
            curr_prev_node->next = curr_next_node;
            curr_next_node->prev = curr_prev_node;
        }
        free(current_node);
        current_node = NULL;
        c_ptr->size--;
        g_Container_items_in_use--;
        g_Container_items_allocated--;
    }
}

/*
 Functions that search and insert into the container using the supplied comparison function.
 */


int OC_equals_or_just_over(const struct Ordered_container* c_ptr, const void* data_ptr,
                           OC_comp_fp_t f_ptr, struct LL_Node** current_node);

/* Search the list with a linear scan, stop scanning when data_ptr 'gone past' 
where it should be */
int OC_equals_or_just_over(const struct Ordered_container* c_ptr, const void* data_ptr,
                           OC_comp_fp_t f_ptr,struct LL_Node** current_node) {
    
    int result = -1;
    while (*current_node && (result = f_ptr(data_ptr, (*current_node)->data_ptr)) > 0 ){
            *current_node = (*current_node)->next;
    }
    return result;
}

/* Create a new item for the specified data pointer and put it in the container in order.
 If there is already an item in the container that compares equal to new item according to
 the comparison function, the insertion will not take place and 0 is returned to indicate failure.
 Otherwise, the insertion is done and non-zero is returned to show success.
 This function will not modify the pointed-to data.*/
int OC_insert(struct Ordered_container* c_ptr, const void* data_ptr) {
    if (c_ptr) {
        struct LL_Node* new_node = malloc_with_error_handling(sizeof(struct LL_Node));
        new_node->data_ptr = (void*) data_ptr;
        
        // if OC has zero item
        if (OC_empty(c_ptr)) {
            c_ptr->first = new_node;
            c_ptr->last = new_node;
            c_ptr->first->prev = c_ptr->first->next = NULL;
            c_ptr->last->prev = c_ptr->last->next = NULL;
            
            // if OC has > 0 item
        } else {
            struct LL_Node* current_node = c_ptr->first;
            struct LL_Node* prev_node = NULL;
            OC_equals_or_just_over(c_ptr, data_ptr, c_ptr->comp_func, &current_node);
            // if item is to be added as the first item on the list
            if (current_node && !current_node->prev) {
                current_node->prev = c_ptr->first = new_node;
                c_ptr->first->next = current_node;
                c_ptr->first->prev = NULL;
                // if item is to be added as the last item on the list
            } else if (!current_node) {
                prev_node = c_ptr->last;
                prev_node->next = c_ptr->last = new_node;
                c_ptr->last->prev = prev_node;
                c_ptr->last->next = NULL;
                // if item is to be added in the middle of the list
            } else {
                prev_node = current_node->prev;
                prev_node->next = new_node;
                new_node->prev = prev_node;
                new_node->next = current_node;
                current_node->prev = new_node;
            }
        }
        
        g_Container_items_in_use++;
        g_Container_items_allocated++;
        c_ptr->size++;
        return -1; // insert success
    }
    return 0;   // insert failed
}



/* Return a pointer to an item that points to data equal to the data object pointed to by data_ptr,
 using the ordering function to do the comparison with data_ptr as the first argument.
 The data_ptr object is assumed to be of the same type as the data objects pointed to by container items.
 NULL is returned if no matching item is found.
 The pointed-to data will not be modified. */
void* OC_find_item(const struct Ordered_container* c_ptr, const void* data_ptr) {
    if (c_ptr) {
        struct LL_Node* current_node = c_ptr->first;
        int found = OC_equals_or_just_over(c_ptr, data_ptr, c_ptr->comp_func, &current_node);
        
        return (!found) ? current_node : NULL;
    }
    return NULL;
}

/* typedef for a function used by OC_find_item_arg. The function returns negative, 0, or positive,
 if the data pointed to by arg_ptr corresponds to a data object that should come before, is equal to, or comes after,
 the data object pointed to by data_ptr. For example, arg_ptr could point to a datum with the same value
 as a member of the sought-for data object. */
typedef int (*OC_find_item_arg_fp_t) (const void* arg_ptr, const void* data_ptr);

/* Return a pointer to the item that points to data that matches the supplied argument given by arg_ptr
 according to the supplied function, which compares arg_ptr as the first argument with the data pointer
 in each item. This function does not require that arg_ptr be of the same type as the data objects, and
 so allows the container to be searched without creating a complete data object first.
 NULL is returned if no matching item is found. If more than one matching item is present, it is
 unspecified which one is returned. The comparison function must implement an ordering consistent
 with the ordering produced by the comparison function specified when the container was created;
 if not, the result is undefined. */
void* OC_find_item_arg(const struct Ordered_container* c_ptr, const void* arg_ptr, OC_find_item_arg_fp_t fafp) {
    if (c_ptr) {
        struct LL_Node* current_node = c_ptr->first;
        int found = OC_equals_or_just_over(c_ptr, arg_ptr, fafp, &current_node);
        
        return (!found) ? current_node : NULL;
    }
    return NULL;
}



/* Functions that traverse the items in the container, processing each item in order. */

/* Type of a function used by OC_apply.
 An apply function takes a data pointer as an argument, and returns void.
 It is allowed to modify the data object, but if the ordering information is changed,
 the effects of attempting to search the container afterwards with OC_find are undefined. */
typedef void (*OC_apply_fp_t) (void* data_ptr);

/* Apply the supplied function to the data pointer in each item of the container.
 The contents of the container cannot be modified. */
void OC_apply(const struct Ordered_container* c_ptr, OC_apply_fp_t afp) {
    if (c_ptr) {
        struct LL_Node* current_node = c_ptr->first;
        
        while(current_node) {
            afp(current_node->data_ptr);
            current_node = current_node->next;
        }
    }
}

/* Type of a function used by OC_apply_if.
 An apply_if function takes a data pointer as an argument, and returns zero or non-zero.
 It is allowed to modify the data object, but if the ordering information is changed,
 the effects of attempting to search the container afterwards with OC_find are undefined. */
typedef int (*OC_apply_if_fp_t) (void* data_ptr);

/* Apply the supplied function to the data pointer in each item in the container.
 If the function returns non-zero, the iteration is terminated, and that value
 returned. Otherwise, zero is returned. The contents of the container cannot be modified. */
int OC_apply_if(const struct Ordered_container* c_ptr, OC_apply_if_fp_t afp) {
    int result = -1;
    if (c_ptr) {
        struct LL_Node* current_node = c_ptr->first;
        while(current_node && ((result = afp(current_node->data_ptr) == 0))) {
            current_node = current_node->next;
        }
    }
    return result;
}

/* Type of a function used by OC_apply_arg.
 An OC_apply_arg function takes a data pointer as the first argument,
 a supplied argument pointer as a second argument, and returns void.
 It is allowed to modify the pointed-to argument, or the data object,
 but if the ordering information is changed, the effects of attempting to search
 the container afterwards with OC_find are undefined. */
typedef void (*OC_apply_arg_fp_t) (void* data_ptr, void* arg_ptr);

/* Apply the supplied function to the data pointer in each item in the container;
 the function takes a second argument, which is the supplied void pointer.
 The contents of the container cannot be modified. */
void OC_apply_arg(const struct Ordered_container* c_ptr, OC_apply_arg_fp_t afp, void* arg_ptr) {
    if (c_ptr) {
        struct LL_Node* current_node = c_ptr->first;
        
        while(current_node) {
            afp(current_node->data_ptr, arg_ptr);
            current_node = current_node->next;
        }
    }
}

/* Type of a function used by OC_apply_if_arg.
 An OC_apply_arg function takes a data pointer as an argument,
 a supplied argument pointer as a second argument, and returns an zero or non-zero.
 It is allowed to modify the pointed-to argument, or the data object,
 but if the ordering information is changed, the effects of attempting to search
 the container afterwards with OC_find are undefined. */
typedef int (*OC_apply_if_arg_fp_t) (void* data_ptr, void* arg_ptr);

/* Apply the supplied function to the data pointer in each item in the container;
 the function takes a second argument, which is the supplied void pointer.
 If the function returns non-zero, the iteration is terminated, and that value
 returned. Otherwise, zero is returned. The contents of the container cannot be modified */
int OC_apply_if_arg(const struct Ordered_container* c_ptr, OC_apply_if_arg_fp_t afp, void* arg_ptr) {
    int result = -1;
    if (c_ptr) {
        struct LL_Node* current_node = c_ptr->first;
        while(current_node && ((result = afp(current_node->data_ptr, arg_ptr) == 0))) {
            current_node = current_node->next;
        }
    }
    return result;
}


/* helper function */
void print_container(const struct Ordered_container* c_ptr);

void print_container(const struct Ordered_container* c_ptr) {
    struct LL_Node* current_node = c_ptr->first;
    while(current_node) {
        printf("%d\t", *(int*)(current_node->data_ptr));
        current_node = current_node->next;
    }
    printf("\n");
}
