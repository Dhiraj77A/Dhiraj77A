// utils.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // For sleep() on Unix-like systems
#include "headers.h"
#include <ctype.h>  // For tolower

int stricmp(const char *s1, const char *s2) {
    while (*s1 && *s2) {
        if (tolower((unsigned char)*s1) != tolower((unsigned char)*s2)) {
            return 1;  // Strings are not equal
        }
        s1++;
        s2++;
    }
    return tolower((unsigned char)*s1) != tolower((unsigned char)*s2);
}

// Function prototypes
void print_header(const char *title);
void print_divider();
void loading_effect();
void clear_screen();
int get_menu_choice(int min, int max);
void display_admin_menu();
void display_student_menu();


// Function definitions
void print_header(const char *title) {
    printf("\n================ %s ================\n", title);
}

void print_divider() {
    printf("============================================\n");
}

void loading_effect() {
    printf("Loading");
    for (int i = 0; i < 3; i++) {
        printf(".");
        fflush(stdout); // Ensure dots appear immediately
        sleep(1);       // 1-second delay (portable alternative to Sleep())
    }
    printf("\n");
}

void clear_screen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

int get_menu_choice(int min, int max) {
    int choice;
    char input[10];
    printf("Enter your choice (%d-%d): ", min, max);
    fgets(input, sizeof(input), stdin);
    choice = atoi(input);
    if (choice < min || choice > max) {
        printf("Invalid choice. Please try again.\n");
        return get_menu_choice(min, max);
    }
    return choice;
}

void display_admin_menu() {
    print_header("Admin Menu");
    printf("| 1. Manage Books                     |\n");
    printf("| 2. Manage Users                     |\n");
    printf("| 3. Logout                           |\n");
    print_divider();
}

void display_student_menu() {
    print_header("Student Menu");
    printf("| 1. Borrow Book                      |\n");
    printf("| 2. Return Book                      |\n");
    printf("| 3. Logout                           |\n");
    print_divider();
}
