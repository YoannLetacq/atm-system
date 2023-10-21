#include <termios.h>
#include "header.h"

char *USERS = "./data/users.txt";

void loginMenu(char a[50], char pass[50])
{
    struct termios oflags, nflags;

    system("clear");
    printf("\n\n\n\t\t\t\t   Bank Management System\n\t\t\t\t\t User Login:");
    scanf("%s", a);

    // disabling echo
    tcgetattr(fileno(stdin), &oflags);
    nflags = oflags;
    nflags.c_lflag &= ~ECHO;
    nflags.c_lflag |= ECHONL;

    if (tcsetattr(fileno(stdin), TCSANOW, &nflags) != 0)
    {
        perror("tcsetattr");
        exit(1);
    }
    printf("\n\n\n\n\n\t\t\t\tEnter the password to login:");
    scanf("%s", pass);

    // restore terminal
    if (tcsetattr(fileno(stdin), TCSANOW, &oflags) != 0)
    {
        perror("tcsetattr");
        exit(1);
    }
}

void getPassword(struct User u, char *buffer)
{
    FILE *fp;
    struct User userChecker;

    if ((fp = fopen("./data/users.txt", "r")) == NULL)
    {
        printf("Error! opening file");
        exit(1);
    }

    while (fscanf(fp, "%s %s", userChecker.name, userChecker.password) != EOF)
    {
        if (strcmp(userChecker.name, u.name) == 0)
        {
            strcpy(buffer, userChecker.password);
            fclose(fp);
            return;
        }
    }

    strcpy(buffer, "no user found");
    fclose(fp);
}

void registerUser(struct User newUser) {
    FILE *fp = fopen(USERS, "a+");

    if (fp == NULL) {
        printf("Error! opening file\n");
        exit(1);
    }

    // Check if user already exists
    struct User existingUser;
    existingUser.id = 0; // Initialize ID to 0 for the case where the file is empty

    char line[150]; // Buffer to store each line from the file
    while (fgets(line, sizeof(line), fp) != NULL) {
        if (sscanf(line, "%d %49s %49s", &existingUser.id, existingUser.name, existingUser.password) != 3) {
            printf("Error reading user data from file.\n");
            fclose(fp);
            return;
        }
        if (strcmp(existingUser.name, newUser.name) == 0) {
            printf("User with this name already exists!\n");
            fclose(fp);
            return;
        }
    }

    // Assign ID to the new user
    newUser.id = existingUser.id + 1;

    fprintf(fp, "%d %s %s\n", newUser.id, newUser.name, newUser.password);
    printf("Registration successful!\n");

    fclose(fp);
}


void registerMenu(char a[50], char pass[50]) {
    printf("\n\n\n\t\t\t\t   Bank Management System\n\t\t\t\t\t User Registration:");
    scanf("%49s", a);
    clearInputBuffer();

    printf("\n\n\n\n\n\t\t\t\tEnter a password for registration:");
    scanf("%49s", pass);
    clearInputBuffer();

    struct User newUser;
    strcpy(newUser.name, a);
    strcpy(newUser.password, pass);
    registerUser(newUser);
}


