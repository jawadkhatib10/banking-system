#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_ACCOUNTS 100
#define MAX_NAME_LENGTH 50
#define MAX_SURNAME_LENGTH 50
#define MAX_IBAN_LENGTH 20
#define FILENAME "accounts.csv"
typedef struct {
    char name[MAX_NAME_LENGTH];
    char surname[MAX_SURNAME_LENGTH];
} Person;
typedef struct {
    char iban[MAX_IBAN_LENGTH];
    Person owner;
    char coin[4];
    double amount;
} Account;


void display(Account *accounts, int numAccounts, Person currentUser);
double getExchangeRate(const char* sourceCurrency, const char* destinationCurrency);
void readAccountsFromFile(Account *accounts, int *numAccounts);
void writeAccountsToFile(Account *accounts, int numAccounts);
Person login();
void createAccount(Account *accounts, int *numAccounts, Person*person, int *successFlag);
void editAccount(Account *accounts, int numAccounts, Person person);
void deleteAccount(Account *accounts, int *numAccounts, Person person);
void viewAccount(Account *accounts, int numAccounts, Person person);
void performTransaction(Account *accounts, int numAccounts, Person person);
void menu2(Account *accounts,int numAccounts,Person *currentUser);
void addAccount(Account *accounts, int *numAccounts, Person *currentUser, int *successFlag);

