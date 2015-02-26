#include <stdio.h>
#include <stdbool.h>

// How many sections in a day
#define DAY_SECTIONS 6

#define COUNT(X) (sizeof(X) / sizeof(X[0]))

// schedule table, row is section, column is week-day
int schedule[DAY_SECTIONS][5] = {
    { 0, 2, 0, 2, 0 },
    { 1, 4, 1, 2, 1 },
    { 5, 4, 5, 0, 5 },
    { 0, 4, 0, 0, 0 },
    { 3, 0, 3, 0, 3 },
    { 0, 0, 0, 0, 0 }
};

// course name talbe
const char *course_names[] = {
    "No Courses",
    "Chinese",
    "Computer",
    "Database",
    "Experiment",
    "Data structure"
};

// week name table
const char *week_names[] = {
    "Monday",
    "Tuesday",
    "Wednesday",
    "Thursday",
    "Friday"
};

// print courses' id
void print_course_id()
{
    int i;
    puts("Course code is shown following:");
    for (i = 1; i < COUNT(course_names); i++) {
        printf("%-4d %s\n", i, course_names[i]);
    }
}

// print weekdays' id
void print_weekday_id()
{
    int i;
    puts("Weekday code is shown following:");
    for (i = 0; i < COUNT(week_names); i++) {
        printf("%-4d %s\n", i, week_names[i]);
    }
}

// query schedule by course id
// and output {weekday-section}
void lookup_course_by_id(int course_id)
{
    int day, n;

    printf("You are querying course: %s\n", course_names[course_id]);

    for (day = 0; day < COUNT(week_names); day++) {
        for (n = 0; n < DAY_SECTIONS; n++) {
            if (schedule[n][day] == course_id) {
                printf("%s - %d\n", week_names[day], n + 1);
            }
        }
    }
}

int main()
{
    int query_type, course_id, week_id, section_id;
    while (true) {

        // there is two way to query schedule
        // read input to choose which way to use
        puts("");
        puts("Input 0 to exit");
        printf("Which query type? (1: course code to time, 2: time to course name): ");
        scanf("%d", &query_type);

        if (query_type == 0) { // if input is zero, we just terminate the program
            break;
        } else if (query_type == 1) { // 1st way to query, input course code and print time
            print_course_id();
            printf("Input course code to query: ");
            scanf("%d", &course_id);
            if (course_id == 0)
                break;
            lookup_course_by_id(course_id);
        } else if (query_type == 2) { // 2nd way to query, input time and print course
            print_weekday_id();
            printf("Input weekdays: ");
            scanf("%d", &week_id);
            if (week_id < 0 || week_id >= COUNT(week_names)) {
                puts("Wrong week id!!");
                continue;
            }
            printf("Input section id (1~%d): ", DAY_SECTIONS);
            scanf("%d", &section_id);
            if (section_id < 1 || section_id > DAY_SECTIONS) {
                puts("Wrong section id!!");
                continue;
            }
            course_id = schedule[section_id - 1][week_id];
            printf("Course at %s - %d is %s\n", week_names[week_id], section_id,
                    course_names[course_id]);
        } else {
            puts("Wrong query type!!");
            continue;
        }
    }

    puts("Bye~");

    return 0;
}
