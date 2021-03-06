//
//  p1_main.c
//  project1
//
//  Created by Yi Jie Tan on 11/20/16.
//  Copyright © 2016 eecs381. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "Ordered_container.h"
#include "Room.h"
#include "Meeting.h"
#include "Utility.h"



/* function prototypes */
/* add functions */
void add_individual(struct Ordered_container* ppl_ptr_c);
void add_room(struct Ordered_container* rm_ptr_c);
void add_meeting(struct Ordered_container* rm_ptr_c);
void add_participant(struct Ordered_container* rm_ptr_c, struct Ordered_container* ppl_ptr_c);

/* print functions */
void print_group(struct Ordered_container* ppl_ptr_c);
void print_individual(struct Ordered_container* ppl_ptr_c);
void print_room_m(struct Ordered_container* rm_ptr_c);
void print_meeting_m(struct Ordered_container* rm_ptr_c);
void print_schedule(struct Ordered_container* rm_ptr_c);
void print_allocation(struct Ordered_container* rm_ptr_c, struct Ordered_container* ppl_ptr_c);

/* delete functions */
void delete_individual(struct Ordered_container* rm_ptr_c, struct Ordered_container* ppl_ptr_c);
void delete_meeting(struct Ordered_container* rm_ptr_c);
void delete_participant(struct Ordered_container* rm_ptr_c, struct Ordered_container* ppl_ptr_c);
void delete_room(struct Ordered_container* rm_ptr_c);
void delete_schedule(struct Ordered_container* rm_ptr_c);
void delete_group(struct Ordered_container* rm_ptr_c, struct Ordered_container* ppl_ptr_c);
void delete_all(struct Ordered_container* rm_ptr_c, struct Ordered_container* ppl_ptr_c);

/* quit program function */
void quit_program(struct Ordered_container* rm_ptr_c, struct Ordered_container* ppl_ptr_c);

/* reschedule meeting function */
void reschedule_meeting(struct Ordered_container* rm_ptr_c);

/* save data function */
void save_data(struct Ordered_container* rm_ptr_c, struct Ordered_container* ppl_ptr_c);

/* load data function */
void load_data(struct Ordered_container* rm_ptr_c, struct Ordered_container* ppl_ptr_c);

/* helper function protypes */
void skip_type_ahead(void);
int verify_rm_num(int scanf_result, int scan_input);
int meeting_input_result(int scanf_result, int scan_input);
int cmp_room_num(const struct Room *rm_ptr1, const struct Room *rm_ptr2);
int cmp_room_num_arg(void* given_num, struct Room * room_ptr);
int loop_into_meeting_di(void* data_ptr, void* arg_ptr);
int loop_into_meeting_dg(void* data_ptr);

/* error message */
void unrecognized_cmd_error(void);

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
                            unrecognized_cmd_error();
                            break;  // default break for command2
                    }
                    break;  //break for command1 'a'
                case 'd':
                    switch (command2) {
                        case 'i':
                            delete_individual(room_list, people_list);
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
                        case 's':
                            delete_schedule(room_list);
                            break;
                        case 'g':
                            delete_group(room_list, people_list);
                            break;
                        case 'a':
                            delete_all(room_list, people_list);
                            break;
                        default:
                            unrecognized_cmd_error();
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
                            unrecognized_cmd_error();
                            break;
                    }
                    break;  // break for command1 'p'
                case 'q':
                    switch (command2) {
                        case 'q':
                            quit_program(room_list, people_list);
                            return EXIT_SUCCESS;
                            break;
                        default:
                            unrecognized_cmd_error();
                            break;
                    }
                    break; // break for command1 'q'
                case 'r':
                    switch (command2) {
                        case 'm':
                            reschedule_meeting(room_list);
                            break;
                        default:
                            unrecognized_cmd_error();
                            break;
                    }
                    break; // break for command1 'r'
                case 's':
                    switch (command2) {
                        case 'd':
                            save_data(room_list, people_list);
                            break;
                        default:
                            unrecognized_cmd_error();
                            break;
                    }
                    break; // break for command1 's'
                case 'l':
                    switch (command2) {
                        case 'd':
                            load_data(room_list, people_list);
                            break;
                        default:
                            unrecognized_cmd_error();
                            break;
                    }
                    break; // break for command1 'd'
                default:
                    unrecognized_cmd_error();
                    break;      // default break for command1
            }
        }
    }
    return 0;
}

/* function definition */
/* add functions */
void add_individual(struct Ordered_container* ppl_ptr_c) {
    char firstname[MAX_CHAR], lastname[MAX_CHAR], phoneno[MAX_CHAR];
    
    
    int scan_firstname = scanf(" %"STR(X)"s", firstname);
    int scan_lastname = scanf(" %"STR(X)"s", lastname);
    int scan_phoneno = scanf(" %"STR(X)"s", phoneno);
    
    if (scan_firstname > 0 && scan_lastname > 0 && scan_phoneno) {
        struct Person *new_person = create_Person(firstname, lastname, phoneno);
        
        void* found_item_ptr = OC_find_item_arg(ppl_ptr_c, lastname, (OC_find_item_arg_fp_t) cmp_person_lastname_arg);
        if (found_item_ptr) {
            printf("There is already a person with this last name!\n");
            destroy_Person(new_person);
            return;
        }
        OC_insert(ppl_ptr_c, new_person);
        printf("Person %s added\n", lastname);
    }
    
}

void add_room(struct Ordered_container* rm_ptr_c) {
    int room_num = -1;
    int scan_room_num = scanf(" %d", &room_num);
    
    int rm_num_istrue = verify_rm_num(scan_room_num, room_num);
    
    if (!rm_num_istrue) {
        return;
    }
    
    void* found_rm_item_ptr = OC_find_item_arg(rm_ptr_c, &room_num, (OC_find_item_arg_fp_t) cmp_room_num_arg);
    
    if(!found_rm_item_ptr) {
        struct Room *new_room = create_Room(room_num);
        OC_insert(rm_ptr_c, new_room);
        printf("Room %d added\n", room_num);
        return;
    }
    printf("There is already a room with this number!\n");
    skip_type_ahead();
    
    
}

void add_meeting(struct Ordered_container* rm_ptr_c) {
    int room_num = -1, meeting_time = -1;
    int scan_room_num = scanf("%d", &room_num);
    int scan_meeting_time = scanf("%d", &meeting_time);
    
    char topic[MAX_CHAR];
    scanf(" %"STR(X)"s", topic);
    
    int rm_num_istrue = verify_rm_num(scan_room_num, room_num);
    
    if (!rm_num_istrue) {
        return;
    }
    
    void* found_rm_item_ptr = OC_find_item_arg(rm_ptr_c, &room_num, (OC_find_item_arg_fp_t) cmp_room_num_arg);
    
    if (!found_rm_item_ptr) {
        printf("No room with that number!\n");
        return;
    }
    
    struct Room * room = OC_get_data_ptr(found_rm_item_ptr);
    
    
    int meeting_istrue = meeting_input_result(scan_meeting_time, meeting_time);
    
    if (!meeting_istrue) {
        return;
    }
    
    struct Meeting *new_meeting = find_Room_Meeting(room, meeting_time);
    if (new_meeting) {
        printf("There is already a meeting at that time!\n");
        return;
    }
    
    new_meeting = create_Meeting(meeting_time, topic);
    if (!add_Room_Meeting(room, new_meeting)) {
        printf("Meeting added at %d\n", meeting_time);
    }

}

void add_participant(struct Ordered_container* rm_ptr_c, struct Ordered_container* ppl_ptr_c) {
    int room_num = -1, meeting_time = -1;
    int scan_room_num = scanf("%d", &room_num);
    int scan_meeting_time = scanf("%d", &meeting_time);
    
    char lastname[MAX_CHAR];
    scanf(" %"STR(X)"s", lastname);
    
    int rm_num_istrue = verify_rm_num(scan_room_num, room_num);
    
    if (!rm_num_istrue) {
        return;
    }
    
    void* found_rm_item_ptr = OC_find_item_arg(rm_ptr_c, &room_num, (OC_find_item_arg_fp_t) cmp_room_num_arg);
    
    if(!found_rm_item_ptr){
        return;
    }
    
    struct Room * room = OC_get_data_ptr(found_rm_item_ptr);
    if (!meeting_input_result(scan_meeting_time, meeting_time)) {
        return;
    }
    
    
    struct Meeting* found_meeting = find_Room_Meeting(room, meeting_time);
    if (!found_meeting) {
        return;
    }
    
    void* found_ppl_item_ptr = OC_find_item_arg(ppl_ptr_c, lastname, (OC_find_item_arg_fp_t) cmp_person_lastname_arg);
    if (!found_ppl_item_ptr) {
        printf("No person with that name!\n");
        return;
    }
    
    struct Person* found_person = OC_get_data_ptr(found_ppl_item_ptr);
    if (is_Meeting_participant_present(found_meeting, found_person)) {
        printf("This person is already a participant!\n");
    } else {
        add_Meeting_participant(found_meeting, found_person);
        printf("Participant %s added\n", lastname);
    }
    
        
    
}

/* print functions */
void print_individual(struct Ordered_container* ppl_ptr_c) {
    char lastname[MAX_CHAR];
    
    scanf(" %"STR(X)"s", lastname);
    void* found_item_ptr = OC_find_item_arg(ppl_ptr_c, lastname, (OC_find_item_arg_fp_t) cmp_person_lastname_arg);
    if (found_item_ptr) {
        print_Person(OC_get_data_ptr(found_item_ptr));
    } else {
        printf("No person with that name!\n");
    }

}

void print_group(struct Ordered_container* ppl_ptr_c) {
    if (!OC_empty(ppl_ptr_c)) {
        printf("Information for %d people:\n", OC_get_size(ppl_ptr_c));
        OC_apply(ppl_ptr_c, (OC_apply_fp_t) print_Person);
    } else {
        printf("List of people is empty\n");
    }
    
}

void print_room_m(struct Ordered_container* rm_ptr_c) {
    int room_num = -1;
    int scan_room_num = scanf(" %d", &room_num);
    int rm_num_istrue = verify_rm_num(scan_room_num, room_num);
    
    if (!rm_num_istrue) {
        return;
    }
    
    void* found_rm_item_ptr = OC_find_item_arg(rm_ptr_c, &room_num, (OC_find_item_arg_fp_t) cmp_room_num_arg);
    
    if(!found_rm_item_ptr) {
        printf("No room with that number!\n");
    } else {
        struct Room* room_ptr = OC_get_data_ptr(found_rm_item_ptr);
        print_Room(room_ptr);
    }
}

void print_meeting_m(struct Ordered_container* rm_ptr_c) {
    int room_num = -1, meeting_time = -1;
    int scan_room_num = scanf("%d", &room_num);
    int scan_meeting_time = scanf("%d", &meeting_time);
    
    int rm_num_istrue = verify_rm_num(scan_room_num, room_num);
    
    if (!rm_num_istrue) {
        return;
    }
    
    void* found_rm_item_ptr = OC_find_item_arg(rm_ptr_c, &room_num, (OC_find_item_arg_fp_t) cmp_room_num_arg);
    
    if(!found_rm_item_ptr) {
        printf("No room with that number!\n");
        return;
    }
    
    struct Room* room = OC_get_data_ptr(found_rm_item_ptr);
    if(!meeting_input_result(scan_meeting_time, meeting_time) ){
        printf("There is already a meeting at that time!\n");
        return;
    }
    
    struct Meeting* found_meeting = find_Room_Meeting(room, meeting_time);
    if (!found_meeting) {
        printf("No meeting at that time!\n");
    } else {
        print_Meeting(found_meeting);
    }
}

void print_schedule(struct Ordered_container* rm_ptr_c) {
    if (!OC_empty(rm_ptr_c)) {
        int rm_c_size = OC_get_size(rm_ptr_c);
        printf("Information for %d rooms:\n", rm_c_size);
        OC_apply(rm_ptr_c, (OC_apply_fp_t) print_Room);
    } else {
        printf("List of rooms is empty\n");
    }
}

void print_allocation(struct Ordered_container* rm_ptr_c, struct Ordered_container* ppl_ptr_c) {
    printf("Memory allocations:\n");
    printf("C-strings: %d bytes total\n", g_string_memory);
    printf("Person structs: %d\n", OC_get_size(ppl_ptr_c));
    printf("Meeting structs: %d\n", g_Meeting_memory);
    printf("Room structs: %d\n", OC_get_size(rm_ptr_c));
    printf("Containers: %d\n", g_Container_count);
    printf("Container items in use: %d\n", g_Container_items_in_use);
    printf("Container items allocated: %d\n", g_Container_items_allocated);
}


/* delete functions */
void delete_individual(struct Ordered_container* rm_ptr_c, struct Ordered_container* ppl_ptr_c) {
    char lastname[MAX_CHAR];
    
    scanf(" %"STR(X)"s", lastname);
    
    void* found_name_item_ptr = OC_find_item_arg(ppl_ptr_c, lastname, (OC_find_item_arg_fp_t) cmp_person_lastname_arg);
    if (!found_name_item_ptr) {
        return;
    }
    struct Person* person_ptr = OC_get_data_ptr(found_name_item_ptr);

    if(!OC_apply_if_arg(rm_ptr_c, (OC_apply_if_arg_fp_t) loop_into_meeting_di, person_ptr)) {
        printf("Person %s deleted\n", get_Person_lastname(person_ptr));
        destroy_Person(person_ptr);
        OC_delete_item(ppl_ptr_c, found_name_item_ptr);
    } else {
        printf("This person is a participant in a meeting!\n");
    }
    
}

void delete_meeting(struct Ordered_container* rm_ptr_c) {
    int room_num = -1, meeting_time = -1;
    int scan_room_num = scanf("%d", &room_num);
    int scan_meeting_time = scanf("%d", &meeting_time);
    
    int rm_num_istrue = verify_rm_num(scan_room_num, room_num);
    
    if (!rm_num_istrue) {
        return;
    }
    
    void* found_rm_item_ptr = OC_find_item_arg(rm_ptr_c, &room_num, (OC_find_item_arg_fp_t) cmp_room_num_arg);
    
    if(!found_rm_item_ptr) {
        printf("No room with that number!\n");
        return;
    }
    
    struct Room* room_ptr = OC_get_data_ptr(found_rm_item_ptr);
    if(!meeting_input_result(scan_meeting_time, meeting_time) ){
        return;
    }
    
    struct Meeting* meeting_ptr = find_Room_Meeting(room_ptr, meeting_time);
    if (!meeting_ptr) {
        printf("No meeting at that time!\n");
    } else {
        remove_Room_Meeting(room_ptr, meeting_ptr);
        destroy_Meeting(meeting_ptr);
        printf("Meeting at %d deleted\n", meeting_time);
    }
    
}

void delete_participant(struct Ordered_container* rm_ptr_c, struct Ordered_container* ppl_ptr_c) {
    int room_num = -1, meeting_time = -1;
    int scan_room_num = scanf("%d", &room_num);
    int scan_meeting_time = scanf("%d", &meeting_time);
    
    char lastname[MAX_CHAR];
    scanf(" %"STR(X)"s", lastname);
    
    int rm_num_istrue = verify_rm_num(scan_room_num, room_num);
    
    if (!rm_num_istrue) {
        return;
    }
    
    void* found_rm_item_ptr = OC_find_item_arg(rm_ptr_c, &room_num, (OC_find_item_arg_fp_t) cmp_room_num_arg);
    
    if(!found_rm_item_ptr){
        return;
    }
    struct Room * room = OC_get_data_ptr(found_rm_item_ptr);
    if(!meeting_input_result(scan_meeting_time, meeting_time)){
        return;
    }
    struct Meeting* found_meeting = find_Room_Meeting(room, meeting_time);
    if (!found_meeting) {
        return;
    }
    void* found_ppl_item_ptr = OC_find_item_arg(ppl_ptr_c, lastname, (OC_find_item_arg_fp_t) cmp_person_lastname_arg);
    if (!found_ppl_item_ptr) {
        printf("No person with that name!\n");
        return;
    }
    struct Person* found_person = OC_get_data_ptr(found_ppl_item_ptr);
    if (remove_Meeting_participant(found_meeting, found_person)) {
        printf("Participant %s deleted\n", lastname);
    } else {
        printf("This person is not a participant in the meeting!\n");
    }
}

void delete_room(struct Ordered_container* rm_ptr_c) {
    int room_num = -1;
    int scan_room_num = scanf(" %d", &room_num);
    int rm_num_istrue = verify_rm_num(scan_room_num, room_num);
    
    if (!rm_num_istrue) {
        return;
    }
    
    void* found_rm_item_ptr = OC_find_item_arg(rm_ptr_c, &room_num, (OC_find_item_arg_fp_t) cmp_room_num_arg);
    
    if(!found_rm_item_ptr) {
        printf("No room with that number!\n");
        return;
    }
    struct Room* room_ptr = OC_get_data_ptr(found_rm_item_ptr);
    destroy_Room(room_ptr);
    OC_delete_item(rm_ptr_c, found_rm_item_ptr);
    printf("Room %d deleted\n", room_num);
}

void delete_schedule(struct Ordered_container* rm_ptr_c) {
    OC_apply(rm_ptr_c, (OC_apply_fp_t) clear_Room);
    printf("All meetings deleted\n");
}


void delete_group(struct Ordered_container* rm_ptr_c, struct Ordered_container* ppl_ptr_c) {
    int has_meeting = OC_apply_if(rm_ptr_c, (OC_apply_if_fp_t) loop_into_meeting_dg);
    if (has_meeting > 0) {
        printf("Cannot clear people list unless there are no meetings!\n");
        return;
    }
    OC_apply(ppl_ptr_c, (OC_apply_fp_t) destroy_Person);
    OC_clear(ppl_ptr_c);
    printf("All persons deleted\n");
}

void delete_all(struct Ordered_container* rm_ptr_c, struct Ordered_container* ppl_ptr_c) {
    OC_apply(rm_ptr_c, (OC_apply_fp_t) destroy_Room);
    OC_clear(rm_ptr_c);
    printf("All rooms and meetings deleted\n");
    OC_apply(ppl_ptr_c, (OC_apply_fp_t) destroy_Person);
    OC_clear(ppl_ptr_c);
    printf("All persons deleted\n");
}

/* quit program function */
void quit_program(struct Ordered_container* rm_ptr_c, struct Ordered_container* ppl_ptr_c) {
    delete_all(rm_ptr_c, ppl_ptr_c);
    OC_destroy_container(rm_ptr_c);
    OC_destroy_container(ppl_ptr_c);
    printf("Done\n");
}

/* reschedule meeting function */
void reschedule_meeting(struct Ordered_container* rm_ptr_c) {
    int room_num1 = -1, meeting_time1 = -1;
    int scan_room_num1 = scanf("%d", &room_num1);
    int scan_meeting_time1 = scanf("%d", &meeting_time1);
    int rm_num_istrue1 = verify_rm_num(scan_room_num1, room_num1);
    
    if (!rm_num_istrue1) {
        return;
    }
    
    void* found_rm_item_ptr1 = OC_find_item_arg(rm_ptr_c, &room_num1, (OC_find_item_arg_fp_t) cmp_room_num_arg);
    
    int room_num2 = -1, meeting_time2 = -1;
    int scan_room_num2 = scanf("%d", &room_num2);
    int scan_meeting_time2 = scanf("%d", &meeting_time2);
    
    int rm_num_istrue2 = verify_rm_num(scan_room_num2, room_num2);
    
    if (!rm_num_istrue2) {
        return;
    }
    
    void* found_rm_item_ptr2 = OC_find_item_arg(rm_ptr_c, &room_num2, (OC_find_item_arg_fp_t) cmp_room_num_arg);
    
    if(!found_rm_item_ptr1) {
        printf("No room %d!\n", room_num1);
        skip_type_ahead();
        return;
    }
    
    struct Room* room_ptr1 = OC_get_data_ptr(found_rm_item_ptr1);
    struct Room* room_ptr2 = OC_get_data_ptr(found_rm_item_ptr2);
    if(!meeting_input_result(scan_meeting_time1, meeting_time1) ||
       !meeting_input_result(scan_meeting_time2, meeting_time2)){
        return;
    }
    
    struct Meeting* meeting_ptr1 = find_Room_Meeting(room_ptr1, meeting_time1);
    if (!meeting_ptr1) {
        printf("No meeting at %d!\n", meeting_time1);
        skip_type_ahead();
        return;
    }
    
    if(!found_rm_item_ptr2) {
        printf("No room %d!\n", room_num2);
        skip_type_ahead();
        return;
    }
    
    struct Meeting* meeting_ptr2 = find_Room_Meeting(room_ptr2, meeting_time2);
    if (meeting_ptr2) {
        printf("There is already a meeting at that %d!", meeting_time2);
        skip_type_ahead();
        return;
    }
    
    meeting_ptr2 = meeting_ptr1;
    remove_Room_Meeting(room_ptr1, meeting_ptr1);
    set_Meeting_time(meeting_ptr2, meeting_time2);
    add_Room_Meeting(room_ptr2, meeting_ptr2);
    
    printf("Meeting rescheduled to room %d at %d\n", room_num2, meeting_time2);
    
}

/* save data function */
void save_data(struct Ordered_container* rm_ptr_c, struct Ordered_container* ppl_c) {
    char filename[MAX_CHAR];
    scanf(" %"STR(X)"s", filename);
    FILE * fp = fopen(filename, "w");
    if (!fp ){
        fprintf(stderr, "Can't open file");
    }
    fprintf(fp, "%d\n", OC_get_size(ppl_c));
    OC_apply_arg(ppl_c, (OC_apply_arg_fp_t) save_Person, fp);
    fprintf(fp, "%d\n", OC_get_size(rm_ptr_c));
    OC_apply_arg(rm_ptr_c, (OC_apply_arg_fp_t) save_Room, fp);
    printf("Data saved\n");
    fclose(fp);
}

/* load data function */
void load_data(struct Ordered_container* rm_ptr_c, struct Ordered_container* ppl_ptr_c) {
    OC_apply(rm_ptr_c, (OC_apply_fp_t) destroy_Room);
    OC_clear(rm_ptr_c);
    OC_apply(ppl_ptr_c, (OC_apply_fp_t) destroy_Person);
    OC_clear(ppl_ptr_c);
    
    char filename[MAX_CHAR];
    scanf(" %"STR(X)"s", filename);
    FILE *fp = fopen(filename, "r");
    
    if (!fp ){
        fprintf(stderr, "Could not open file!\n");
        return;
    }
    
    int total_person = 0;
    int scan = fscanf(fp, "%d\n", &total_person);
    
    if (!scan) {
        printf("Invalid data found in file!\n");
        return;
    }
    
    for (; total_person > 0; total_person--) {
        struct Person* new_person = load_Person(fp);
        if (!new_person) {
            return;
        }
        OC_insert(ppl_ptr_c, new_person);
    }
    
    int total_rooms = 0;
    scan = fscanf(fp, "%d\n", &total_rooms);
    
    if (!scan) {
        printf("Invalid data found in file!\n");
        return;
    }

    for (; total_rooms; total_rooms--) {
        struct Room* new_room = load_Room(fp, ppl_ptr_c);
        OC_insert(rm_ptr_c, new_room);
    }
    printf("Data loaded\n");
    fclose(fp);
}

/* helper function defintion */
void skip_type_ahead(void) {
    scanf("%*[^\n]");
}

int verify_rm_num(int scanf_result, int scan_input) {
    int result = 0;
    
    if ((scanf_result > 0) && (scan_input > 0)) {
        return -1;
    }
    
    if ( (scanf_result > 0) && (scan_input < 0) ) {
        printf("Room number is not in range!\n");
    }
    if (!scanf_result) {
        printf("Could not read an integer value!\n");
    }
    skip_type_ahead();
    return result;
    
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


int cmp_room_num(const struct Room *rm_ptr1, const struct Room *rm_ptr2) {
    return get_Room_number(rm_ptr1) - get_Room_number(rm_ptr2);
}

int cmp_room_num_arg(void* given_num, struct Room * room_ptr) {
    return *(int*)given_num - get_Room_number(room_ptr);
}

int loop_into_meeting_di(void* data_ptr, void* arg_ptr) {
    const struct Ordered_container* meeting_ptr_c = get_Room_Meetings((const struct Room*)data_ptr);
    return OC_apply_if_arg(meeting_ptr_c, (OC_apply_if_arg_fp_t) is_Meeting_participant_present, arg_ptr);
}

int loop_into_meeting_dg(void* data_ptr) {
    const struct Ordered_container* meeting_ptr_c = get_Room_Meetings((const struct Room*)data_ptr);
    return OC_empty(meeting_ptr_c);
}

/* error message */
void unrecognized_cmd_error(void) {
    printf("Unrecognized command!\n");
    skip_type_ahead();
}
