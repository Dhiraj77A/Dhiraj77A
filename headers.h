// headers.h
#ifndef HEADERS_H
#define HEADERS_H

#include <stdio.h> // For printf, scanf
#include <time.h>  // For time_t

#define MAX_USERNAME 20
#define MAX_PASSWORD 20
#define MAX_NAME 50
#define MAX_ROLE 10
#define MAX_TITLE 100
#define MAX_AUTHOR 50
#define MAX_BOOK_ID 13

// User structure
typedef struct {
    char username[MAX_USERNAME];
    char password[MAX_PASSWORD];
    char name[MAX_NAME];
    char role[MAX_ROLE];
    int failed_attempts;
    int is_admin;
} User;

// Session structure
typedef struct {
    User current_user;
    int is_logged_in;
} Session;

// Book structure
typedef struct {
    char book_id[MAX_BOOK_ID];
    char title[MAX_TITLE];
    char author[MAX_AUTHOR];
    int published_year;
    int is_available;
} Book;

// Transaction structure
typedef struct {
    char book_id[MAX_BOOK_ID];
    char username[MAX_USERNAME];
    time_t issue_date;
    time_t return_date; // 0 if not returned yet
} Transaction;

// Fine structure
typedef struct {
    char username[MAX_USERNAME];
    double amount;
    int is_paid;
} Fine;

// Function prototypes (database.c)
int authenticate_user(char *username, char *password, const char *role, User *user);
int create_user(char *username, char *password, char *name, int is_admin);
void save_session(Session *session);
void load_session(Session *session);
void database_init();
int validate_string(const char *str, int max_len);

// Function prototypes (utils.c)
void print_header(const char *title);
void print_divider();
void loading_effect();
int get_menu_choice(int min, int max);
void display_admin_menu();
void display_student_menu();
void clear_screen();

// Function prototypes (student.c)
int borrow_book(const char *username, const char *book_id);
int return_book(const char *username, const char *book_id);
int reserve_book(const char *username, const char *book_id);
int check_book_availability(const char *book_id);

// Function prototypes (transactions.c)
int issue_book(const char *username, const char *book_id);
int return_book_transaction(const char *username, const char *book_id);
double calculate_fine(const char *username, const char *book_id);
void update_fine(const char *username, double amount);
double get_user_fines(const char *username);

// Function prototypes (admin.c)
void manage_books();
void display_all_books();
void generate_book_id(const char *title_code, const char *author_code, int year, char *book_id);
void update_user();
void display_all_users();

#endif
