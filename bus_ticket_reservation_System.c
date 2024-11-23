#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

// Display available stations
void display_stations(const char *search) {
    printf("Matching Stations:\n");
    int found = 0;
    for (int i = 0; i < station_count; i++) {
        if (strcasestr(stations[i], search)) {  // Case-insensitive partial match
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
        if (strcasestr(stations[i], station)) {  // Case-insensitive match
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
int check_seat_availability(char *date, char *coach, int seat_number) {
    for (int i = 0; i < ticket_count; i++) {
        if (strcmp(tickets[i].date, date) == 0 && strcmp(tickets[i].coach, coach) == 0 && tickets[i].seat_number == seat_number) {
            return 0; // Seat is already reserved
        }
    }
    return 1; // Seat is available
}

// Make a reservation
void make_reservation(char *logged_in_email) {
    if (ticket_count >= MAX_TICKETS) {
        printf("No more tickets can be reserved.\n");
        return;
    }

    Ticket ticket;
    strcpy(ticket.reserved_by, logged_in_email);
    select_station(ticket.from, "From");
    select_station(ticket.to, "To");
    select_class(ticket.class, ticket.coach);
    select_seat(&ticket.seat_number);
    select_date(ticket.date);

    if (check_seat_availability(ticket.date, ticket.coach, ticket.seat_number)) {
        tickets[ticket_count] = ticket;
        ticket_count++;
        save_reservation(ticket);
        printf("Reservation successful!\n");
    } else {
        printf("Sorry, this seat is already reserved. Please try again.\n");
        make_reservation(logged_in_email);  // Retry if the seat is taken
    }
}

// Main menu
void menu(char *logged_in_email) {
    int choice;
    while (1) {
        printf("\nMain Menu:\n");
        printf("1. Make a Reservation\n");
        printf("2. Logout\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        if (choice == 1) {
            make_reservation(logged_in_email);
        } else if (choice == 2) {
            printf("Logging out...\n");
            break;
        } else {
            printf("Invalid choice, please try again.\n");
        }
    }
}

int main() {
    load_users();

    char logged_in_email[50];

    printf("Welcome to the Train Reservation System\n");
    int choice;

    printf("1. Register\n2. Login\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);

    if (choice == 1) {
        register_user();
    }

    if (authenticate_user(logged_in_email)) {
        menu(logged_in_email);
    }

    return 0;
}
