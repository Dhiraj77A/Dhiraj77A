// main.c
#include <stdio.h>
#include <string.h>
#include "headers.h"

Session current_session = {{{0}}, 0};

void clear_input_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int login_system(const char* role) {
    char username[MAX_USERNAME];
    char password[MAX_PASSWORD];
    User user;

    print_header("Library Management System");
    printf("Enter username: ");
    scanf("%19s", username);
    clear_input_buffer();

    printf("Enter password: ");
    scanf("%19s", password);
    clear_input_buffer();

    if (authenticate_user(username, password, role, &user)) {
        current_session.current_user = user;
        current_session.is_logged_in = 1;
        save_session(&current_session);  // Fixed typo: ¤t_session -> &current_session
        printf("Login successful! Welcome, %s\n", user.name);
        printf("Debug: Role set to '%s', is_logged_in = %d\n", user.role, current_session.is_logged_in);
        loading_effect();
        return 1;
    } else {
        printf("Login failed. Please try again.\n");
        return 0;
    }
}

int create_account() {
    char username[MAX_USERNAME];
    char password[MAX_PASSWORD];
    char name[MAX_NAME];

    print_header("Create New Account");
    printf("Enter username: ");
    scanf("%19s", username);
    clear_input_buffer();

    printf("Enter password: ");
    scanf("%19s", password);
    clear_input_buffer();

    printf("Enter your full name: ");
    fgets(name, MAX_NAME, stdin);
    name[strcspn(name, "\n")] = 0;

    return create_user(username, password, name, 0);
}

void logout_system() {
    if (current_session.is_logged_in) {
        printf("Logging out %s...\n", current_session.current_user.name);
        current_session.is_logged_in = 0;
        memset(&current_session.current_user, 0, sizeof(User));
        save_session(&current_session);  // Fixed typo
        printf("Logged out successfully.\n");
        loading_effect();
    }
}

void display_main_menu() {
    clear_screen();
    print_header("Library Management System");
    printf("| Welcome to the Library System!        |\n");
    print_divider();
    printf("| 1. Admin Login                       |\n");
    printf("| 2. Student Login                     |\n");
    printf("| 3. Create Account                    |\n");
    printf("| 4. Exit                              |\n");
    print_divider();
}

int main() {
    database_init();
    load_session(&current_session);  // Fixed typo

    while (1) {
        if (!current_session.is_logged_in) {
            display_main_menu();
            int choice = get_menu_choice(1, 4);

            switch (choice) {
                case 1: // Admin Login
                    clear_screen();
                    login_system("Admin");
                    break;
                case 2: // Student Login
                    clear_screen();
                    login_system("Student");
                    break;
                case 3: // Create Account
                    clear_screen();
                    create_account();
                    break;
                case 4: // Exit
                    printf("Exiting...\n");
                    loading_effect();
                    return 0;
                default:
                    printf("Invalid choice\n");
            }
        } else {
            printf("Debug: Entering logged-in state\n");
            printf("Debug: is_logged_in = %d, Role = '%s'\n",
                   current_session.is_logged_in,
                   current_session.current_user.role);
            if (strcmp(current_session.current_user.role, "Admin") == 0) {
                printf("Debug: Admin menu triggered\n");
                display_admin_menu();
                int choice = get_menu_choice(1, 3);
                switch (choice) {
                    case 1: // Manage Books
                        clear_screen();
                        manage_books();
                        break;
                    case 2: // Manage Users
                        clear_screen();
                        update_user();
                        break;
                    case 3: // Logout
                        clear_screen();
                        logout_system();
                        break;
                }
            } else if (strcmp(current_session.current_user.role, "Student") == 0) {
                printf("Debug: Student menu triggered\n");
                display_student_menu();
                int choice = get_menu_choice(1, 3);
                switch (choice) {
                    case 1: { // Borrow Book
                        clear_screen();
                        char book_id[MAX_BOOK_ID];
                        printf("Enter Book ID: ");
                        fgets(book_id, MAX_BOOK_ID, stdin);
                        book_id[strcspn(book_id, "\n")] = 0;
                        borrow_book(current_session.current_user.username, book_id);
                        break;
                    }
                    case 2: { // Return Book
                        clear_screen();
                        char book_id[MAX_BOOK_ID];
                        printf("Enter Book ID: ");
                        fgets(book_id, MAX_BOOK_ID, stdin);
                        book_id[strcspn(book_id, "\n")] = 0;
                        return_book(current_session.current_user.username, book_id);
                        break;
                    }
                    case 3: // Logout
                        clear_screen();
                        logout_system();
                        break;
                }
            } else {
                printf("Unknown role: '%s'\n", current_session.current_user.role);
                logout_system();
            }
        }
        printf("Press Enter to continue...");
        clear_input_buffer();
        getchar();
    }

    return 0;
}
