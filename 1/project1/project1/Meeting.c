#include "Ordered_container.h"
#include "Meeting.h"
#include "Person.h"
#include "Utility.h"
#include <stdlib.h>
/* a Meeting contains a time, a topic, and a container of participants */
struct Meeting {
	char* topic;
	int time;
	struct Ordered_container* participants;	/* a container of pointers to struct Person objects */
};

/* global variable defintion for this module */
int g_Meeting_memory = 0;			/* number of meeting structs currently allocated */

/* Helper function prototype */
int cmp_person_lastname(struct Person* person1, struct Person* person2);

/* Create a Meeting object.
 This is the only function that allocates memory for a Meeting
 and the contained data. The supplied time is assumed to be in 12-hr form. */
struct Meeting* create_Meeting(int time, const char* topic) {
    struct Meeting* new_meeting = (struct Meeting*) malloc_with_error_handling(sizeof(struct Meeting));
    new_meeting->participants = OC_create_container((OC_comp_fp_t) cmp_person_lastname);
    new_meeting->time = time;
    new_meeting->topic = (char*) malloc_with_error_handling(sizeof(strlen(topic) + 1));
    new_meeting->topic = strcpy(new_meeting->topic, topic);
    g_Meeting_memory++;
    g_string_memory += strlen(topic) + 1;
    return  new_meeting;
}


/* Destroy a Meeting object.
 This is the only function that frees the memory for a Meeting
 and the contained data. It discards the participant list,
 but of course does not delete the Persons themselves. */
void destroy_Meeting(struct Meeting* meeting_ptr) {
    g_Meeting_memory--;
    g_string_memory -= (strlen(meeting_ptr->topic) + 1);
    
    OC_destroy_container(meeting_ptr->participants);
    meeting_ptr->topic = NULL;
    free(meeting_ptr->topic);
    meeting_ptr = NULL;
    free(meeting_ptr);
    
}

/* Return and set the meeting time. The time value is assumed to be in 12-hr form.*/
int get_Meeting_time(const struct Meeting* meeting_ptr) {
    return meeting_ptr->time;
}
void set_Meeting_time(struct Meeting* meeting_ptr, int time) {
    meeting_ptr->time = time;
}

/* Add a participant; return non-zero and do nothing if already present. */
int add_Meeting_participant(struct Meeting* meeting_ptr, const struct Person* person_ptr) {
    void* found =  OC_find_item(meeting_ptr->participants, person_ptr);
    
    if (!found) {
        OC_insert(meeting_ptr->participants, person_ptr);
        return -1;
    }
    return 0;
}

/* Return non-zero if the person is a participant, zero if not. */
int is_Meeting_participant_present(const struct Meeting* meeting_ptr, const struct Person * person_ptr) {
    void* found =  OC_find_item(meeting_ptr->participants, person_ptr);
    
    return found ? -1: 0;
}

/* Remove a participant; return non-zero if not present, zero if was present. */
int remove_Meeting_participant(struct Meeting* meeting_ptr, const struct Person* person_ptr) {
    void* found =  OC_find_item(meeting_ptr->participants, person_ptr);
    
    if (found) {
        OC_delete_item(meeting_ptr->participants, (void*)person_ptr);
        return -1;
    }
    return 0;
}

/* Print the data in a struct Meeting. The time is expressed in 12-hr form with no AM/PM.*/
void print_Meeting(const struct Meeting* meeting_ptr) {
    if (meeting_ptr) {
        printf("Meeting time: %d, Topic: %s\n", meeting_ptr->time, meeting_ptr->topic);
        if (OC_empty(meeting_ptr->participants)) {
            printf("Participants: None\n");
        } else {
            printf("Participants:\n");
            OC_apply(meeting_ptr->participants, (OC_apply_fp_t) print_Person);
        }
    } else {
        printf("meeting not exist");
    }
    
    
}


/* Helper function */
int cmp_person_lastname(struct Person* person1, struct Person* person2) {
    return strcmp(get_Person_lastname(person1), get_Person_lastname(person2));
}


