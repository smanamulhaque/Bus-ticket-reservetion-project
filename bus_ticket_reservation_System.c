#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>  // For tolower()

#define MAX_USERS 100
#define MAX_TICKETS 100
#define MAX_STATIONS 8
#define USERS_FILE "users.txt"
#define RESERVATIONS_FILE "reservations.txt"

// User structure
typedef struct {
    char email[50];
    char password[30];
    char fullname[50];
} User;

// Ticket structure
typedef struct {
    char from[30];
    char to[30];
    char coach[10];
    char class[20];
    int seat_number;
    char date[15];
    int is_reserved;
    char reserved_by[50]; // Email of the user who reserved
} Ticket;

User users[MAX_USERS];
Ticket tickets[MAX_TICKETS];
int user_count = 0;
int ticket_count = 0;

// Predefined list of stations
const char *stations[] = {
    "Dhaka",
    "Chattogram",
    "Sylhet",
    "Rajshahi",
    "Khulna",
    "Barishal",
    "Rangpur",
    "Mymensingh"
};
const int station_count = sizeof(stations) / sizeof(stations[0]);

// Load users from file
void load_users() {
    FILE *file = fopen(USERS_FILE, "r");
    if (file == NULL) {
        printf("No users file found. Starting fresh.\n");
        return;
    }
    while (fscanf(file, "%s %s %[^\n]", users[user_count].email, users[user_count].password, users[user_count].fullname) != EOF) {
        user_count++;
    }
    fclose(file);
}

// Save a new user to the file
void save_user(User user) {
    FILE *file = fopen(USERS_FILE, "a");
    if (file == NULL) {
        printf("Error saving user data.\n");
        return;
    }
    fprintf(file, "%s %s %s\n", user.email, user.password, user.fullname);
    fclose(file);
}

// Save reservation details to file
void save_reservation(Ticket ticket) {
    FILE *file = fopen(RESERVATIONS_FILE, "a");
    if (file == NULL) {
        printf("Error saving reservation data.\n");
        return;
    }
    fprintf(file, "Reserved By: %s\nFrom: %s\nTo: %s\nClass: %s\nCoach: %s\nSeat: %d\nDate: %s\n\n",
            ticket.reserved_by, ticket.from, ticket.to, ticket.class, ticket.coach, ticket.seat_number, ticket.date);
    fclose(file);
}

// Validate email
int is_valid_email(const char *email) {
    return strchr(email, '@') != NULL;
}

// Register a new user
void register_user() {
    if (user_count >= MAX_USERS) {
        printf("User limit reached.\n");
        return;
    }

    char email[50], password[30], fullname[50];
    printf("Enter Gmail: ");
    while (1) {
        scanf("%s", email);
        if (is_valid_email(email)) {
            break;  // If email is valid, exit loop
        } else {
            printf("Invalid Gmail. Please enter a valid email containing '@': ");
        }
    }

    printf("Enter Password: ");
    scanf("%s", password);
    getchar(); // Consume newline left by scanf
    printf("Enter Full Name: ");
    fgets(fullname, sizeof(fullname), stdin);
    fullname[strcspn(fullname, "\n")] = '\0'; // Remove trailing newline

    // Check if the email already exists
    for (int i = 0; i < user_count; i++) {
        if (strcmp(users[i].email, email) == 0) {
            printf("Email already exists. Please use a different one.\n");
            return;
        }
    }

    strcpy(users[user_count].email, email);
    strcpy(users[user_count].password, password);
    strcpy(users[user_count].fullname, fullname);
    save_user(users[user_count]);
    user_count++;
    printf("User registered successfully.\n");
}

// Authenticate a user
int authenticate_user(char *logged_in_email) {
    char email[50], password[30];
    printf("Enter Gmail: ");
    while (1) {
        scanf("%s", email);
        if (is_valid_email(email)) {
            break;  // If email is valid, exit loop
        } else {
            printf("Invalid Gmail. Please enter a valid email containing '@': ");
        }
    }
    
    printf("Enter Password: ");
    scanf("%s", password);

    for (int i = 0; i < user_count; i++) {
        if (strcmp(users[i].email, email) == 0) {
            if (strcmp(users[i].password, password) == 0) {
                printf("Authentication successful.\n");
                strcpy(logged_in_email, email);
                return 1;
            } else {
                printf("Incorrect password.\n");
                return 0;
            }
        }
    }

    printf("Email not found.\n");
    return 0;
}

// Custom case-insensitive substring match
int case_insensitive_strstr(const char *haystack, const char *needle) {
    while (*haystack) {
        const char *h = haystack;
        const char *n = needle;
        
        while (*n && (tolower(*h) == tolower(*n))) {
            h++;
            n++;
        }

        if (!*n) {  // Found the needle
            return 1;
        }

        haystack++;
    }
    return 0;  // Not found
}

// Display available stations
void display_stations(const char *search) {
    printf("Matching Stations:\n");
    int found = 0;
    for (int i = 0; i < station_count; i++) {
        if (case_insensitive_strstr(stations[i], search)) {  // Use custom case-insensitive match
            printf("%d. %s\n", i + 1, stations[i]);
            found = 1;
        }
    }
    if (!found) {
        printf("No stations found matching '%s'.\n", search);
    }
}

// Check if the station is valid
int is_valid_station(const char *station) {
    for (int i = 0; i < station_count; i++) {
        if (case_insensitive_strstr(stations[i], station)) {  // Case-insensitive match
            return 1;
        }
    }
    return 0;
}

// Select station
void select_station(char *station, const char *label) {
    char input_station[30];
    
    printf("Select %s station (Type part of station name):\n", label);
    
    // Get the partial input from the user
    printf("Enter part of the station name: ");
    getchar();  // To consume any leftover newline character
    fgets(input_station, sizeof(input_station), stdin);
    input_station[strcspn(input_station, "\n")] = 0; // Remove trailing newline

    // Display matching stations based on user input
    display_stations(input_station);

    // Ask the user to select from the matching options
    printf("Enter the full station name exactly as listed: ");
    fgets(input_station, sizeof(input_station), stdin);
    input_station[strcspn(input_station, "\n")] = 0; // Remove trailing newline

    // Check if the station is valid
    if (is_valid_station(input_station)) {
        strcpy(station, input_station);
    } else {
        printf("Invalid station. Please try again.\n");
        select_station(station, label); // Retry on invalid input
    }
}

// Select class and coach
void select_class(char *class, char *coach) {
    int choice;
    printf("Select class:\n1. AC\n2. Non-AC\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);

    if (choice == 1) {
        strcpy(class, "AC");
        printf("Select coach (A-E): ");
        scanf("%s", coach);
        if (coach[0] < 'A' || coach[0] > 'E') {
            printf("Invalid coach. Please select again.\n");
            select_class(class, coach);
        }
    } else if (choice == 2) {
        strcpy(class, "Non-AC");
        printf("Select coach (F-M): ");
        scanf("%s", coach);
        if (coach[0] < 'F' || coach[0] > 'M') {
            printf("Invalid coach. Please select again.\n");
            select_class(class, coach);
        }
    } else {
        printf("Invalid choice. Please try again.\n");
        select_class(class, coach); // Retry on invalid input
    }
}

// Select seat number
void select_seat(int *seat_number) {
    printf("Enter seat number (1-40): ");
    scanf("%d", seat_number);
}

// Select journey date
void select_date(char *date) {
    printf("Enter journey date (DD-MM-YYYY): ");
    scanf("%s", date);
}

// Check seat availability
int check_seat_availability(const char *from, const char *to, int seat_number) {
    for (int i = 0; i < ticket_count; i++) {
        if (strcmp(tickets[i].from, from) == 0 && strcmp(tickets[i].to, to) == 0 &&
            tickets[i].seat_number == seat_number && tickets[i].is_reserved) {
            return 0;  // Seat already reserved
        }
    }
    return 1;  // Seat available
}

// Reserve a ticket
void reserve_ticket(const char *logged_in_email) {
    Ticket ticket;
    char from[30], to[30], class[20], coach[10], date[15];
    int seat_number;

    // Select from and to stations
    select_station(from, "departure");
    select_station(to, "destination");

    // Ensure the departure station is different from the destination station
    if (strcmp(from, to) == 0) {
        printf("Departure and destination cannot be the same. Please try again.\n");
        reserve_ticket(logged_in_email); // Retry
        return;
    }

    // Select class, coach, seat, and date
    select_class(class, coach);
    select_seat(&seat_number);
    select_date(date);

    // Check if seat is available
    if (!check_seat_availability(from, to, seat_number)) {
        printf("Sorry, seat number %d is already reserved. Please choose another seat.\n", seat_number);
        reserve_ticket(logged_in_email);  // Retry
        return;
    }

    // Save reservation
    strcpy(ticket.from, from);
    strcpy(ticket.to, to);
    strcpy(ticket.class, class);
    strcpy(ticket.coach, coach);
    ticket.seat_number = seat_number;
    strcpy(ticket.date, date);
    ticket.is_reserved = 1;
    strcpy(ticket.reserved_by, logged_in_email);

    tickets[ticket_count++] = ticket;
    save_reservation(ticket);
    printf("Ticket reserved successfully.\n");
}

int main() {
    int choice;
    char logged_in_email[50];

    load_users(); // Load existing users from file

    while (1) {
        printf("\nWelcome to the Ticket Reservation System\n");
        printf("1. Register\n2. Login\n3. Reserve Ticket\n4. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                register_user();
                break;
            case 2:
                if (authenticate_user(logged_in_email)) {
                    printf("Logged in as %s.\n", logged_in_email);
                }
                break;
            case 3:
                reserve_ticket(logged_in_email);
                break;
            case 4:
                printf("Exiting system.\n");
                return 0;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }

    return 0;
}
