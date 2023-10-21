#include "header.h"

const char *RECORDS = "./data/records.txt";


/* -------------------------------------------------------------------------- */
/*                             CLEAN BUFFER                                   */
/* -------------------------------------------------------------------------- */

void clearInputBuffer() {
    char ch;
    while ((ch = getchar()) != '\n' && ch != EOF);
}


/* -------------------------------------------------------------------------- */
/*                             GET ACCOUNT FROM FILE                          */
/* -------------------------------------------------------------------------- */

int getAccountFromFile(FILE *ptr, char name[50], struct Record *r)
{
    return fscanf(ptr, "%d %d %s %d %d/%d/%d %s %d %lf %s",
                  &r->id,
                  &r->userId,
                  name,
                  &r->accountNbr,
                  &r->deposit.month,
                  &r->deposit.day,
                  &r->deposit.year,
                  r->country,
                  &r->phone,
                  &r->amount,
                  r->accountType) != EOF;
}

/* -------------------------------------------------------------------------- */
/*                             SAVE USER TO FILE                              */
/* -------------------------------------------------------------------------- */

void saveAccountToFile(FILE *ptr, struct User u, struct Record r)
{
    fprintf(ptr, "%d %d %s %d %d/%d/%d %s %d %.2lf %s\n",
            r.id,
            u.id,
            u.name,
            r.accountNbr,
            r.deposit.month,
            r.deposit.day,
            r.deposit.year,
            r.country,
            r.phone,
            r.amount,
            r.accountType);
}

/* -------------------------------------------------------------------------- */
/*                             SYSTEM FUNCTIONS                               */
/* -------------------------------------------------------------------------- */

void stayOrReturn(int notGood, void f(struct User u), struct User u)
{
    int option;
    if (notGood == 0)
    {
        system("clear");
        printf("\n✖ Record not found!!\n");
    invalid:
        printf("\nEnter 0 to try again, 1 to return to main menu and 2 to exit:");
        scanf("%d", &option);
        if (option == 0)
            f(u);
        else if (option == 1)
            mainMenu(u);
        else if (option == 2)
            exit(0);
        else
        {
            printf("Insert a valid operation!\n");
            goto invalid;
        }
    }
    else
    {
        printf("\nEnter 1 to go to the main menu and 0 to exit:");
        scanf("%d", &option);
    }
    if (option == 1)
    {
        system("clear");
        mainMenu(u);
    }
    else
    {
        system("clear");
        exit(1);
    }
}

void success(struct User u)
{
    int option;
    printf("\n✔ Success!\n\n");
invalid:
    printf("Enter 1 to go to the main menu and 0 to exit!\n");
    scanf("%d", &option);
    system("clear");
    if (option == 1)
    {
        mainMenu(u);
    }
    else if (option == 0)
    {
        exit(1);
    }
    else
    {
        printf("Insert a valid operation!\n");
        system("clear");
        goto invalid;
    }
}

void createNewAcc(struct User u) {
    struct Record r;
    struct Record cr;
    char userName[50];
    FILE *pf = fopen(RECORDS, "a+");

noAccount:
    system("clear");
    printf("\t\t\t===== New record =====\n");

    printf("\nEnter today's date(mm/dd/yyyy):");
    scanf("%d/%d/%d", &r.deposit.month, &r.deposit.day, &r.deposit.year);
    clearInputBuffer();  // Clear the input buffer after reading the date

    printf("\nEnter the account number:");
    scanf("%d", &r.accountNbr);
    clearInputBuffer();  // Clear the input buffer after reading an integer

    while (getAccountFromFile(pf, userName, &cr)) {
        if (strcmp(userName, u.name) == 0 && cr.accountNbr == r.accountNbr) {
            printf("✖ This Account already exists for this user\n\n");
            goto noAccount;
        }
    }

    printf("\nEnter the country:");
    fgets(r.country, sizeof(r.country), stdin);  // Use fgets to read strings with spaces

    printf("\nEnter the phone number:");
    scanf("%d", &r.phone);
    clearInputBuffer();  // Clear the input buffer after reading an integer

    printf("\nEnter amount to deposit: $");
    scanf("%lf", &r.amount);
    clearInputBuffer();  // Clear the input buffer after reading a double

    printf("\nChoose the type of account:\n\t-> saving\n\t-> current\n\t-> fixed01(for 1 year)\n\t-> fixed02(for 2 years)\n\t-> fixed03(for 3 years)\n\n\tEnter your choice:");
    fgets(r.accountType, sizeof(r.accountType), stdin);  // Use fgets to read strings with spaces

    saveAccountToFile(pf, u, r);

    fclose(pf);
    success(u);
}

void checkAllAccounts(struct User u)
{
    char userName[100];
    struct Record r;

    FILE *pf = fopen(RECORDS, "r");

    system("clear");
    printf("\t\t====== All accounts from user, %s =====\n\n", u.name);
    while (getAccountFromFile(pf, userName, &r))
    {
        if (strcmp(userName, u.name) == 0)
        {
            printf("_____________________\n");
            printf("\nAccount number:%d\nDeposit Date:%d/%d/%d \ncountry:%s \nPhone number:%d \nAmount deposited: $%.2f \nType Of Account:%s\n",
                   r.accountNbr,
                   r.deposit.day,
                   r.deposit.month,
                   r.deposit.year,
                   r.country,
                   r.phone,
                   r.amount,
                   r.accountType);
        }
    }
    fclose(pf);
    success(u);
}

void updateAccountInfo(struct User u) {
    struct Record r;
    char userName[50];
    FILE *pf = fopen(RECORDS, "r+");
    int accID, choice, found = 0;

    printf("Enter the account ID you want to update: ");
    scanf("%d", &accID);

    while (getAccountFromFile(pf, userName, &r)) {
        if (strcmp(userName, u.name) == 0 && r.accountNbr == accID) {
            found = 1;
            printf("Which field do you want to update?\n");
            printf("1. Country\n2. Phone Number\n");
            scanf("%d", &choice);

            switch (choice) {
                case 1:
                    printf("Enter new country: ");
                    scanf("%s", r.country);
                    break;
                case 2:
                    printf("Enter new phone number: ");
                    scanf("%d", &r.phone);
                    break;
                default:
                    printf("Invalid choice!\n");
                    return;
            }

            fseek(pf, -sizeof(struct Record), SEEK_CUR);
            saveAccountToFile(pf, u, r);
            break;
        }
    }

    fclose(pf);
    if (found) {
        success(u);
    } else {
        stayOrReturn(0, updateAccountInfo, u);
    }
}

void checkAccountDetails(struct User u) {
    struct Record r;
    char userName[50];
    FILE *pf = fopen(RECORDS, "r");
    int accID, found = 0;

    printf("Enter the account ID you want to check: ");
    scanf("%d", &accID);

    while (getAccountFromFile(pf, userName, &r)) {
        if (strcmp(userName, u.name) == 0 && r.accountNbr == accID) {
            found = 1;
            printf("Account Details:\n");
            printf("Account number: %d\n", r.accountNbr);
            printf("Deposit Date: %d/%d/%d\n", r.deposit.day, r.deposit.month, r.deposit.year);
            printf("Country: %s\n", r.country);
            printf("Phone number: %d\n", r.phone);
            printf("Amount deposited: $%.2f\n", r.amount);
            printf("Type Of Account: %s\n", r.accountType);
            // Display interest details based on account type
            if (strcmp(r.accountType, "saving") == 0) {
                printf("Interest Rate: 3.5%%\n");
                printf("Interest Amount: $%.2f\n", r.amount * 0.035);
            } else if (strcmp(r.accountType, "current") == 0) {
                printf("Interest Rate: 0%%\n");
                printf("Interest Amount: $0.00\n");
            } else if (strcmp(r.accountType, "fixed01") == 0) {
                printf("Interest Rate: 5%%\n");
                printf("Interest Amount: $%.2f\n", r.amount * 0.05);
            } else if (strcmp(r.accountType, "fixed02") == 0) {
                printf("Interest Rate: 7%%\n");
                printf("Interest Amount: $%.2f\n", r.amount * 0.07);
            } else if (strcmp(r.accountType, "fixed03") == 0) {
                printf("Interest Rate: 9%%\n");
                printf("Interest Amount: $%.2f\n", r.amount * 0.09);
            }

            break;
        }
    }

    fclose(pf);
    if (found) {
        success(u);
    } else {
        stayOrReturn(0, checkAccountDetails, u);
    }
}

void makeTransaction(struct User u) {
    struct Record r;
    char userName[50];
    FILE *pf = fopen(RECORDS, "r+");
    int accID, choice, found = 0;
    double amount;

    printf("Enter the account ID for the transaction: ");
    scanf("%d", &accID);

    while (getAccountFromFile(pf, userName, &r)) {
        if (strcmp(userName, u.name) == 0 && r.accountNbr == accID) {
            found = 1;
            printf("1. Deposit\n2. Withdraw\n");
            scanf("%d", &choice);

            switch (choice) {
                case 1:
                    printf("Enter amount to deposit: ");
                    scanf("%lf", &amount);
                    r.amount += amount;
                    break;
                case 2:
                    printf("Enter amount to withdraw: ");
                    scanf("%lf", &amount);
                    if (amount <= r.amount) {
                        r.amount -= amount;
                    } else {
                        printf("Insufficient funds!\n");
                        return;
                    }
                    break;
                default:
                    printf("Invalid choice!\n");
                    return;
            }

            fseek(pf, -sizeof(struct Record), SEEK_CUR);
            saveAccountToFile(pf, u, r);
            break;
        }
    }

    fclose(pf);
    if (found) {
        success(u);
    } else {
        stayOrReturn(0, makeTransaction, u);
    }
}

void removeAccount(struct User u) {
    struct Record r;
    char userName[50];
    FILE *pf = fopen(RECORDS, "r");
    FILE *temp = fopen("temp.txt", "w");
    int accID, found = 0;

    printf("Enter the account ID you want to remove: ");
    scanf("%d", &accID);

    while (getAccountFromFile(pf, userName, &r)) {
        if (strcmp(userName, u.name) == 0 && r.accountNbr == accID) {
            found = 1;
            continue;
        }
        saveAccountToFile(temp, u, r);
    }

    fclose(pf);
    fclose(temp);
    remove(RECORDS);
    rename("temp.txt", RECORDS);

    if (found) {
        success(u);
    } else {
        stayOrReturn(0, removeAccount, u);
    }
}

void transferOwnership(struct User u) {
    struct Record r;
    char userName[50], newOwner[50];
    FILE *pf = fopen(RECORDS, "r+");
    int accID, found = 0;

    printf("Enter the account ID you want to transfer: ");
    scanf("%d", &accID);
    printf("Enter the name of the new owner: ");
    scanf("%s", newOwner);

    while (getAccountFromFile(pf, userName, &r)) {
        if (strcmp(userName, u.name) == 0 && r.accountNbr == accID) {
            found = 1;
            strcpy(userName, newOwner);
            fseek(pf, -sizeof(struct Record), SEEK_CUR);
            saveAccountToFile(pf, u, r);
            break;
        }
    }

    fclose(pf);
    if (found) {
        success(u);
    } else {
        stayOrReturn(0, transferOwnership, u);
    }
}
