#include "dataStructers.h"

void display(Account *accounts, int numAccounts, Person currentUser) {
    double balnce=0;
    printf("Accounts for %s %s:\n", currentUser.surname, currentUser.name);
    printf("==================================\n");
    printf("%-20s %-10s\n", "IBAN", "Amount");
    printf("==================================\n");
    int foundAccounts = 0;
    for (int i = 0; i < numAccounts; i++) {
        if (strcmp(accounts[i].owner.name, currentUser.name) == 0 && strcmp(accounts[i].owner.surname, currentUser.surname) == 0) {
            double amountEUR = accounts[i].amount;
            amountEUR *= getExchangeRate(accounts[i].coin, "EUR");
            balnce+=amountEUR;
            printf("%-20s %.2lf %s\n", accounts[i].iban, accounts[i].amount, accounts[i].coin);
            foundAccounts = 1;
        }
    }
    if (!foundAccounts) {
        printf("No accounts found for %s %s.\n", currentUser.surname, currentUser.name);
    }
    printf("==================================\n");
    printf("Total Balance in EUR: %.2lf\n", balnce);
}

double getExchangeRate(const char* sourceCurrency, const char* destinationCurrency) {
    if (strcmp(sourceCurrency, "USD") == 0 && strcmp(destinationCurrency, "EUR") == 0) {
        return 0.85;
    } else if (strcmp(sourceCurrency, "USD") == 0 && strcmp(destinationCurrency, "RON") == 0) {
        return 4.15;
    } else if (strcmp(sourceCurrency, "EUR") == 0 && strcmp(destinationCurrency, "USD") == 0) {
        return 1.18;
    } else if (strcmp(sourceCurrency, "EUR") == 0 && strcmp(destinationCurrency, "RON") == 0) {
        return 4.88; 
    } else if (strcmp(sourceCurrency, "RON") == 0 && strcmp(destinationCurrency, "USD") == 0) {
        return 0.24;
    } else if (strcmp(sourceCurrency, "RON") == 0 && strcmp(destinationCurrency, "EUR") == 0) {
        return 0.21; 
    }
    return 1;
}
void readAccountsFromFile(Account *accounts, int *numAccounts) {
    FILE *file = fopen(FILENAME, "r");
    if (file == NULL) {
        printf("Error: Unable to open file %s.\n", FILENAME);
        exit(1);
    }
    *numAccounts = 0;
    while (fscanf(file, "%[^,],%[^,],%[^,],%lf,%[^\n]\n", accounts[*numAccounts].iban, accounts[*numAccounts].owner.name, accounts[*numAccounts].owner.surname, &accounts[*numAccounts].amount,accounts[*numAccounts].coin) == 5) {
        (*numAccounts)++;
    }
    fclose(file);
}
void writeAccountsToFile(Account *accounts, int numAccounts) {
    FILE *file = fopen(FILENAME, "w");
    if (file == NULL) {
        printf("Error: Unable to open file %s.\n", FILENAME);
        exit(1);
    }
    for (int i = 0; i < numAccounts; i++) {
        fprintf(file, "%s,%s,%s,%lf,%s\n", accounts[i].iban, accounts[i].owner.name, accounts[i].owner.surname, accounts[i].amount,accounts[i].coin);
    }
    fclose(file);
}
Person login() {
    Person person;
    printf("Enter your name: ");
    scanf("%s", person.name);
    printf("Enter your surname: ");
    scanf("%s", person.surname);
    return person;
}
void createAccount(Account *accounts, int *numAccounts, Person*person, int *successFlag) {
    if (*numAccounts >= MAX_ACCOUNTS) {
        printf("Error: Maximum number of accounts reached.\n");
        *successFlag = 0;
        return;
    }
    *person=login();
    for (int i = 0; i < *numAccounts; i++) {
        if (strcmp(accounts[i].owner.name, person->name) == 0 && strcmp(accounts[i].owner.surname, person->surname) == 0) {
            printf("%s %s is already taken. Please choose another name or surname .\n", person->name,person->surname);
            *successFlag = 0; 
            return;
        }
    }
    char validCurrencies[3][4] = {"RON", "EUR", "USD"};
    char currency[4];
    int validCurrency = 0;
    
    do {
        printf("Enter the currency (RON/EUR/USD): ");
        scanf("%s", currency);
        for (int i = 0; i < 3; i++) {
            if (strcmp(currency, validCurrencies[i]) == 0) {
                validCurrency = 1;
                break;
            }
        }
        if (!validCurrency) {
            printf("Error: Please enter a valid currency (RON, EUR, USD).\n");
        }
    } while (!validCurrency);
    
    strcpy(accounts[*numAccounts].coin, currency);
    strcpy(accounts[*numAccounts].owner.name, person->name);
    strcpy(accounts[*numAccounts].owner.surname, person->surname);
    sprintf(accounts[*numAccounts].iban, "RO%d", *numAccounts + 1);
    accounts[*numAccounts].amount = 0.0;
    (*numAccounts)++;
    
    printf("Account created successfully.\n");
    writeAccountsToFile(accounts, *numAccounts);
    
    *successFlag = 1; 
}

void addAccount(Account *accounts, int *numAccounts, Person *currentUser, int *successFlag) {
    if (*numAccounts >= MAX_ACCOUNTS) {
        printf("Error: Maximum number of accounts reached.\n");
        *successFlag = 0;
        return;
    }

    

    char validCurrencies[3][4] = {"RON", "EUR", "USD"};
    char currency[4];
    int validCurrency = 0;

    do {
        printf("Enter the currency (RON/EUR/USD): ");
        scanf("%s", currency);
        for (int i = 0; i < 3; i++) {
            if (strcmp(currency, validCurrencies[i]) == 0) {
                validCurrency = 1;
                break;
            }
        }
        if (!validCurrency) {
            printf("Error: Please enter a valid currency (RON, EUR, USD).\n");
        }
    } while (!validCurrency);

    // Add the new account
    strcpy(accounts[*numAccounts].coin, currency);
    strcpy(accounts[*numAccounts].owner.name, currentUser->name);
    strcpy(accounts[*numAccounts].owner.surname, currentUser->surname);
    sprintf(accounts[*numAccounts].iban, "RO%d", *numAccounts + 1);
    accounts[*numAccounts].amount = 0.0;
    (*numAccounts)++;

    printf("Account created successfully.\n");
    writeAccountsToFile(accounts, *numAccounts);

    *successFlag = 1;
}



void editAccount(Account *accounts, int numAccounts, Person person) {
    char iban[MAX_IBAN_LENGTH];
    
    printf("\n Enter the IBAN of the account you want to edit: ");
    scanf("%s", iban);
    for (int i = 0; i < numAccounts; i++) {
        if (strcmp(accounts[i].iban, iban) == 0 && strcmp(accounts[i].owner.name, person.name) == 0 && strcmp(accounts[i].owner.surname, person.surname) == 0) {
            printf("1. Edit IBAN\n");
            printf("2. Edit amount\n");
            printf("3. Edit currency\n");
            printf("Enter your choice: ");
            int choice;
            scanf("%d", &choice);
            if (choice == 1) {
                printf("Enter the new IBAN: ");
                scanf("%s", accounts[i].iban);
                printf("Account IBAN edited successfully.\n");
            } else if (choice == 2) {
                printf("Enter the new amount: ");
                scanf("%lf", &accounts[i].amount);
                printf("Account amount edited successfully.\n");
            } else if (choice == 3) {
                char validCurrencies[3][4] = {"RON", "EUR", "USD"};
                char currency[4];
                int validCurrency = 0;
                do {
                    printf("Enter the currency (RON/EUR/USD): ");
                    scanf("%s", currency);
                    for (int i = 0; i < 3; i++) {
                        if (strcmp(currency, validCurrencies[i]) == 0) {
                            validCurrency = 1;
                            break;
                        }
                    }
                    if (!validCurrency) {
                        printf("Error: Please enter a valid currency (RON, EUR, USD).\n");
                    }
                } while (!validCurrency);
                accounts[i].amount *= getExchangeRate(accounts[i].coin, currency);
                strcpy(accounts[i].coin, currency);
                printf("Account currency edited successfully.\n");
            } else {
                printf("Invalid choice.\n");
            }
            return;
        }
    }
    printf("Error: Account not found or you don't have access to edit this account.\n");
}
void deleteAccount(Account *accounts, int *numAccounts, Person person) {
    char iban[MAX_IBAN_LENGTH];
    printf("\n Enter the IBAN of the account you want to delete: ");
    scanf("%s", iban);
    for (int i = 0; i < *numAccounts; i++) {
        if (strcmp(accounts[i].iban, iban) == 0 && strcmp(accounts[i].owner.name, person.name) == 0 && strcmp(accounts[i].owner.surname, person.surname) == 0) {
            for (int j = i; j < *numAccounts - 1; j++) {
                accounts[j] = accounts[j + 1];
            }
            (*numAccounts)--;
            printf("Account deleted successfully.\n");
            int userAccounts = 0;
            for (int i = 0; i < *numAccounts; i++) {
                if (strcmp(accounts[i].owner.name, person.name) == 0 && strcmp(accounts[i].owner.surname, person.surname) == 0) {
                    userAccounts++;
                }
            }
            if (userAccounts == 0) {
                printf("No accounts left for user. Exiting program.\n");
                writeAccountsToFile(accounts, *numAccounts);
                exit(0);
            }
            return;
        }
    }
    printf("Error: Account not found or you don't have access to delete this account.\n");
}
void viewAccount(Account *accounts, int numAccounts, Person person) {
    char iban[MAX_IBAN_LENGTH];
    printf("\n Enter the IBAN of the account you want to view: ");
    scanf("%s", iban);
    for (int i = 0; i < numAccounts; i++) {
        if (strcmp(accounts[i].iban, iban) == 0 && strcmp(accounts[i].owner.name, person.name) == 0 && strcmp(accounts[i].owner.surname, person.surname) == 0) {
            printf("IBAN: %s\n", accounts[i].iban);
            printf("Owner: %s %s\n", accounts[i].owner.name, accounts[i].owner.surname);
            printf("Currency: %s\n", accounts[i].coin);
            printf("Amount: %.2lf\n", accounts[i].amount);
            return;
        }
    }
    printf("Error: Account not found or you don't have access to view this account.\n");
}
void performTransaction(Account *accounts, int numAccounts, Person person) {
    char sourceIBAN[MAX_IBAN_LENGTH];
    char destinationIBAN[MAX_IBAN_LENGTH];
    double amount;
    printf("\nEnter source IBAN: ");
    scanf("%s", sourceIBAN);
    printf("Enter destination IBAN: ");
    scanf("%s", destinationIBAN);
    printf("Enter amount to transfer: ");
    scanf("%lf", &amount);
    Account *sourceAccount = NULL;
    Account *destinationAccount = NULL;
    for (int i = 0; i < numAccounts; i++) {
        if (strcmp(accounts[i].iban, sourceIBAN) == 0 && strcmp(accounts[i].owner.name, person.name) == 0 && strcmp(accounts[i].owner.surname, person.surname) == 0) {
            sourceAccount = &accounts[i];
        }
        if (strcmp(accounts[i].iban, destinationIBAN) == 0) {
            destinationAccount = &accounts[i];
        }
    }
    if (sourceAccount == NULL || destinationAccount == NULL) {
        printf("Error: Invalid source or destination account.\n");
        return;
    }
    if (sourceAccount->amount < amount) {
        printf("Error: Insufficient funds in the source account.\n");
        return;
    }
    double amm=amount;
    sourceAccount->amount -= amount;
    amount*=getExchangeRate(sourceAccount->coin,destinationAccount->coin);
    destinationAccount->amount += amount;
    printf("Transaction successful.\n");
    printf("%lf %s been trasnfer from %s to %s \n",amm,sourceAccount->coin,sourceIBAN,destinationIBAN);
    printf("your balnce is %lf",sourceAccount->amount);
}

void menu2(Account *accounts,int numAccounts,Person *currentUser){
    int choice;
    while (1) {
        printf("\n1. Edit Account\n");
        printf("2. view all Accounts\n");
        printf("3. View Account\n");
        printf("4. add Account\n");
        printf("5. Delete Account\n");
        printf("6. Perform Transaction\n");
        printf("7. Logout\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        if (choice == 1) {
            editAccount(accounts, numAccounts, *currentUser);
            writeAccountsToFile(accounts, numAccounts);
        }else if (choice == 4){
            int successFlag;
            addAccount(accounts, &numAccounts, currentUser, &successFlag);
            if (successFlag) {
                printf("Account added successfully.\n");
            }
            writeAccountsToFile(accounts, numAccounts);
        }else if (choice == 2){
            display(accounts, numAccounts, *currentUser);
        }else if (choice == 5) {
            deleteAccount(accounts, &numAccounts, *currentUser);
            writeAccountsToFile(accounts, numAccounts);
        } else if (choice ==3) {
            viewAccount(accounts, numAccounts, *currentUser);
        } else if (choice == 6) {
            performTransaction(accounts, numAccounts, *currentUser);
            writeAccountsToFile(accounts, numAccounts);
        } else if (choice == 7) {
            printf("goodBye %s %s.\n",currentUser->name,currentUser->surname);
            strcpy(currentUser->name, "");
            strcpy(currentUser->surname, "");
            return;
        } else {
            printf("Invalid choice.\n");
        }
    }
}
