#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_USERNAME 50
#define MAX_PASSWORD 50
#define USER_FILE "users.txt"

typedef struct{
    int id;
    char title[100];
    char author[50];
    int quantity;
    float price;
    int issuedCount;
} Book;

int login();
void addBook();
void displayBooks();
void searchBook();
void updateBook();
void issueBook();
void deleteBook();

int login() {
    char username[MAX_USERNAME], password[MAX_PASSWORD];
    char fileUser[MAX_USERNAME], filePass[MAX_PASSWORD];
    int found = 0;
    FILE *fp = fopen(USER_FILE, "r");

    if (fp == NULL) {
        printf("Error: Could not open %s\n", USER_FILE);
        return 0;
    }

    printf("====== Library Login ======\n");
    printf("Username: ");
    scanf("%s", username);
    printf("Password: ");
    scanf("%s", password);

    while (fscanf(fp, "%s %s", fileUser, filePass) != EOF) {
        if (strcmp(username, fileUser) == 0 && strcmp(password, filePass) == 0) {
            found = 1;
            break;
        }
    }

    fclose(fp);

    if (found) {
        printf("Login successfull!\n");
        return 1;
    } else {
        printf("Invalid username or password.\n");
        return 0;
    }
}

int main() {
    if (!login()) {
        printf("Access denied. Exiting program.\n");
        return 0;
    }

    int choice;
    do {
        printf("\n------ Library Management System ------\n");
        printf("1. Add Book\n");
        printf("2. Display All Books\n");
        printf("3. Search Book by ID\n");
        printf("4. Update Book\n");
        printf("5. Issued Book\n");
        printf("6. Delete Book\n");
        printf("7. Exit\n");
        printf("---------------------------------------\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: addBook(); break;
            case 2: displayBooks(); break;
            case 3: searchBook(); break;
            case 4: updateBook(); break;
            case 5: issueBook(); break;
            case 6: deleteBook(); break;
            case 7: printf("Exiting program.\n"); break;
            default: printf("Invalid choice! Try again.\n");
        }
    } while (choice != 7);

    return 0;
}

void addBook() {
    FILE *fp;
    Book b;
    fp = fopen("library.dat", "ab");
    if (fp == NULL) {
        printf("Error opening file.\n");
        return;
    }

    printf("Enter Book ID: ");
    scanf("%d", &b.id);
    getchar();
    printf("Enter Title: ");
    fgets(b.title, sizeof(b.title), stdin);
    b.title[strcspn(b.title, "\n")] = 0;
    printf("Enter Author: ");
    fgets(b.author, sizeof(b.author), stdin);
    b.author[strcspn(b.author, "\n")] = 0;
    printf("Enter Quantity: ");
    scanf("%d", &b.quantity);
    printf("Enter Price: ");
    scanf("%f", &b.price);
    b.issuedCount = 0;

    fwrite(&b, sizeof(Book), 1, fp);
    fclose(fp);
    printf("Book added successfully!\n");
}

void displayBooks() {
    FILE *fp;
    Book b;
    fp = fopen("library.dat", "rb");
    if (fp == NULL) {
        printf("No records found.\n");
        return;
    }

    printf("\nID\tTitle\t\t\tAuthor\t\tQuantity\tPrice\tBook Issue\n");
    printf("---------------------------------------------------------------------------------------\n");

    while (fread(&b, sizeof(Book), 1, fp)) {
        printf("%d\t%s\t%s\t%d\t%.2f\t%d\n",
               b.id, b.title, b.author, b.quantity, b.price, b.issuedCount);
    }
    fclose(fp);
}

void searchBook() {
    FILE *fp;
    Book b;
    int id, found = 0;

    printf("Enter Book ID to search: ");
    scanf("%d", &id);

    fp = fopen("library.dat", "rb");
    if (fp == NULL) {
        printf("Error opening file.\n");
        return;
    }

    while (fread(&b, sizeof(Book), 1, fp)) {
        if (b.id == id) {
            printf("\nBook Found:\n");
            printf("ID: %d\nTitle: %s\nAuthor: %s\nQuantity: %d\nPrice: %.2f\n",
                   b.id, b.title, b.author, b.quantity, b.price);
            found = 1;
            break;
        }
    }
    if (!found)
        printf("Book not found.\n");

    fclose(fp);
}

void updateBook() {
    FILE *fp;
    Book b;
    int id, found = 0;
    fp = fopen("library.dat", "rb+");
    if (fp == NULL) {
        printf("Error opening file.\n");
        return;
    }

    printf("Enter Book ID to update: ");
    scanf("%d", &id);

    while (fread(&b, sizeof(Book), 1, fp)) {
        if (b.id == id) {
            printf("Enter new Title: ");
            getchar();
            fgets(b.title, sizeof(b.title), stdin);
            b.title[strcspn(b.title, "\n")] = 0;
            printf("Enter new Author: ");
            fgets(b.author, sizeof(b.author), stdin);
            b.author[strcspn(b.author, "\n")] = 0;
            printf("Enter new Quantity: ");
            scanf("%d", &b.quantity);
            printf("Enter new Price: ");
            scanf("%f", &b.price);

            fseek(fp, -sizeof(Book), SEEK_CUR);
            fwrite(&b, sizeof(Book), 1, fp);
            printf("Book updated successfully!\n");
            found = 1;
            break;
        }
    }

    if (!found)
        printf("Book not found.\n");

    fclose(fp);
}

void issueBook() {
    FILE *fp;
    Book b;
    int id, found = 0;

    fp = fopen("library.dat", "rb+");
    if (fp == NULL) {
        printf("Error opening file.\n");
        return;
    }

    printf("Enter Book ID to issue: ");
    scanf("%d", &id);

    while (fread(&b, sizeof(Book), 1, fp)) {
        if (b.id == id) {
            found = 1;
            if (b.quantity > 0) {
                b.quantity -= 1;
                b.issuedCount++;
                fseek(fp, -sizeof(Book), SEEK_CUR);
                fwrite(&b, sizeof(Book), 1, fp);
                printf("Book issued successfully! Remaining quantity: %d\n", b.quantity);
            } else {
                printf("Book is currently not available (Out of stock).\n");
            }
            break;
        }
    }

    if (!found)
        printf("Book not found.\n");

    fclose(fp);
}


void deleteBook() {
    FILE *fp, *temp;
    Book b;
    int id, found = 0;

    printf("Enter Book ID to delete: ");
    scanf("%d", &id);

    fp = fopen("library.dat", "rb");
    temp = fopen("temp.dat", "wb");

    if (fp == NULL || temp == NULL) {
        printf("Error opening file.\n");
        return;
    }

    while (fread(&b, sizeof(Book), 1, fp)) {
        if (b.id != id) {
            fwrite(&b, sizeof(Book), 1, temp);
        } else {
            found = 1;
        }
    }

    fclose(fp);
    fclose(temp);
    remove("library.dat");
    rename("temp.dat", "library.dat");

    if (found)
        printf("Book deleted successfully!\n");
    else
        printf("Book not found.\n");
}
