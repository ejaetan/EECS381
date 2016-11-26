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
/* add functions */
void add_individual(struct Ordered_container* c_ptr);
void add_room(struct Ordered_container* c_ptr);
void add_meeting(struct Ordered_container* c_ptr);
void add_participant(struct Ordered_container* rm_ptr, struct Ordered_container* ppl_ptr);

/* print functions */
void print_group(struct Ordered_container* c_ptr);
void print_individual(struct Ordered_container* c_ptr);
void print_room_m(struct Ordered_container* c_ptr);
void print_meeting_m(struct Ordered_container* c_ptr);
void print_schedule(struct Ordered_container* c_ptr);
void print_allocation(struct Ordered_container* rm_ptr, struct Ordered_container* ppl_ptr);

/* delete functions */
void delete_individual(struct Ordered_container* c_ptr);
void delete_meeting(struct Ordered_container* c_ptr);
void delete_participant(struct Ordered_container* rm_ptr_c, struct Ordered_container* ppl_ptr_c);
void delete_room(struct Ordered_container* rm_ptr_c);

/* helper function protypes */
void skip_type_ahead(void);
void* rm_input_result(int scanf_result, int scan_input, struct Ordered_container* c_ptr);
int meeting_input_result(int scanf_result, int scan_input);
int cmp_person_lastname_arg(char *lastname, struct Person * person_ptr);
int cmp_room_num(const struct Room *rm_ptr1, const struct Room *rm_ptr2);
int cmp_room_num_arg(void* given_num, struct Room * room_ptr);


int main() {
    struct Ordered_container* people_list = OC_create_container((OC_comp_fp_t)cmp_person_lastname);
    struct Ordered_container* room_list = OC_create_container((OC_comp_fp_t) cmp_room_num);
    char command1, command2;

    while (1) {
        printf("\nEnter command: ");
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
                            delete_individual(people_list);
                            break;
                        case 'm':
                            delete_meeting(room_list);
                            break;
                        case 'r':
                            delete_room(room_list);
                            break;
                        case 'p':
                            delete_participant(room_list, people_list);
                            break;
                        default:
                            printf("Unrecognized command\n");
                            skip_type_ahead();
                            break;
                    }
                    break;
                
                case 'p':
                    switch (command2) {
                        case 'i':
                            print_individual(people_list);
                            break;
                        case 'g':
                            print_group(people_list);
                            break;
                        case 'r':
                            print_room_m(room_list);
                            break;
                        case 'm':
                            print_meeting_m(room_list);
                            break;
                        case 's':
                            print_schedule(room_list);
                            break;
                        case 'a':
                            print_allocation(room_list, people_list);
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
/* add functions */
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
            printf("Person %s added\n", lastname);
        } else {
            printf("There is already a person with this last name!\n");
        }
        
        
    }
    
}


void add_room(struct Ordered_container* c_ptr) {
    int room_num = -1;
    int scan_room_num = scanf(" %d", &room_num);
    void* found_rm_item_ptr = rm_input_result(scan_room_num, room_num, c_ptr);
    
    if(!found_rm_item_ptr) {
        struct Room *new_room = create_Room(room_num);
        OC_insert(c_ptr, new_room);
        printf("Room %d added\n", room_num);
    } else {
        printf("There is already a room with this number!\n");
    }
    
}

void add_meeting(struct Ordered_container* c_ptr) {
    int room_num = -1, meeting_time = -1;
    int scan_room_num = scanf("%d", &room_num);
    int scan_meeting_time = scanf("%d", &meeting_time);
    
    char topic[MAX_CHAR];
    int scan_topic = scanf(" %"STR(X)"s", topic);
    
    void* found_rm_item_ptr = rm_input_result(scan_room_num, room_num, c_ptr);
    if(found_rm_item_ptr) {
        struct Room * room = OC_get_data_ptr(found_rm_item_ptr);
        
        if(meeting_input_result(scan_meeting_time, meeting_time) && (scan_topic > 0) ){
            struct Meeting *new_meeting = create_Meeting(meeting_time, topic);
            if (!add_Room_Meeting(room, new_meeting)) {
                printf("Meeting added at %d\n", meeting_time);
            } else {
                printf("There is already a meeting at that time!\n");
            }
        }
    } else {
        printf("Room %d doesn't exist!\n", room_num);
    }
    
}

void add_participant(struct Ordered_container* rm_ptr, struct Ordered_container* ppl_ptr) {
    int room_num = -1, meeting_time = -1;
    int scan_room_num = scanf("%d", &room_num);
    int scan_meeting_time = scanf("%d", &meeting_time);
    
    char lastname[MAX_CHAR];
    int scan_lastname = scanf(" %"STR(X)"s", lastname);
    
    void* found_item_ptr = rm_input_result(scan_room_num, room_num, rm_ptr);
    
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

/* print functions */
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
    if (!OC_empty(c_ptr)) {
        printf("Information for %d people:\n", OC_get_size(c_ptr));
        OC_apply(c_ptr, (OC_apply_fp_t) print_Person);
    } else {
        printf("List of people is empty\n");
    }
    
}

void print_room_m(struct Ordered_container* c_ptr) {
    int room_num = -1;
    int scan_room_num = scanf(" %d", &room_num);
    void* found_rm_item_ptr = rm_input_result(scan_room_num, room_num, c_ptr);
    
    if(!found_rm_item_ptr) {
        printf("No room with that number!\n");
    } else {
        struct Room* room_ptr = OC_get_data_ptr(found_rm_item_ptr);
        print_Room(room_ptr);
    }
}

void print_meeting_m(struct Ordered_container* c_ptr) {
    int room_num = -1, meeting_time = -1;
    int scan_room_num = scanf("%d", &room_num);
    int scan_meeting_time = scanf("%d", &meeting_time);
    
    void* found_rm_item_ptr = rm_input_result(scan_room_num, room_num, c_ptr);
    
    if(!found_rm_item_ptr) {
        printf("No room with that number!\n");
    } else {
        struct Room* room = OC_get_data_ptr(found_rm_item_ptr);
        if(meeting_input_result(scan_meeting_time, meeting_time) ){
            struct Meeting* found_meeting = find_Room_Meeting(room, meeting_time);
            if (!found_meeting) {
                printf("No meeting at that time!\n");
            } else {
                print_Meeting(found_meeting);
            }
    
        }

    }
}

void print_schedule(struct Ordered_container* c_ptr) {
    if (!OC_empty(c_ptr)) {
        int rm_c_size = OC_get_size(c_ptr);
        printf("Information for %d rooms:\n", rm_c_size);
        OC_apply(c_ptr, (OC_apply_fp_t) print_Room);
    } else {
        printf("List of rooms is empty\n");
    }
}

void print_allocation(struct Ordered_container* rm_ptr, struct Ordered_container* ppl_ptr) {
    printf("Memory allocations:\n");
    printf("C-strings: %d bytes total\n", g_string_memory);
    printf("Person structs: %d\n", OC_get_size(ppl_ptr));
    printf("Meeting structs: %d\n", g_Meeting_memory);
    printf("Room structs: %d\n", OC_get_size(rm_ptr));
    printf("Containers: %d\n", g_Container_count);
    printf("Container items in use: %d\n", g_Container_items_in_use);
    printf("Container items allocated: %d\n", g_Container_items_allocated);
}


/* delete functions */
void delete_individual(struct Ordered_container* c_ptr) {
    char lastname[MAX_CHAR];
    
    int scan_lastname = scanf(" %"STR(X)"s", lastname);
    if (scan_lastname > 0) {
        void* found_item_ptr = OC_find_item_arg(c_ptr, lastname, (OC_find_item_arg_fp_t) cmp_person_lastname_arg);
        if (found_item_ptr) {
            struct Person* person = OC_get_data_ptr(found_item_ptr);
            printf("Person %s deleted\n", get_Person_lastname(person));
            destroy_Person(person);
            OC_delete_item(c_ptr, found_item_ptr);
            
        }
    }
    
}

void delete_meeting(struct Ordered_container* c_ptr) {
    int room_num = -1, meeting_time = -1;
    int scan_room_num = scanf("%d", &room_num);
    int scan_meeting_time = scanf("%d", &meeting_time);
    
    void* found_rm_item_ptr = rm_input_result(scan_room_num, room_num, c_ptr);
    
    if(!found_rm_item_ptr) {
        printf("No room with that number!\n");
    } else {
        struct Room* room_ptr = OC_get_data_ptr(found_rm_item_ptr);
        if(meeting_input_result(scan_meeting_time, meeting_time) ){
            struct Meeting* meeting_ptr = find_Room_Meeting(room_ptr, meeting_time);
            if (!meeting_ptr) {
                printf("No meeting at that time!\n");
            } else {
                remove_Room_Meeting(room_ptr, meeting_ptr);
                destroy_Meeting(meeting_ptr);
                printf("Meeting at %d deleted\n", meeting_time);
            }
            
        }
        
    }
}

void delete_participant(struct Ordered_container* rm_ptr_c, struct Ordered_container* ppl_ptr_c) {
    int room_num = -1, meeting_time = -1;
    int scan_room_num = scanf("%d", &room_num);
    int scan_meeting_time = scanf("%d", &meeting_time);
    
    char lastname[MAX_CHAR];
    int scan_lastname = scanf(" %"STR(X)"s", lastname);
    
    void* found_item_ptr = rm_input_result(scan_room_num, room_num, rm_ptr_c);
    
    if(found_item_ptr){
        struct Room * room = OC_get_data_ptr(found_item_ptr);
        
        if(meeting_input_result(scan_meeting_time, meeting_time) && (scan_lastname > 0) ){
            struct Meeting* found_meeting = find_Room_Meeting(room, meeting_time);
            if (found_meeting) {
                void* found_ppl_item_ptr = OC_find_item_arg(ppl_ptr_c, lastname, (OC_find_item_arg_fp_t) cmp_person_lastname_arg);
                if (!found_ppl_item_ptr) {
                    printf("No person with that name!\n");
                } else {
                    struct Person* found_person = OC_get_data_ptr(found_ppl_item_ptr);
                    if (remove_Meeting_participant(found_meeting, found_person)) {
                        printf("Participant %s deleted\n", lastname);
                    } else {
                        printf("This person is not a participant in the meeting!\n");
                    }
                }
                
            }
            
        }
    }
}

void delete_room(struct Ordered_container* rm_ptr_c) {
    int room_num = -1;
    int scan_room_num = scanf(" %d", &room_num);
    void* found_rm_item_ptr = rm_input_result(scan_room_num, room_num, rm_ptr_c);
    
    if(!found_rm_item_ptr) {
        printf("No room with that number!\n");
        return;
    }
    struct Room* room_ptr = OC_get_data_ptr(found_rm_item_ptr);
    destroy_Room(room_ptr);
    OC_delete_item(rm_ptr_c, found_rm_item_ptr);
}

/* helper function defintion */
void skip_type_ahead(void) {
    scanf("%*[^\n]");
}

void* rm_input_result(int scanf_result, int scan_input, struct Ordered_container* c_ptr) {
    if ( (scanf_result > 0) && (scan_input > 0) ) {
        void* found_item_ptr = OC_find_item_arg(c_ptr, &scan_input, (OC_find_item_arg_fp_t) cmp_room_num_arg);
        return found_item_ptr;
    }
    if ( (scanf_result > 0) && (scan_input < 0) ) {
        printf("Room number is not in range!\n");
        
    }
    if (!scanf_result) {
        printf("Could not read an integer value!\n");
    }
    skip_type_ahead();
    return NULL;
    
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
