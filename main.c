#include "dataStructers.h"

int main(int argc, char *argv[]) {
    Account accounts[MAX_ACCOUNTS];
    int numAccounts = 0;
    int successFlag;
    Person currentUser;
    readAccountsFromFile(accounts, &numAccounts);
    if (argc == 4&&strcmp(argv[1], "login") == 0) {
        char *name = argv[2];
        char *surname = argv[3];
        int found = 0;
        for (int i = 0; i < numAccounts; i++) {
            if (strcmp(accounts[i].owner.name, name) == 0 && strcmp(accounts[i].owner.surname, surname) == 0) {
                found = 1;
                strcpy(currentUser.name, name);
                strcpy(currentUser.surname, surname);
                break;
            }
        }
        if (!found) {
            printf("Error: User not found in accounts.\n");
            return 1;
        }
        printf("Login successful.\n");
        menu2(accounts, numAccounts, &currentUser);
    }
    while (1) {
        printf("\n1. Login\n");
        printf("2. Create Account\n");
        printf("3. Exit\n");
        int choice;
        printf("Enter your choice: ");
        scanf("%d", &choice);
        if (choice == 1) {
            currentUser = login();
            int found = 0;
            for (int i = 0; i < numAccounts; i++) {
                if (strcmp(accounts[i].owner.name, currentUser.name) == 0 && strcmp(accounts[i].owner.surname, currentUser.surname) == 0) {
                    found = 1;
                    break;
                }
            }
            if (!found) {
                printf("Error: User not found in accounts.\n");
                strcpy(currentUser.name, "");
                strcpy(currentUser.surname, "");
            }
            else{
                printf("Login successful.\n\n");
                printf("Welcome Mr./Ms. %s %s.\n",currentUser.surname,currentUser.name);
                menu2(accounts, numAccounts, &currentUser);
            }
            
        } else if (choice == 2) {
            createAccount(accounts, &numAccounts, &currentUser, &successFlag);
        if (successFlag) {
        menu2(accounts, numAccounts, &currentUser);
        
    }
    
        } else if (choice == 3) {
            break;
        } else {
             printf("Invalid choice.\n");
             while(getchar() != '\n');
             continue;
        }
    }

    return 0;
}

