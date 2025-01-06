#include<stdio.h>
#include<time.h>
#include<string.h>
#define MAX_EVENTS 100
#define EVENT_DESC_LENGTH 100

int getDaysInMonth(int month, int year);
int calculateDayOfWeek(int day, int month, int year);
void printMonthHeader(int month, int year);
int isLeapYear(int year);
void printCalendar(int month,int year,int highlightCurrent);

typedef struct {
    int day;
    int month;
    int year;
    char description[EVENT_DESC_LENGTH]; //array of characters to store the event's description, with a maximum length of EVENT_DESC_LENGTH
} Event;

Event events[MAX_EVENTS];  //array of Event structures to store the events and Event indicates that each element in the array will be of type Event
int eventCount = 0;


// Function to display the calendar of the entire year
void printYearCalendar(int year, int highlightCurrent) {
    for (int month = 1; month <= 12; month++) {
        printCalendar(month, year, highlightCurrent);
    }
}

//Function to print the calendar for a given month
void printCalendar(int month, int year, int highlightCurrent) {
    int days = getDaysInMonth(month, year);
    int firstDay = calculateDayOfWeek(1, month, year);  //calculate the day of the week for the 1st day of the month
    printMonthHeader(month, year);
    
    // Print spaces before the first day of the month
    for (int i = 0; i < firstDay; i++) {
        printf("   ");
    }
    
    // Print days of the month with highlights
    for (int day = 1; day <= days; day++) {

        // Highlight the current day
        if (highlightCurrent) {
            time_t t = time(NULL);
            struct tm tm = *localtime(&t);
            if (tm.tm_mday == day && tm.tm_mon + 1 == month && tm.tm_year + 1900 == year) {
                printf("[%2d]", day);  
            
            } else {
                printf("%2d ", day);}
        
        } 
        else {
            printf("%2d ", day);
        }

        if ((firstDay + day) % 7 == 0) {
            printf("\n"); // New line after Saturday
        }
    }
    printf("\n");
}

// Function to get the number of days in a month
int getDaysInMonth(int month, int year) {
    const int daysInMonth[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
    if (month == 2 && isLeapYear(year)) {
        return 29;
    }
    return daysInMonth[month - 1]; //index 0=january
}

// Function to calculate the day of the week for the 1st day of the month
int calculateDayOfWeek(int day, int month, int year) {
    if (month < 3) {
        month=month + 12;
        year--;
    }
    int K = year % 100;
    int J = year / 100;
    // Zeller's Congruence Formula     (0 = Saturday, 1 = Sunday,..., 6 = Friday)
    int h = (day + (13 * (month + 1)) / 5 + K + K / 4 + J / 4 + 5 * J) % 7;
    return (h+6)%7;
}

// Function to print the month header (Month Name and Year)
void printMonthHeader(int month, int year) {
    const char *months[] = {
        "January", "February", "March", "April", "May", "June",
        "July", "August", "September", "October", "November", "December"
    };
    printf("\n    %s %d\n", months[month - 1], year);
    printf("Su Mo Tu We Th Fr Sa\n");
}

// Function to check if a year is a leap year
int isLeapYear(int year) {
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}


// Function to calculate the number of days between two dates
int calculateDaysBetween(int day1, int month1, int year1, int day2, int month2, int year2) {
    struct tm startDate = {0}, endDate = {0};  //Declaration of struct tm Variables
    startDate.tm_mday = day1;
    startDate.tm_mon = month1 - 1;     //0-11 range
    startDate.tm_year = year1 - 1900;  // expects year as number of years since 1900
    endDate.tm_mday = day2;
    endDate.tm_mon = month2 - 1;   
    endDate.tm_year = year2 - 1900; 

    time_t t1 = mktime(&startDate);
    time_t t2 = mktime(&endDate);
    double seconds = difftime(t2, t1);
    
    return seconds / (60 * 60 * 24);
}

// Function to calculate the number of weeks between two dates
int calculateWeeksBetween(int day1, int month1, int year1, int day2, int month2, int year2) {
    int days = calculateDaysBetween(day1, month1, year1, day2, month2, year2);
    return days / 7;
}


// Function to add an event to the calendar
void addEvent(int day, int month, int year, const char* description) {

    if (eventCount < MAX_EVENTS) {
        events[eventCount].day = day;     //Accessing Structure Members in an Array: arrayName[index].memberName = value;
        events[eventCount].month = month;
        events[eventCount].year = year;
        strncpy(events[eventCount].description, description, EVENT_DESC_LENGTH); //strncpy(destination, source, max_length);
        eventCount++;
        printf("Event added successfully!\n");
    } else {
        printf("Event list is full! Cannot add more events.\n");
    }
}

// Function to display events for a given month and year
void displayEvents(int month, int year) {
    printf("Enter the month: ");
    scanf("%d", &month);
    printf("\nEvents in %d %d:\n", month, year);
    int found = 0;
    for (int i = 0; i < eventCount; i++) {
        if (events[i].month == month && events[i].year == year) {
            printf("%02d: %s\n", events[i].day, events[i].description);
            found = 1;
        }
    }
    if (!found) {
        printf("No events found for this month.\n");
    }
}
// Update printCalendar to highlight days with events
void printCalendarWithEvents(int month, int year, int highlightCurrent) {
    int days = getDaysInMonth(month, year);
    int firstDay = calculateDayOfWeek(1, month, year);
    printMonthHeader(month, year);
    
    // Print leading spaces for the first day of the month
    for (int i = 0; i < firstDay; i++) {
        printf("   ");
    }


    
    // Print days of the month with highlights
    for (int day = 1; day <= days; day++) {
        int isEventDay = 0;
        for (int i = 0; i < eventCount; i++) {
            if (events[i].day == day && events[i].month == month && events[i].year == year) {
                isEventDay = 1;
                break;
            }
        }
        
        if (highlightCurrent) {
            time_t t = time(NULL);
            struct tm tm = *localtime(&t);
            if (tm.tm_mday == day && tm.tm_mon + 1 == month && tm.tm_year + 1900 == year) {
                printf("[%2d]", day); // Highlight current day
            } else if (isEventDay) {
                printf("(%2d)", day); // Highlight event day
            } else {
                printf("%2d ", day);
            }
        } else if (isEventDay) {
            printf("(%2d)", day); // Highlight event day
        } else {
            printf("%2d ", day);
        }

        if ((firstDay + day) % 7 == 0) {
            printf("\n"); // New line after Saturday
        }
    }
    printf("\n");
}


int main() {
    int month, year, choice;
    printf("\n\n-------------------- Welcome -------------------\n\n");
    // Ask for input year and month
    printf("Enter the year: ");
    scanf("%d", &year);

    // Main menu
    do {
        printf("\nMenu:\n");
        printf("1. View Full Year Calendar\n");
        printf("2. View Specific Month Calendar\n");
        printf("3. Check Leap Year\n");
        printf("4. View Previous Month\n");
        printf("5. View Next Month\n");
        printf("6. Highlight Current Date\n");
        printf("7. Calculate Days Between Two Dates\n");
        printf("8. Calculate Weeks Between Two Dates\n");
        printf("9. Add an Event\n");
        printf("10. View Events for a Month\n");

        printf("0. Exit\n");
        
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printYearCalendar(year,1);
                break;

            case 2:
                printf("Enter the month (1-12): ");
                scanf("%d", &month);
                printCalendar(month, year, 1);
                break;

            case 3:
                if (isLeapYear(year)) {
                    printf("%d is a Leap Year.\n", year);
                } else {
                    printf("%d is not a Leap Year.\n", year);
                }
                break;

            case 4:
                if (month == 1) {
                    month = 12;
                    year--;
                } else {
                    month--;
                }
                printCalendar(month, year, 1);
                break;
            case 5:
                if (month == 12) {
                    month = 1;
                    year++;
                } else {
                    month++;
                }
                printCalendar(month, year, 1);
                break;
            case 6:
                printCalendar(month, year, 1);
                break;
            
            case 7: {
                int day1, month1, year1, day2, month2, year2;
                printf("Enter the first date (day month year): ");
                scanf("%d %d %d", &day1, &month1, &year1);
                printf("Enter the second date (day month year): ");
                scanf("%d %d %d", &day2, &month2, &year2);
                int daysBetween = calculateDaysBetween(day1, month1, year1, day2, month2, year2);
                printf("Days between the two dates: %d\n", daysBetween);
                break;
            }
            case 8: {
                int day1, month1, year1, day2, month2, year2;
                printf("Enter the first date (day month year): ");
                scanf("%d %d %d", &day1, &month1, &year1);
                printf("Enter the second date (day month year): ");
                scanf("%d %d %d", &day2, &month2, &year2);
                int weeksBetween = calculateWeeksBetween(day1, month1, year1, day2, month2, year2);
                printf("Weeks between the two dates: %d\n", weeksBetween);
                break;
            }

            case 9: {
            int day, eventMonth;
            char description[EVENT_DESC_LENGTH];
            printf("Enter the date (day month year): ");
            scanf("%d %d %d", &day, &eventMonth, &year);
            printf("Enter the event description: ");
            getchar(); // Clear the buffer
            fgets(description, EVENT_DESC_LENGTH, stdin); 
            description[strcspn(description, "\n")] = 0; // Remove newline character
            addEvent(day, eventMonth, year, description);
            break;
            }
            case 10:{
                displayEvents(month, year);
                break;}
            
            case 0:
                break;
            default:
                printf("Invalid choice! Please try again.\n");
        }
    } while (choice != 0);
            
return 0;
}