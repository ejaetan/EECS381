//
//  p1_main.c
//  project1
//
//  Created by Yi Jie Tan on 11/20/16.
//  Copyright © 2016 eecs381. All rights reserved.
//

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "Ordered_container.h"
#include "Room.h"
#include "Meeting.h"
#include "Utility.h"

#define MAX_CHAR 64
#define MAX_INPUT 63
#define STR2(x)
#define STR(X) STR2(X)

/* function prototypes */
void add_individual(struct Ordered_container* c_ptr);
void add_room(struct Ordered_container* c_ptr);
void add_meeting(struct Ordered_container* c_ptr);
void add_participant(struct Ordered_container* rm_ptr, struct Ordered_container* ppl_ptr);

void print_group(struct Ordered_container* c_ptr);
void print_individual(struct Ordered_container* c_ptr);
//void delete_individual(struct Ordered_container* c_ptr);

/* helper function protypes */
void skip_type_ahead(void);
int rm_input_result(int scanf_result, int scan_input);
int meeting_input_result(int scanf_result, int scan_input);
int cmp_person_lastname_arg(char *lastname, struct Person * person_ptr);
int cmp_room_num(const struct Room *rm_ptr1, const struct Room *rm_ptr2);
int cmp_room_num_arg(void* given_num, struct Room * room_ptr);


int main() {
    struct Ordered_container* people_list = OC_create_container((OC_comp_fp_t)cmp_person_lastname);
    struct Ordered_container* room_list = OC_create_container((OC_comp_fp_t) cmp_room_num);
    char command1, command2;
    
    while (1) {
        int c1_result = scanf(" %c", &command1);
        int c2_result = scanf(" %c", &command2);
        
        if (c1_result > 0 && c2_result > 0) {
            switch (command1) {
                case 'a':
                    switch (command2) {
                        case 'i':
                            add_individual(people_list);
                            break;
                        case'r':
                            add_room(room_list);
                            break;
                        case 'm':
                            add_meeting(room_list);
                            break;
                        case 'p':
                            add_participant(room_list, people_list);
                            break;
                        default:
                            printf("Unrecognized command\n");
                            skip_type_ahead();
                            break;  // default break for command2
                    }
                    break;  //break for command1 'a'
                case 'd':
                    switch (command2) {
                        case 'i':
                            //delete_individual(people_list);
                            break;
                            
                        default:
                            printf("Unrecognized command\n");
                            skip_type_ahead();
                            break;
                    }
                
                case 'p':
                    switch (command2) {
                        case 'i':
                            print_individual(people_list);
                            break;
                        case 'g':
                            print_group(people_list);
                            break;
                            
                        default:
                            skip_type_ahead();
                            break;
                    }
                    break;  // break for command1 'p'
                default:
                    printf("Unrecognized command\n");
                    skip_type_ahead();
                    break;      // default break for command1
            }
            
            
            
        }
    }
    return 0;
}

/* function definition */
void add_individual(struct Ordered_container* c_ptr) {
    char firstname[MAX_CHAR], lastname[MAX_CHAR], phoneno[MAX_CHAR];
    
    
    int scan_firstname = scanf(" %"STR(X)"s", firstname);
    int scan_lastname = scanf(" %"STR(X)"s", lastname);
    int scan_phoneno = scanf(" %"STR(X)"s", phoneno);
    
    if (scan_firstname > 0 && scan_lastname > 0 && scan_phoneno) {
        struct Person *new_person = create_Person(firstname, lastname, phoneno);
        
        void* found_item_ptr = OC_find_item_arg(c_ptr, lastname, (OC_find_item_arg_fp_t) cmp_person_lastname_arg);
        if (!found_item_ptr) {
            OC_insert(c_ptr, new_person);
        } else {
            printf("There is already a person with this last name!\n");
        }
        
        
    }
    
}


void add_room(struct Ordered_container* c_ptr) {
    int room_num = -1;
    int scan_room_num = scanf(" %d", &room_num);
    
    if (rm_input_result(scan_room_num, room_num)) {
        void* found_item_ptr = OC_find_item_arg(c_ptr, &room_num, (OC_find_item_arg_fp_t) cmp_room_num_arg);
        if(!found_item_ptr) {
            struct Room *new_room = create_Room(room_num);
            OC_insert(c_ptr, new_room);
        } else {
            printf("There is already a room with this number!\n");
        }
    }
}

void add_meeting(struct Ordered_container* c_ptr) {
    int room_num = -1, meeting_time = -1;
    int scan_room_num = scanf("%d", &room_num);
    int scan_meeting_time = scanf("%d", &meeting_time);
    
    char topic[MAX_CHAR];
    int scan_topic = scanf(" %"STR(X)"s", topic);

    if (rm_input_result(scan_room_num, room_num)) {
        void* found_item_ptr = OC_find_item_arg(c_ptr, &room_num, (OC_find_item_arg_fp_t) cmp_room_num_arg);
        if(found_item_ptr) {
            struct Room * room = OC_get_data_ptr(found_item_ptr);
            
            if(meeting_input_result(scan_meeting_time, meeting_time) && (scan_topic > 0) ){
                struct Meeting *new_meeting = create_Meeting(meeting_time, topic);
                if (!add_Room_Meeting(room, new_meeting)) {
                    printf("Meeting added at %d\n", meeting_time);
                }
            }
        } else {
            printf("Room %d doesn't exist!\n", room_num);
        }
    }
}

void add_participant(struct Ordered_container* rm_ptr, struct Ordered_container* ppl_ptr) {
    int room_num = -1, meeting_time = -1;
    int scan_room_num = scanf("%d", &room_num);
    int scan_meeting_time = scanf("%d", &meeting_time);
    
    char lastname[MAX_CHAR];
    int scan_lastname = scanf(" %"STR(X)"s", lastname);
    
    if (!rm_input_result(scan_room_num, room_num)) {
        printf("Room %d doesn't exist!\n", room_num);
    } else {
        void* found_item_ptr = OC_find_item_arg(rm_ptr, &room_num, (OC_find_item_arg_fp_t) cmp_room_num_arg);
        if(found_item_ptr){
            struct Room * room = OC_get_data_ptr(found_item_ptr);
            if(meeting_input_result(scan_meeting_time, meeting_time) && (scan_lastname > 0) ){
                struct Meeting* found_meeting = find_Room_Meeting(room, meeting_time);
                if (found_meeting) {
                    void* found_ppl_item_ptr = OC_find_item_arg(ppl_ptr, lastname, (OC_find_item_arg_fp_t) cmp_person_lastname_arg);
                    if (!found_ppl_item_ptr) {
                        printf("No person with that name!\n");
                    } else {
                        struct Person* found_person = OC_get_data_ptr(found_ppl_item_ptr);
                        if (is_Meeting_participant_present(found_meeting, found_person)) {
                            printf("This person is already a participant!\n");
                        } else {
                            add_Meeting_participant(found_meeting, found_person);
                            printf("Participant %s added\n", lastname);
                        }
                    }
                    
                }
                
            }
        }

    }
    
}

void print_individual(struct Ordered_container* c_ptr) {
    char lastname[MAX_CHAR];
    
    int scan_lastname = scanf(" %"STR(X)"s", lastname);
    if (scan_lastname > 0) {
        void* found_item_ptr = OC_find_item_arg(c_ptr, lastname, (OC_find_item_arg_fp_t) cmp_person_lastname_arg);
        if (found_item_ptr) {
            print_Person(OC_get_data_ptr(found_item_ptr));
        } else {
            printf("No person with that name!\n");
        }
    }
}

void print_group(struct Ordered_container* c_ptr) {
    OC_apply(c_ptr, (OC_apply_fp_t) print_Person);
}

/*
void delete_individual(struct Ordered_container* c_ptr) {
    char lastname[MAX_CHAR];
    
    int scan_lastname = scanf(" %"STR(X)"s", lastname);
    if (scan_lastname > 0) {
        void* found_item_ptr = OC_find_item_arg(c_ptr, lastname, (OC_find_item_arg_fp_t) cmp_person_lastname_arg);
        if (found_item_ptr) {
 
        }
    }
    
}
 */

/* helper function defintion */
void skip_type_ahead(void) {
    scanf("%*[^\n]");
}

int rm_input_result(int scanf_result, int scan_input) {
    if ( (scanf_result > 0) && (scan_input > 0) ) {
        skip_type_ahead();
        return 1;
    }
    if ( (scanf_result > 0) && (scan_input < 0) ) {
        printf("Room number is not in range!\n");
        skip_type_ahead();
        return 0;
    }
    if (!scanf_result) {
        printf("Could not read an integer value!\n");
        skip_type_ahead();
        return 0;
    }
    return 0;
    
}

int meeting_input_result(int scanf_result, int scan_input) {
    if (scanf_result > 0 &&
        ( (convert_time(scan_input) < 9) || convert_time(scan_input) > 17) ) {
        printf("Time is not in range!\n");
        skip_type_ahead();
        return 0;
    }
    if (!scanf_result) {
        printf("Could not read an integer value!\n");
        skip_type_ahead();
        return 0;
    }
    return 1;
}



int cmp_person_lastname_arg(char *lastname, struct Person * person_ptr) {
    return strcmp(lastname, get_Person_lastname(person_ptr));
}

int cmp_room_num(const struct Room *rm_ptr1, const struct Room *rm_ptr2) {
    return get_Room_number(rm_ptr1) - get_Room_number(rm_ptr2);
}

int cmp_room_num_arg(void* given_num, struct Room * room_ptr) {
    return *(int*)given_num - get_Room_number(room_ptr);
}
