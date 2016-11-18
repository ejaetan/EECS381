#include "Ordered_container.h"
#include "Room.h"
#include "Meeting.h"
#include "Utility.h"

/* Helper function prototype*/
int convert_time(int a, int b);
int cmp_meeting_time(int a, int b);

/* a Room contains a container of meetings and a room number */
struct Room {
	struct Ordered_container* meetings;	/* a container of pointers to Meeting objects */
	int number;
};


/* Create a Room object.
 This is the only function that allocates memory for a Room
 and the contained data. */
struct Room* create_Room(int number) {
    struct Room* new_Room = (struct Room*)malloc_with_error_handling(sizeof(struct Room));
    new_Room->meetings = OC_create_container((OC_comp_fp_t) cmp_meeting_time);
    new_Room->number = number;
    return new_Room;
}

/* Destroy a Room object.
 This is the only function that frees the memory for a Room
 and the contained data. The Meetings are destroyed before the
 Meeting list is emptied. */
void destroy_Room(struct Room* room_ptr);

/* Return the room number. */
int get_Room_number(const struct Room* room_ptr) {
    return room_ptr->number;
}

/* Add the meeting to the room, return non-zero if a meeting already at that time, 0 if OK. */
int add_Room_Meeting(struct Room* room_ptr, const struct Meeting* meeting_ptr);

/* Return a pointer to the meeting at the specified time, NULL if not present. */
struct Meeting* find_Room_Meeting(const struct Room* room_ptr, int time);

/* Remove the supplied meeting from the room; return non-zero if not there; 0 if OK.
 The meeting is not destroyed because we may need to place it into another room. */
int remove_Room_Meeting(struct Room* room_ptr, const struct Meeting* meeting_ptr);

/* Clear and destroy the Meetings in a Room.
 This function destroys each meeting and leaves the Room empty of meetings. */
void clear_Room(struct Room* room_ptr);

/* Return a pointer to the const container of meetings for read-only iterations over the meetings. */
const struct Ordered_container* get_Room_Meetings(const struct Room* room_ptr);

/* Print the data in a struct Room. */
void print_Room(const struct Room* room_ptr);

/* Helper function */
int convert_time(int a, int b) ;

int cmp_meeting_time(int a, int b) {
    if (a > b) return 1;
    if (a < b) return -1;
    return 0;
}
