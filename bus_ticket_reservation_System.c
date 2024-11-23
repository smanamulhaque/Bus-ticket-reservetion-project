#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_USERS 100
#define MAX_TICKETS 100
#define USERS_FILE "users.txt"
#define RESERVATIONS_FILE "reservations.txt"

typedef struct {
    char email[50];
    char password[30];
    char fullname[50];
} User;

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

void register_user() {
    if (user_count >= MAX_USERS) {
        printf("User limit reached.\n");
        return;
    }

    char email[50], password[30], fullname[50];
    printf("Enter Gmail: ");
    scanf("%s", email);
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

int authenticate_user(char *logged_in_email) {
    char email[50], password[30];
    printf("Enter Gmail: ");
    scanf("%s", email);
    printf("Enter Password: ");
    scanf("%s", password);

    for (int i = 0; i < user_count; i++) {
        if (strcmp(users[i].email, email) == 0 && strcmp(users[i].password, password) == 0) {
            printf("Authentication successful.\n");
            strcpy(logged_in_email, email);
            return 1;
        }
    }
    printf("Authentication failed. Please check your Gmail and password.\n");
    return 0;
}

void display_stations() {
    printf("Available Stations:\n");
    for (int i = 0; i < station_count; i++) {
        printf("%d. %s\n", i + 1, stations[i]);
    }
}

void select_station(char *station, const char *label) {
    display_stations();
    int choice;
    printf("Select %s station (Enter number): ", label);
    scanf("%d", &choice);
    if (choice >= 1 && choice <= station_count) {
        strcpy(station, stations[choice - 1]);
    } else {
        printf("Invalid choice. Please try again.\n");
        select_station(station, label); // Retry on invalid input
    }
}

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

void select_seat(int *seat_number) {
    printf("Enter seat number (1-40): ");
    scanf("%d", seat_number);
}

void select_date(char *date) {
    printf("Enter journey date (DD-MM-YYYY): ");
    scanf("%s", date);
}

int check_seat_availability(char *date, char *coach, int seat_number) {
    for (int i = 0; i < ticket_count; i++) {
        if (strcmp(tickets[i].date, date) == 0 && strcmp(tickets[i].coach, coach) == 0 && tickets[i].seat_number == seat_number) {
            return 0; // Seat is already reserved
        }
    }
    return 1; // Seat is available
}

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
        ticket.is_reserved = 1;
        tickets[ticket_count++] = ticket;
        save_reservation(ticket);
        printf("\nReservation successful!\n");
        printf("Details:\nFrom: %s\nTo: %s\nClass: %s\nCoach: %s\nSeat Number: %d\nDate: %s\n",
               ticket.from, ticket.to, ticket.class, ticket.coach, ticket.seat_number, ticket.date);
    } else {
        printf("Seat not available.\n");
    }
}

int main() {
    load_users(); // Load existing users from file

    char logged_in_email[50];
    int choice;
    while (1) {
        printf("\n1. Register\n2. Login\n3. Reserve Ticket\n4. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                register_user();
                break;
            case 2:
                if (authenticate_user(logged_in_email)) {
                    printf("Logged in successfully.\n");
                }
                break;
            case 3:
                if (authenticate_user(logged_in_email)) {
                    make_reservation(logged_in_email);
                }
                break;
            case 4:
                exit(0);
            default:
                printf("Invalid choice.\n");
        }
    }
    return 0;
}
