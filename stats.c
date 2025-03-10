#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "headers.h"

#define TRANSACTIONS_FILE "transactions.dat"
#define FINES_FILE "fines.dat"

// Function prototypes
void popular_books();
void fine_revenue();

// Function definitions
void popular_books() {
    FILE *fp = fopen(TRANSACTIONS_FILE, "rb");
    if (!fp) {
        printf("Error opening transactions file.\n");
        return;
    }

    // Count the number of times each book has been borrowed
    int book_count[MAX_BOOK_ID] = {0};
    Transaction trans;
    while (fread(&trans, sizeof(Transaction), 1, fp)) {
        book_count[atoi(trans.book_id)]++;
    }
    fclose(fp);

    // Find the most popular book
    int max_count = 0;
    char popular_book[MAX_BOOK_ID];
    for (int i = 0; i < MAX_BOOK_ID; i++) {
        if (book_count[i] > max_count) {
            max_count = book_count[i];
            sprintf(popular_book, "%d", i);
        }
    }

    printf("Most popular book: %s (borrowed %d times)\n", popular_book, max_count);
}

// Function to generate statistics on revenue from fines
void fine_revenue() {
    FILE *fp = fopen(FINES_FILE, "rb");
    if (!fp) {
        printf("Error opening fines file.\n");
        return;
    }

    double total_revenue = 0.0;
    Fine fine;
    while (fread(&fine, sizeof(Fine), 1, fp)) {
        if (fine.is_paid) {
            total_revenue += fine.amount;
        }
    }
    fclose(fp);

    printf("Total revenue from fines: $%.2f\n", total_revenue);
}
