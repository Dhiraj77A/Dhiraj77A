#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "headers.h"

#define BOOKS_FILE "books.dat"
#define REPORTS_FILE "reports.csv"
#define USERS_FILE "users.dat"
#define MAX_ID_LENGTH 13

// Function prototypes
void clear_input_buffer();
void add_book();
void remove_book();
void update_book();
void display_all_books();
void generate_reports();
void manage_books();
void update_user();
void display_all_users();
void generate_book_id(const char *title_code, const char *author_code, int year, char *book_id);

// Function definitions
void add_book() {
    Book book;
    char title_code[4], author_code[4];

    printf("Enter a unique 3-letter code for the book title (e.g., PHY): ");
    scanf("%3s", title_code);
    clear_input_buffer();

    printf("Enter a unique 3-letter code for the author's name (e.g., BPK): ");
    scanf("%3s", author_code);
    clear_input_buffer();

    printf("Enter publication year: ");
    scanf("%d", &book.published_year);
    clear_input_buffer();

    generate_book_id(title_code, author_code, book.published_year, book.book_id);

    printf("Generated Book ID: %s\n", book.book_id);

    printf("Enter Title: ");
    fgets(book.title, MAX_TITLE, stdin);
    book.title[strcspn(book.title, "\n")] = 0;
    printf("Enter Author: ");
    fgets(book.author, MAX_AUTHOR, stdin);
    book.author[strcspn(book.author, "\n")] = 0;
    book.is_available = 1;

    FILE *fp = fopen(BOOKS_FILE, "ab");
    if (!fp) {
        printf("Error opening books file.\n");
        return;
    }
    fwrite(&book, sizeof(Book), 1, fp);
    fclose(fp);
    printf("Book added successfully!\n");
}

void remove_book() {
    display_all_books();

    char book_id[MAX_BOOK_ID];
    printf("Enter Book ID to remove: ");
    fgets(book_id, MAX_BOOK_ID, stdin);
    book_id[strcspn(book_id, "\n")] = 0;  // Remove newline
    clear_input_buffer();

    FILE *fp = fopen(BOOKS_FILE, "rb");
    if (!fp) {
        printf("Error opening books file.\n");
        return;
    }

    FILE *temp_fp = fopen("temp.dat", "wb");
    if (!temp_fp) {
        printf("Error creating temporary file.\n");
        fclose(fp);
        return;
    }

    Book book;
    int found = 0;
    while (fread(&book, sizeof(Book), 1, fp)) {
        printf("Comparing: '%s' with '%s'\n", book.book_id, book_id);  // Debug output
        if (stricmp(book.book_id, book_id) == 0) {
            found = 1;
        } else {
            fwrite(&book, sizeof(Book), 1, temp_fp);
        }
    }

    fclose(fp);
    fclose(temp_fp);

    if (found) {
        remove(BOOKS_FILE);
        rename("temp.dat", BOOKS_FILE);
        printf("Book removed successfully!\n");
    } else {
        remove("temp.dat");
        printf("Book ID not found.\n");
    }
}

void update_book() {
    display_all_books();

    char book_id[MAX_BOOK_ID];
    printf("Enter Book ID to update: ");
    fgets(book_id, MAX_BOOK_ID, stdin);
    book_id[strcspn(book_id, "\n")] = 0;  // Remove newline
    clear_input_buffer();

    FILE *fp = fopen(BOOKS_FILE, "rb+");
    if (!fp) {
        printf("Error opening books file.\n");
        return;
    }

    Book book;
    long pos;
    int found = 0;
    while (fread(&book, sizeof(Book), 1, fp)) {
        printf("Comparing: '%s' with '%s'\n", book.book_id, book_id);  // Debug output
        if (stricmp(book.book_id, book_id) == 0) {
            found = 1;
            pos = ftell(fp) - sizeof(Book);
            printf("Enter new Title: ");
            fgets(book.title, MAX_TITLE, stdin);
            book.title[strcspn(book.title, "\n")] = 0;
            printf("Enter new Author: ");
            fgets(book.author, MAX_AUTHOR, stdin);
            book.author[strcspn(book.author, "\n")] = 0;
            printf("Enter new Publication Year: ");
            scanf("%d", &book.published_year);
            clear_input_buffer();
            fseek(fp, pos, SEEK_SET);
            fwrite(&book, sizeof(Book), 1, fp);
            printf("Book updated successfully!\n");
            break;
        }
    }

    if (!found) {
        printf("Book ID not found.\n");
    }

    fclose(fp);
}
void display_all_books() {
    FILE *fp = fopen(BOOKS_FILE, "rb");
    if (!fp) {
        printf("Error opening books file.\n");
        return;
    }

    Book book;
    printf("\nAll Books:\n");
    printf("ID\t\t\tTitle\t\t\t\tAuthor\t\t\t\tYear\tAvailability\n");
    printf("----------------------------------------------------------------------------------------------------------\n");
    while (fread(&book, sizeof(Book), 1, fp)) {
        printf("%-15s\t%-30s\t%-30s\t%d\t%s\n", book.book_id, book.title, book.author, book.published_year, book.is_available ? "Available" : "Borrowed");
    }

    fclose(fp);
}

void update_user() {
    display_all_users();

    char username[MAX_USERNAME];
    printf("Enter Username to update: ");
    scanf("%19s", username);
    clear_input_buffer();

    FILE *fp = fopen(USERS_FILE, "rb+");
    if (!fp) {
        printf("Error opening users file.\n");
        return;
    }

    User user;
    long pos;
    int found = 0;
    while (fread(&user, sizeof(User), 1, fp)) {
        if (strcmp(user.username, username) == 0) {
            found = 1;
            pos = ftell(fp) - sizeof(User);
            printf("Enter new Password: ");
            fgets(user.password, MAX_PASSWORD, stdin);
            user.password[strcspn(user.password, "\n")] = 0;
            printf("Enter new Full Name: ");
            fgets(user.name, MAX_NAME, stdin);
            user.name[strcspn(user.name, "\n")] = 0;
            fseek(fp, pos, SEEK_SET);
            fwrite(&user, sizeof(User), 1, fp);
            printf("User updated successfully!\n");
            break;
        }
    }

    if (!found) {
        printf("Username not found.\n");
    }

    fclose(fp);
}

void display_all_users() {
    FILE *fp = fopen(USERS_FILE, "rb");
    if (!fp) {
        printf("Error opening users file.\n");
        return;
    }

    User user;
    printf("\nAll Users:\n");
    printf("Username\t\tName\t\t\tRole\n");
    printf("------------------------------------------------------------\n");
    while (fread(&user, sizeof(User), 1, fp)) {
        printf("%-20s\t%-30s\t%s\n", user.username, user.name, user.role);
    }

    fclose(fp);
}

void generate_reports() {
    FILE *fp = fopen(REPORTS_FILE, "w");
    if (!fp) {
        printf("Error creating reports file.\n");
        return;
    }

    fprintf(fp, "Book ID,Title,Author,Year,Availability\n");

    FILE *books_fp = fopen(BOOKS_FILE, "rb");
    if (!books_fp) {
        printf("Error opening books file.\n");
        fclose(fp);
        return;
    }

    Book book;
    while (fread(&book, sizeof(Book), 1, books_fp)) {
        fprintf(fp, "%s,%s,%s,%d,%s\n", book.book_id, book.title, book.author, book.published_year, book.is_available ? "Available" : "Borrowed");
    }

    fclose(books_fp);
    fclose(fp);
    printf("Reports generated successfully!\n");
}

void manage_books() {
    while (1) {
        clear_screen();
        print_header("Manage Books");
        printf("| 1. Add Book                         |\n");
        printf("| 2. Remove Book                      |\n");
        printf("| 3. Update Book                      |\n");
        printf("| 4. Display All Books                |\n");
        printf("| 5. Back to Admin Menu               |\n");
        print_divider();
        int choice = get_menu_choice(1, 5);

        switch (choice) {
            case 1:
                clear_screen();
                add_book();
                break;
            case 2:
                clear_screen();
                remove_book();
                break;
            case 3:
                clear_screen();
                update_book();
                break;
            case 4:
                clear_screen();
                display_all_books();
                break;
            case 5:
                return;
            default:
                printf("Invalid choice\n");
        }
        printf("Press Enter to continue...");
        clear_input_buffer();
        getchar();
    }
}
void generate_book_id(const char *title_code, const char *author_code, int year, char *book_id) {
    char upper_title[4], upper_author[4];
    for (int i = 0; i < 3; i++) {
        upper_title[i] = toupper(title_code[i]);
        upper_author[i] = toupper(author_code[i]);
    }
    upper_title[3] = '\0';
    upper_author[3] = '\0';
    snprintf(book_id, MAX_BOOK_ID, "%s-%s-%d", upper_title, upper_author, year);
}
