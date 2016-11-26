#include "Ordered_container.h"
#include "Room.h"
#include "Meeting.h"
#include "Utility.h"
#include <stdlib.h>

/* Helper function prototype*/

int cmp_meeting_time(struct Meeting* meeting_ptr1, struct Meeting* meeting_ptr2);
int cmp_meeting_time_arg(int* given_time, struct Meeting* meeting_ptr);

/* a Room contains a container of meetings and a room number */
struct Room {
	struct Ordered_container* meetings;	/* a container of pointers to Meeting objects */
	int number;
};


/* Create a Room object.
 This is the only function that allocates memory for a Room
 and the contained data. */
struct Room* create_Room(int number) {
    struct Room* new_Room = malloc_with_error_handling(sizeof(struct Room));
    new_Room->meetings = OC_create_container((OC_comp_fp_t) cmp_meeting_time);
    new_Room->number = number;
    return new_Room;
}

/* Destroy a Room object.
 This is the only function that frees the memory for a Room
 and the contained data. The Meetings are destroyed before the
 Meeting list is emptied. */
void destroy_Room(struct Room* room_ptr) {
    clear_Room(room_ptr);
    free(room_ptr);
    room_ptr = NULL;
}

/* Return the room number. */
int get_Room_number(const struct Room* room_ptr) {
    return room_ptr->number;
}

/* Add the meeting to the room, return non-zero if a meeting already at that time, 0 if OK. */
int add_Room_Meeting(struct Room* room_ptr, const struct Meeting* meeting_ptr) {
    void *found_item_ptr = OC_find_item(room_ptr->meetings, meeting_ptr);
    
    if (!found_item_ptr) {
        OC_insert(room_ptr->meetings, meeting_ptr);
        return 0;
    }
    return -1;
}

/* Return a pointer to the meeting at the specified time, NULL if not present. */
struct Meeting* find_Room_Meeting(const struct Room* room_ptr, int time) {
    void *found_item_ptr = OC_find_item_arg(room_ptr->meetings, &time, (OC_find_item_arg_fp_t) cmp_meeting_time_arg);
    return found_item_ptr ? (struct Meeting*)OC_get_data_ptr(found_item_ptr) : NULL;
}

/* Remove the supplied meeting from the room; return non-zero if not there; 0 if OK.
 The meeting is not destroyed because we may need to place it into another room. */
int remove_Room_Meeting(struct Room* room_ptr, const struct Meeting* meeting_ptr) {
    void *found_item_ptr = OC_find_item(room_ptr->meetings, meeting_ptr);
    
    if (found_item_ptr) {
        OC_delete_item(room_ptr->meetings, found_item_ptr);
        return 0;
    }
    return -1;
}

/* Clear and destroy the Meetings in a Room.
 This function destroys each meeting and leaves the Room empty of meetings. */
void clear_Room(struct Room* room_ptr) {
    OC_apply(room_ptr->meetings, (OC_apply_fp_t) destroy_Meeting);
    OC_clear(room_ptr->meetings);
    OC_destroy_container(room_ptr->meetings);
}

/* Return a pointer to the const container of meetings for read-only iterations over the meetings. */
const struct Ordered_container* get_Room_Meetings(const struct Room* room_ptr);

/* Print the data in a struct Room. */
void print_Room(const struct Room* room_ptr) {
    printf("--- Room %d ---\n", room_ptr->number);
    if (!OC_empty(room_ptr->meetings)) {
        OC_apply(room_ptr->meetings, (OC_apply_fp_t) print_Meeting);
    } else {
        printf("No meetings are scheduled\n");
    }
    
    
}

/* Helper function */


int cmp_meeting_time(struct Meeting* meeting_ptr1, struct Meeting* meeting_ptr2) {
    return convert_time(get_Meeting_time(meeting_ptr1)) - convert_time(get_Meeting_time(meeting_ptr2));
}

int cmp_meeting_time_arg(int* given_time, struct Meeting* meeting_ptr) {
    return convert_time(*(int*)given_time) - convert_time(get_Meeting_time(meeting_ptr));
}
