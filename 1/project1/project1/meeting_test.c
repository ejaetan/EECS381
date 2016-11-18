//
//  meeting_test.c
//  project1
//
//  Created by Yi Jie Tan on 11/18/16.
//  Copyright © 2016 eecs381. All rights reserved.
//

#include <stdio.h>
#include "Ordered_container.h"
#include "Person.h"
#include "Utility.h"
#include "Meeting.h"
#include <stdlib.h>

int main() {
    struct Meeting* meeting1 = create_Meeting(12, "Hello");
    struct Person* person1 = create_Person("Eugene", "Goh", "734");
    struct Person* person2 = create_Person("Yi", "Tan", "123");
    struct Person* person3 = create_Person("Ali", "Bo", "123");
    struct Person* person4 = create_Person("Fake", "person", "000");
    add_Meeting_participant(meeting1, person1);
    add_Meeting_participant(meeting1, person2);
    add_Meeting_participant(meeting1, person3);
    print_Meeting(meeting1);
    
    printf("%d\n", is_Meeting_participant_present(meeting1, person4));
    printf("%d\n", is_Meeting_participant_present(meeting1, person3));
    
    printf("%d\n", get_Meeting_time(meeting1));
    set_Meeting_time(meeting1, 1);
    printf("%d\n", get_Meeting_time(meeting1));
    
    
    remove_Meeting_participant(meeting1, person1);
    print_Meeting(meeting1);
    
    destroy_Meeting(meeting1);
    print_Meeting(meeting1);
}

