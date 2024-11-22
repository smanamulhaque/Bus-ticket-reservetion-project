#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_USERS 100
#define MAX_TICKETS 100
#define MAX_COACHES 5
#define MAX_SEATS 40
#define USERS_FILE "users.txt"

typedef struct {
    char email[50];
    char username[30];
    char password[30];
} User;

typedef struct {
    char from[30];
    char to[30];
    char coach[10];
    int seat_number;
    char date[15];
    int is_reserved;
} Ticket;

User users[MAX_USERS];
Ticket tickets[MAX_TICKETS];
int user_count = 0;
int ticket_count = 0;

// Load users from file
void load_users() {
    FILE *file = fopen(USERS_FILE, "r");
    if (file == NULL) {
        printf("No users file found. Starting fresh.\n");
        return;
    }
    while (fscanf(file, "%s %s %s", users[user_count].email, users[user_count].username, users[user_count].password) != EOF) {
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
    fprintf(file, "%s %s %s\n", user.email, user.username, user.password);
    fclose(file);
}

void register_user() {
    if (user_count >= MAX_USERS) {
        printf("User limit reached.\n");
        return;
    }

    char email[50], username[30], password[30];
    printf("Enter email: ");
    scanf("%s", email);
    printf("Enter username: ");
    scanf("%s", username);
    printf("Enter password: ");
    scanf("%s", password);

    // Check if the email or username already exists
    for (int i = 0; i < user_count; i++) {
        if (strcmp(users[i].email, email) == 0) {
            printf("Email already exists. Please use a different one.\n");
            return;
        }
        if (strcmp(users[i].username, username) == 0) {
            printf("Username already exists. Please choose a different one.\n");
            return;
        }
    }

    strcpy(users[user_count].email, email);
    strcpy(users[user_count].username, username);
    strcpy(users[user_count].password, password);
    save_user(users[user_count]);
    user_count++;
    printf("User registered successfully.\n");
}

int authenticate_user() {
    char identifier[50], password[30];
    printf("Enter email or username: ");
    scanf("%s", identifier);
    printf("Enter password: ");
    scanf("%s", password);

    for (int i = 0; i < user_count; i++) {
        if ((strcmp(users[i].email, identifier) == 0 || strcmp(users[i].username, identifier) == 0) &&
            strcmp(users[i].password, password) == 0) {
            printf("Authentication successful.\n");
            return 1;
        }
    }
    printf("Authentication failed. Please check your email/username and password.\n");
    return 0;
}

void select_station(char *station) {
    printf("Enter station: ");
    scanf("%s", station);
}

void select_coach(char *coach) {
    printf("Enter coach (A-E): ");
    scanf("%s", coach);
}

void select_seat(int *seat_number) {
    printf("Enter seat number (1-40): ");
    scanf("%d", seat_number);
}

void select_date(char *date) {
    printf("Enter date (DD-MM-YYYY): ");
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

void make_reservation() {
    if (ticket_count >= MAX_TICKETS) {
        printf("No more tickets can be reserved.\n");
        return;
    }

    Ticket ticket;
    select_station(ticket.from);
    select_station(ticket.to);
    select_coach(ticket.coach);
    select_seat(&ticket.seat_number);
    select_date(ticket.date);

    if (check_seat_availability(ticket.date, ticket.coach, ticket.seat_number)) {
        ticket.is_reserved = 1;
        tickets[ticket_count++] = ticket;
        printf("Reservation successful.\n");
    } else {
        printf("Seat not available.\n");
    }
}

void make_payment() {
    printf("Payment successful.\n");
}

void confirm_ticket() {
    make_reservation();
    make_payment();
    printf("Ticket confirmed.\n");
}

int main() {
    load_users(); // Load existing users from file

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
                if (authenticate_user()) {
                    printf("Logged in successfully.\n");
                }
                break;
            case 3:
                if (authenticate_user()) {
                    confirm_ticket();
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

