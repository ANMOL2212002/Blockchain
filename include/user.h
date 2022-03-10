#ifndef __USER_DATA_H_
#define __USER_DATA_H_

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>
#include <unistd.h>
#include <openssl/sha.h>

#include "block.h"
#include "transact.h"

int AddUser();
//Adds a user to the system

char *RandomID(char *ID);
//Generates a random user ID for the newly added user

long long hash(char ID[], int tablesize);
int quadprob(User *UserData, int tablesize, char ID[]);

User *SearchUserByID(char *ID);
//Searches the database for the using with UniqueID ID

extern int tablesize;
extern User *UserData;   //Array of users
extern long NumberOfUsers;   //Keeps track of the number of users in the  system (and also to check if its time to double table size)
//if array reaches a certain capacity, realloc the array with double the size

#endif