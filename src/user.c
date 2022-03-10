#include "user.h"

long NumberOfUsers = 0;
User *UserData = NULL;
int tablesize = 4999;

/*takes the alphanumeric ID as a parameter and 
generates a key(integers) for hashing*/

long long hash(char ID[], int tablesize)
{
    const int p = 31;
    const int m = 1e9 + 9;
    long long key = 0;
    long long index;
    long long power = 1;

    //rolling hash is used here

    for (int i = 0; i < 10; i++)
    {
        key = (key + (ID[i] - '0' + 1) * power) % m;
        power = (power * p) % m;
    }

    return key;
}

/*takes an id as a parameter and generates a key 
and then we use hashing(quadratic probing) to search for our id in our hashtable*/

User *SearchUserByID(char *id)
{
    User *node = NULL;
    if(strlen(id)!=10){
        return NULL;
    }

    //no users have been added to the table
    if (UserData == NULL)
    {
        return NULL;
    }

    long long key = hash(id, tablesize);
    for (int i = 0; i < tablesize; i++)
    {
        //trying to avoid secondary clustering,tablesize is a prime number
        int newindex = ((key % tablesize) + i * (1 + (key % (tablesize - 1)))) % tablesize;
        //if a slot is empty , wallet balance would be -1
        if (UserData[newindex].WalletBalance != -1 && (strcmp(UserData[newindex].UniqueID, id) == 0))
        {
            node = &UserData[newindex];
            break;
        }
    }

    return node;
}

//generates a random alphanumeric user ID
char *RandomID(char *ID)
{
    static char string[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789123456789";

    //srand takes a seed from getpid (program ID) due to it being a better randomized seed

    if (!srand_flag)
        __initialisesrand();

    int length = sizeof(string) - 1;
    for (int i = 0; i < 10; i++)
    {
        ID[i] = string[rand() % length];
    }

    ID[10] = '\0';
    return ID;
}

//finds a newindex if there is a collision using quadratic probing
int quadprob(User *UserData, int tablesize, char ID[])
{

    long long key = hash(ID, tablesize);
    for (int i = 0; i < tablesize; i++)
    {
        int newindex = ((key % tablesize) + i * (1 + (key % (tablesize - 1)))) % tablesize;
        if (UserData[newindex].WalletBalance == -1)
        {
            return newindex;
        }
    }
}

//adds a user
int AddUser()
{
    //checks of there is an error in inputing walletbalance
    time_t t;
    time(&t); //current time

    //checks if a user has been added or not
    if (UserData == NULL)
    {
        UserData = (User *)calloc(tablesize, sizeof(User));

        for (int i = 0; i < tablesize; i++)
        {
            UserData[i].WalletBalance = -1;
        }
    } //Initial size of 4999 users(prime)
    else if (NumberOfUsers == 75000)
    {   
        //maximum limit reached
        return -1;
    }
    //this is for doubling the tablesize if the load factor
    //doubles the table size by reallocing and rehashing again
    //we double it because reallocing is resource intensive
    else if (NumberOfUsers * 2 > tablesize)
    {
        User TempUserArray[tablesize];
        for (int i = 0; i < tablesize; i++)
        {
            strcpy(TempUserArray[i].JoinDateTime, UserData[i].JoinDateTime);
            strcpy(TempUserArray[i].UniqueID, UserData[i].UniqueID);
            TempUserArray[i].UTH = UserData[i].UTH;
            TempUserArray[i].WalletBalance = UserData[i].WalletBalance;
        }

        free(UserData);
        UserData = (User *)calloc(2 * tablesize, sizeof(User));

        for (int i = 0; i < 2 * tablesize; i++)
        {
            UserData[i].WalletBalance = -1;
        }

        for (int i = 0; i < tablesize; i++)
        {
            if (TempUserArray[i].WalletBalance != -1)
            {
                int position = quadprob(UserData, 2 * tablesize, TempUserArray[i].UniqueID);
                strcpy(UserData[position].UniqueID, TempUserArray[i].UniqueID);
                strcpy(UserData[position].JoinDateTime, TempUserArray[i].JoinDateTime);
                UserData[position].WalletBalance = TempUserArray[i].WalletBalance;
                UserData[position].UTH = TempUserArray[i].UTH;
            }
        }
        tablesize = tablesize * 2;
    }

    char newID[25];

    RandomID(newID);
    int position = quadprob(UserData, tablesize, newID);

    strcpy(UserData[position].UniqueID, newID);
    strcpy(UserData[position].JoinDateTime, ctime(&t));
    UserData[position].WalletBalance = 1000; //Initial balance of 1000
    UserData[position].UTH = CreateUserTransactHistory(UserData[position].UTH);

    //keeps a count of how many users have been added which we can use to keep track of the load factor of the hashtable
    NumberOfUsers++;
    return position;
}