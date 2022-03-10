#ifndef __TRANSACT_H_
#define __TRANSACT_H_

#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <assert.h>
#include <openssl/sha.h>

typedef struct User User;
typedef struct UserTransactionHistory UserTransactionHistory;
typedef struct pointer pointer;

struct User
{
    char JoinDateTime[40];
    char UniqueID[12];
    long double WalletBalance;
    pointer *UTH;
};

struct UserTransactionHistory
{
    long double AmountTransferred;
    char SenderUID[12];
    char ReceiverUID[12];
    char TransactionTime[40];
    int TranscationType; // 0 -> Amount Received, 1 -> Amount Sent
    UserTransactionHistory *next;   //Used to maintain doubly linked list
    UserTransactionHistory *prev;   //Used to maintain doubly linked list
};

struct pointer
{
    UserTransactionHistory *H;  //Used to maintain doubly linked list
    UserTransactionHistory *T;  //Used to maintain doubly linked list
};

extern int NumberofTempTransactions;

int TransactionValidity(User *Sender, User *Receiver, long double AmountToBeTransferred);
//returns 1 if transaction is valid else returns 2,3,4.. depending on why transaction is invalid.

void UpdateUserHistory(User *Sender, User *Receiver, char *SenderUID, char *ReceiverUID, long double AmountToBeTransferred);
// updates user transaction history (and WalletBalance)of both sender and receiver.

pointer *CreateUserTransactHistory();
// for creating user transaction history after adding the user,it allocates memory to struct pointer containing head and tail nodes and calls 
//InitCreateTransactHistory and returns pointer to struct pointer

void InitCreateTransactHistory(pointer *Q);
// this function is called in pointer *CreateUserTransactHistory() and it allocates memory

void PrintUserTransactionHistory(char *UID);
//prints user transaction history in order from latest to oldest by taking user id as parameter.

void PrintUserDetails(char *UID);
//prints user details like ID,wallet balance and join date time. Also asks if the user wants to get his transaction history printed and prints if user gives
//it command to print

void push(pointer *Q, long double AmountTransferred, char *SenderUID, char *ReceiverUID, int TransactionType);
//Insert item(transaction info) on the front end of the list(user transaction history).

void UpdateBlockTransactionHistory(char *SenderUID, char *ReceiverUID, long double AmountToBeTransferred);
//updates temporary transaction array that store stransactions until we get 50 transactions and then calls create block. and starts from 0 transactions again.

void Transact();
//main transaction function which takes input and calls the other functions to perform the transactions.

void RepTransact(char *SenderUID, char *ReceiverUID, long double AmountToBeTransferred);
//transaction function that performs a particular function n number of times

#endif
