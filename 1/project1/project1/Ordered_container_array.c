/* skeleton file for Ordered_container_array.c
The struct declaration below must be used for the Ordered_container object.
Remove this comment and complete this file with all necessary code.
*/

#include "Ordered_container.h"
#include "Utility.h"
#include <stdlib.h>

/* helper function */
void resize_array(struct Ordered_container* c_ptr);
int binsearch(struct Ordered_container* c_ptr, const void* data_ptr);

int g_Container_count = 0;		/* number of Ordered_containers currently allocated */
int g_Container_items_in_use = 0;	/* number of Ordered_container items currently in use */
int g_Container_items_allocated = 0;	/* number of Ordered_container items currently allocated */

/* A complete type declaration for Ordered_container implemented as an array */
struct Ordered_container {
	OC_comp_fp_t comp_fun;	/* pointer to comparison function  */
	void** array;			/* pointer to array of pointers to void */
	int allocation;			/* current size of array */
	int size;				/* number of items currently in the array */
};


/*
 Functions for the entire container.
 */

struct Ordered_container* OC_create_container(OC_comp_fp_t f_ptr) {
    struct Ordered_container *new_container = malloc_with_error_handling(sizeof(struct Ordered_container));;
    new_container->comp_fun = f_ptr;
    new_container->array = malloc_with_error_handling(sizeof(void*) * 3);
    new_container->allocation = 3;
    new_container->size = 0;
    
    g_Container_count++;
    g_Container_items_allocated += 3;
    return new_container;
}

/* Destroy the container and its items; caller is responsible for
 deleting all pointed-to data before calling this function.
 After this call, the container pointer value must not be used again. */
void OC_destroy_container(struct Ordered_container* c_ptr);

/* Delete all the items in the container and initialize it.
 Caller is responsible for deleting any pointed-to data first. */
void OC_clear(struct Ordered_container* c_ptr);


/* Return the number of items currently stored in the container */
int OC_get_size(const struct Ordered_container* c_ptr) {
    return c_ptr->size;
}

/* Return non-zero (true) if the container is empty, zero (false) if the container is non-empty */
int OC_empty(const struct Ordered_container* c_ptr) {
    return c_ptr->size ? -1: 0;
}

/*
 Functions for working with individual items in the container.
 */

/* Get the data object pointer from an item. */
void* OC_get_data_ptr(const void* item_ptr) {
    return *(void**)(item_ptr);
}

/* Delete the specified item.
 Caller is responsible for any deletion of the data pointed to by the item. */
void OC_delete_item(struct Ordered_container* c_ptr, void* item_ptr);




/*
 Functions that search and insert into the container using the supplied comparison function.
 */

/* Create a new item for the specified data pointer and put it in the container in order.
 If there is already an item in the container that compares equal to new item according to
 the comparison function, the insertion will not take place and 0 is returned to indicate failure.
 Otherwise, the insertion is done and non-zero is returned to show success.
 This function will not modify the pointed-to data. */
int OC_insert(struct Ordered_container* c_ptr, const void* data_ptr) {
    if (!c_ptr) {
        return 0;
    }
    
    
    int binsearch_result = binsearch(c_ptr, data_ptr);
    
    if (binsearch_result == -1) {
        return 0;
    }
    
    resize_array(c_ptr);
    
    for (int i = c_ptr->size ; i > binsearch_result; i--) {
        c_ptr->array[i] = c_ptr->array[i-1];
    }
    c_ptr->array[binsearch_result] = (void*)data_ptr;
    c_ptr->size++;
    g_Container_items_in_use++;
    return -1;
    
}



/* Return a pointer to an item that points to data equal to the data object pointed to by data_ptr,
 using the ordering function to do the comparison with data_ptr as the first argument.
 The data_ptr object is assumed to be of the same type as the data objects pointed to by container items.
 NULL is returned if no matching item is found.
 The pointed-to data will not be modified. */
void* OC_find_item(const struct Ordered_container* c_ptr, const void* data_ptr);

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
void* OC_find_item_arg(const struct Ordered_container* c_ptr, const void* arg_ptr, OC_find_item_arg_fp_t fafp);

/* Functions that traverse the items in the container, processing each item in order. */

/* Type of a function used by OC_apply.
 An apply function takes a data pointer as an argument, and returns void.
 It is allowed to modify the data object, but if the ordering information is changed,
 the effects of attempting to search the container afterwards with OC_find are undefined. */
typedef void (*OC_apply_fp_t) (void* data_ptr);

/* Apply the supplied function to the data pointer in each item of the container.
 The contents of the container cannot be modified. */
void OC_apply(const struct Ordered_container* c_ptr, OC_apply_fp_t afp);

/* Type of a function used by OC_apply_if.
 An apply_if function takes a data pointer as an argument, and returns zero or non-zero.
 It is allowed to modify the data object, but if the ordering information is changed,
 the effects of attempting to search the container afterwards with OC_find are undefined. */
typedef int (*OC_apply_if_fp_t) (void* data_ptr);

/* Apply the supplied function to the data pointer in each item in the container.
 If the function returns non-zero, the iteration is terminated, and that value
 returned. Otherwise, zero is returned. The contents of the container cannot be modified. */
int OC_apply_if(const struct Ordered_container* c_ptr, OC_apply_if_fp_t afp);

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
void OC_apply_arg(const struct Ordered_container* c_ptr, OC_apply_arg_fp_t afp, void* arg_ptr);

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
int OC_apply_if_arg(const struct Ordered_container* c_ptr, OC_apply_if_arg_fp_t afp, void* arg_ptr);

/* helper function */
void resize_array(struct Ordered_container* c_ptr) {
    if (c_ptr->size == c_ptr->allocation) {
        int ori_array_size = OC_get_size(c_ptr);
        int new_array_size = 2 * (ori_array_size + 1);
        void** new_array = malloc_with_error_handling(sizeof(void*) * new_array_size);
        for (int i = 0; i < ori_array_size; i++) {
            new_array[i] = c_ptr->array[i];
        }
        free(c_ptr->array);
        c_ptr->array = NULL;
        c_ptr->array = new_array;
        c_ptr->allocation = new_array_size;
        g_Container_items_allocated += (new_array_size - ori_array_size);
    }
}

int binsearch(struct Ordered_container* c_ptr, const void* data_ptr) {
    int low, high, mid;
    low = 0;
    high = c_ptr->size - 1;
    while(low <= high) {
        mid = (low + high) / 2;
        if (c_ptr->comp_fun(data_ptr, c_ptr->array[mid]) < 0) {
            high = mid - 1;
        } else if (c_ptr->comp_fun(data_ptr, c_ptr->array[mid]) > 0) {
            low = mid + 1;
        } else {
            return mid; // return match
        }
    }
    return low;  // no match
}


void print_container(const struct Ordered_container* c_ptr);

void print_container(const struct Ordered_container* c_ptr) {
    for (int i = 0; i < c_ptr->size; i++) {
        printf("%d\t", (int) c_ptr->array[i]);
    }
    printf("\n");
}
