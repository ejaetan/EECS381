//
//  room_test.c
//  project1
//
//  Created by Yi Jie Tan on 11/18/16.
//  Copyright © 2016 eecs381. All rights reserved.
//

#include <stdio.h>
#include "Ordered_container.h"
#include "Room.h"
#include "Meeting.h"
#include "Person.h"
#include "Utility.h"

int main() {
    struct Room* room1 = create_Room(1001);
    struct Meeting* meeting1 = create_Meeting(12, "Hello");
    struct Meeting* meeting2 = create_Meeting(2, "Bye");
    struct Person* person1 = create_Person("Eugene", "Goh", "734");
    struct Person* person2 = create_Person("Yi", "Tan", "123");
    struct Person* person3 = create_Person("Ali", "Bo", "123");

    add_Meeting_participant(meeting1, person1);
    add_Meeting_participant(meeting1, person2);
    add_Meeting_participant(meeting2, person3);
    
    printf("%d\n", add_Room_Meeting(room1, meeting1));
    printf("%d\n", get_Room_number(room1));
    
    struct Meeting * found_meeting = find_Room_Meeting(room1, 12);
    printf("%d\n", get_Meeting_time(found_meeting));
}
