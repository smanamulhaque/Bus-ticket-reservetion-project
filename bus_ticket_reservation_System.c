#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_USERS 1144
#define MAX_TICKETS 1144
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
    char cclass[20];
    int seat_number;
    char date[15];
    int is_reserved;
    char reserved_by[50]; 
    char fullname[50];   // Added full name for reservation
} Ticket;

User users[MAX_USERS];
Ticket tickets[MAX_TICKETS];
int user_count = 0;
int ticket_count = 0;

// Predefined list of stations
const char *stations[] = {
    "Dhaka", "Chattogram", "Sylhet", "Rajshahi", 
    "Khulna", "Barishal", "Rangpur", "Mymensingh"
};
const int station_count = sizeof(stations) / sizeof(stations[0]);

// Validation Functions
int is_valid_email(const char *email) {
    const char *at = strchr(email, '@');
    const char *dot = strrchr(email, '.');
    return at && dot && at < dot;
}

int is_valid_phone(const char *phone) {
    if (strlen(phone) != 11) return 0;
    for (int i = 0; i < 11; i++) {
        if (!isdigit(phone[i])) return 0;
    }
    return 1;
}

// File operations
void load_users() {
    FILE *file = fopen(USERS_FILE, "r");
    if (file == NULL) {
        printf("\t\t\t\t\t\tNo users file found. Starting fresh.\n");
        return;
    }
    while (fscanf(file, "%s %s %[^\n]", users[user_count].email, users[user_count].password, users[user_count].fullname) != EOF) {
        user_count++;
    }
    fclose(file);
}

void save_user(User user) {
    FILE *file = fopen(USERS_FILE, "a");
    if (file == NULL) {
        printf("\t\t\t\t\t\tError saving user data.\n");
        return;
    }
    fprintf(file, "%s %s %s\n", user.email, user.password, user.fullname);
    fclose(file);
}

void save_reservation(Ticket ticket) {
    FILE *file = fopen(RESERVATIONS_FILE, "a");
    if (file == NULL) {
        printf("\t\t\t\t\t\tError saving reservation data.\n");
        return;
    }
    fprintf(file, "Reserved By: %s\nFull Name: %s\nFrom: %s\nTo: %s\nClass: %s\nCoach: %s\nSeat: %d\nDate: %s\n\n",
            ticket.reserved_by, ticket.fullname, ticket.from, ticket.to, ticket.cclass, ticket.coach, ticket.seat_number, ticket.date);
    fclose(file);
}

// User registration
void register_user() {
    if (user_count >= MAX_USERS) {
        printf("\t\t\t\t\t\tUser limit reached.\n");
        return;
    }

    char email_or_phone[50], password[30], fullname[50];
    int valid_contact = 0;

    while (!valid_contact) {
        printf("\t\t\t\t\t\tGmail or Phone Number: ");
        scanf("%s", email_or_phone);
        if (is_valid_email(email_or_phone) || is_valid_phone(email_or_phone)) {
            valid_contact = 1;
        } else {
            printf("\t\t\t\t\t\tInvalid Gmail or Phone Number. Please try again.\n");
        }
    }

    printf("\t\t\t\t\t\tPassword: ");
    scanf("%s", password);
    getchar(); // Consume newline
    printf("\t\t\t\t\t\tFull Name: ");
    fgets(fullname, sizeof(fullname), stdin);
    fullname[strcspn(fullname, "\n")] = '\0';

    for (int i = 0; i < user_count; i++) {
        if (strcmp(users[i].email, email_or_phone) == 0) {
            printf("\t\t\t\t\t\tThis Gmail or Phone Number already exists.\n");
            return;
        }
    }

    strcpy(users[user_count].email, email_or_phone);
    strcpy(users[user_count].password, password);
    strcpy(users[user_count].fullname, fullname);
    save_user(users[user_count]);
    user_count++;
    printf("\n\t\t\t\t\t\t**User registered successfully**\n\n");
  
}

int authenticate_user(char *logged_in_email, char *logged_in_fullname) {
    char email[50], password[30];
    int attempts = 3;

    while (attempts > 0) {
        printf("\t\t\t\t\t\tGmail or Phone Number: ");
        scanf("%s", email);
        printf("\n\t\t\t\t\t\tPassword: ");
        scanf("%s", password);

        for (int i = 0; i < user_count; i++) {
            if (strcmp(users[i].email, email) == 0 && strcmp(users[i].password, password) == 0) {
                printf("\n\t\t\t\t\t\t**Authentication successful**\n");
                printf("\n-------------------------------------------------------------------------------------------------------------------\n");
                strcpy(logged_in_email, email);
                strcpy(logged_in_fullname, users[i].fullname);
                return 1;
            }
        }
        attempts--;
        printf("\t\t\t\t\t\tInvalid information. %d attempts remaining.\n", attempts);
    }
    printf("\t\t\t\t\t\tAuthentication failed.\n");
    return 0;
}

void display_stations() {
    printf("\n\t\t\t\t\t\t->Available Stations<- \n\n");
    for (int i = 0; i < station_count; i++) {
        printf("\t\t\t\t\t\t%d. %s\n", i + 1, stations[i]);
    }
}

void select_station(char *station, const char *label) {
    int choice;
    while (1) {
        display_stations();
        printf("\n\t\t\t\t\t\t%s station (Enter number): ", label);
        scanf("%d", &choice);
    printf("\n-------------------------------------------------------------------------------------------------------------------\n\n");
        if (choice >= 1 && choice <= station_count) {
            strcpy(station, stations[choice - 1]);
            break;
        } else {
            printf("\t\t\t\t\t\tInvalid station number. Try again.\n");
        }
    }
}

void select_class(char *cclass, char *coach) {
    int choice;
    while (1) {
    	//printf("\n-------------------------------------------------------------------------------------------------------------------\n");
        printf("\n\t\t\t\t\t\t-> Choose class <- \n\n\t\t\t\t\t\t1. AC \n\t\t\t\t\t\t2. Sleeper \n\t\t\t\t\t\t3. Non-AC \n\n\t\t\t\t\t\t Enter your choice: ");
        scanf("%d", &choice);
    printf("\n\n-------------------------------------------------------------------------------------------------------------------\n\n");
        switch (choice) {
            case 1:
                strcpy(cclass, "AC");
                printf("\t\t\t\t\t\tSelect coach (A-E): ");
                scanf("%s", coach);
                if (coach[0] >= 'A' && coach[0] <= 'E') return;
                break;
            case 2:
                strcpy(cclass, "Sleeper");
                printf("\t\t\t\t\t\tSelect coach (F-J): ");
                scanf("%s", coach);
                if (coach[0] >= 'F' && coach[0] <= 'J') return;
                break;
            case 3:
                strcpy(cclass, "Non-AC");
                printf("\t\t\t\t\t\tSelect coach (K-Z): ");
                scanf("%s", coach);
                if (coach[0] >= 'K' && coach[0] <= 'Z') return;
                break;
            default:
                printf("\t\t\t\t\t\tInvalid choice.\n");
        }
        printf("\t\t\t\t\t\tInvalid coach.\n");
    }
}

void select_seat(int *seat_number) {
    while (1) {
        printf("\t\t\t\t\t\tSeat number (1-40): ");
        scanf("%d", seat_number);
        if (*seat_number >= 1 && *seat_number <= 40) break;
        printf("\t\t\t\t\t\tInvalid seat number. Try again.\n");
    }
}

void select_date(char *date) {
    printf("\t\t\t\t\t\tDate of Journey (DD-MM-YYYY): ");
    scanf("%s", date);
}

int check_seat_availability(char *date, char *coach, int seat_number) {
    for (int i = 0; i < ticket_count; i++) {
        if (strcmp(tickets[i].date, date) == 0 && strcmp(tickets[i].coach, coach) == 0 && tickets[i].seat_number == seat_number) {
            return 0;
        }
    }
    return 1;
}

void make_reservation(char *logged_in_email, char *logged_in_fullname) {
    int tickets_to_reserve;

    printf("\n\t\t\t\t\t\tHow many tickets do you want to buy: ");
    scanf("%d", &tickets_to_reserve);
    printf("\n\n-------------------------------------------------------------------------------------------------------------------\n");

    for (int i = 0; i < tickets_to_reserve; i++) {
        printf("\n\t\t\t\t\t\tReserving ticket: %d\n", i + 1);
        printf("-------------------------------------------------------------------------------------------------------------------\n");

        Ticket ticket;
        strcpy(ticket.reserved_by, logged_in_email);
        strcpy(ticket.fullname, logged_in_fullname);

        select_station(ticket.from, "From");
        select_station(ticket.to, "To");
        select_class(ticket.cclass, ticket.coach);
        select_seat(&ticket.seat_number);
        select_date(ticket.date);

        if (check_seat_availability(ticket.date, ticket.coach, ticket.seat_number)) {
            ticket.is_reserved = 1;
            tickets[ticket_count++] = ticket;
            save_reservation(ticket);

            char ts[30];
            printf("\n\t\t\t\t\t\tEnter your Transaction ID: ");
            scanf("%s", ts);
            printf("\n\t\t\t\t\t\tPayment Successful\n");
            printf("\t\t\t\t\t\t*** Reservation Confirmed! ***\n");
            printf("\n\t\t\t\t\t\t[ THANK YOU ]\n");
            printf("*******************************************[ Have A Safe Journey ]*******************************************\n\n");
        } else {
            printf("\t\t\t\t\t\tSeat is already reserved. Please choose another.\n");
            i--; // Retry current ticket
        }
    }
}


void main_menu() {
    char logged_in_email[50];
    char logged_in_fullname[50];
    int choice;
    load_users();

    while (1) {
        printf("########################################## | Bus Ticket Reservation System | ##########################################\n\n");
        
        printf("\t\t\t\t\t\t1. Register\n\t\t\t\t\t\t2. Login\n\t\t\t\t\t\t3. Exit\n\n\n\t\t\t\t\t\t");
        printf("\n-------------------------------------------------------------------------------------------------------------------\n\n");
        printf("\t\t\t\t\t\tEnter your choice : ");
        scanf("%d", &choice);
        printf("\n");
        if (choice == 1) {
            register_user();
        } else if (choice == 2) {
            if (authenticate_user(logged_in_email, logged_in_fullname)) {
                make_reservation(logged_in_email, logged_in_fullname);
            }
        } else if (choice == 3) {
            printf("\t\t\t\t\t\tExiting system.\n");
            break;
        } else {
            printf("\t\t\t\t\t\tInvalid choice.\n");
        }
    }
}

int main() {
    main_menu();
    return 0;
}
