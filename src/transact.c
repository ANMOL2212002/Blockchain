#include "user.h"

int NumberofTempTransactions = 0;

void InitCreateTransactHistory(pointer *Q)
{
    // allocates memory equal to the size of UserTransactionHistory to the head of transaction history from the heap.
    UserTransactionHistory *Head = (UserTransactionHistory *)malloc(sizeof(UserTransactionHistory));
    assert(Head != NULL);
    Head->next = NULL;
    Head->prev = NULL;
    Head->AmountTransferred = -1;
    //Head->SenderUID = NULL;

    Q->H = Head;

    // allocates memory equal to the size of UserTransactionHistory to the tail of transaction history from the heap.
    UserTransactionHistory *Tail = (UserTransactionHistory *)malloc(sizeof(UserTransactionHistory));
    assert(Tail != NULL);
    Tail->next = NULL;
    Tail->prev = NULL;
    Tail->AmountTransferred = -1;
    //Tail->SenderUID = NULL;

    Q->T = Tail;
}

pointer *CreateUserTransactHistory()
{
    // allocates memory equal to size of head and tail nodes(i.e., struct pointer)
    pointer *Q = (pointer *)malloc(sizeof(pointer));

    // calls InitCreateTransactHistory() passing the pointer to the previous allocated memory.
    // InitCreateTransactHistory updates Q by allocating memory for head and tail.
    InitCreateTransactHistory(Q);
    return Q;
}

void PrintUserTransactionHistory(char *UID)
{
    //Search user by UID and get a pointer to user(struct user).
    User *U = SearchUserByID(UID);

    //U == NULL condition is already checked in the calling function. So U will be valid

    //get pointer to user transaction history's head node.
    UserTransactionHistory *P = U->UTH->H;

    // since we are not storing transaction history in the head node so we will have to start from next node.
    // so we will check the condition that head and head->next are not null.
    // if they are null that means the user has not performed any transactions yet.
    if (P && P->next != NULL)
    {
        while (P && P->next != NULL)
        {
            P = P->next;
            if (P->TranscationType == 0)
                printf("Amount received : %Lg\nSender UID : %s \nTransaction time : %s\n", P->AmountTransferred, P->SenderUID, P->TransactionTime);
            else if (P->TranscationType == 1)
                printf("Amount sent : %Lg\nReceiver UID : %s \nTransaction time : %s\n", P->AmountTransferred, P->ReceiverUID, P->TransactionTime);
        }
    }
    else // no transactions performed by user.
        printf("This user hasn't performed any transactions.\n");
    printf("\n");
}

void PrintUserDetails(char *UID)
{
    //Search user by UID and get a pointer to user(struct user).

    if (UserData == NULL)
    {
        printf("No user has been added till now\n");
        return;
    }
    User *U = SearchUserByID(UID);

    //if the pointer is null that means there is no such user with that user id.
    if (U == NULL)
    {
        printf("\nInvalid User ID\n\n");
        return;
    }
    else // if userID is valid
    {
        printf("\nUser ID : %s\n", U->UniqueID);            // prints UserID.
        printf("Wallet Balance : %Lg\n", U->WalletBalance); // prints wallet balance
        printf("User join time : %s\n", U->JoinDateTime);   // prints join date time

        int ch;
        printf("Enter 1 if you wish to view the transaction history for this user. Else enter 0.\n");
        scanf("%d", &ch);

        if (ch == 1)
        {
            printf("\nThe transaction history is as follows - \n\n");

            //call PrintUserTransactionHistory which prints transaction history for the user.
            PrintUserTransactionHistory(UID);
        }
    }
}

void push(pointer *Q, long double AmountTransferred, char *SenderUID, char *ReceiverUID, int TransactionType)
{
    time_t t;
    time(&t);

    //Storing the user transactions as a doubly linked list

    if (Q->H->next == NULL) //Transaction history  IS EMPTY
    {
        UserTransactionHistory *temp = (UserTransactionHistory *)malloc(sizeof(UserTransactionHistory));
        if (temp == NULL)
        {
            printf("Memory full! Cannot add anymore transactions.\n");
            return;
        }
        temp->AmountTransferred = AmountTransferred;
        strcpy(temp->SenderUID, SenderUID);
        strcpy(temp->ReceiverUID, ReceiverUID);
        strcpy(temp->TransactionTime, ctime(&t));
        temp->TranscationType = TransactionType;
        temp->next = NULL;
        temp->prev = NULL;
        Q->H->next = temp;
        Q->T->prev = temp;
    }

    else //transaction history is not empty
    {
        UserTransactionHistory *temp = (UserTransactionHistory *)malloc(sizeof(UserTransactionHistory));
        if (temp == NULL)
        {
            printf("Memory full! Cannot add anymore transactions.\n");
            return;
        }
        temp->AmountTransferred = AmountTransferred;
        strcpy(temp->SenderUID, SenderUID);
        strcpy(temp->ReceiverUID, ReceiverUID);
        strcpy(temp->TransactionTime, ctime(&t));
        temp->TranscationType = TransactionType;
        temp->next = Q->H->next;
        temp->prev = NULL;

        Q->H->next->prev = temp;
        Q->H->next = temp;
    }
}

////////////////////////////////////////////////////////////////////////////////////////

int TransactionValidity(User *Sender, User *Receiver, long double AmountToBeTransferred)
{

    if (Sender == NULL) //Sender with given UID doesnot exist
    {
        return 2;
    }
    else if (Receiver == NULL) //Receiver with given UID doesnot exist
    {
        return 3;
    }
    else if (Sender == Receiver) //If sender and receiver IDs are same(transaction with self is invalid).
    {
        return 6;
    }
    else if (Sender->WalletBalance < AmountToBeTransferred) // Wallet balance of sender is lesser than the amount to be transferred.
    {
        return 4;
    }

    else if (AmountToBeTransferred <= 0) // amount of transaction is non positive
    {
        return 5;
    }
    return 1; //Everything is alright or transaction is valid.
}

void UpdateUserHistory(User *Sender, User *Receiver, char *SenderUID, char *ReceiverUID, long double AmountToBeTransferred)
{
    //push function updates user transaction history
    push(Sender->UTH, AmountToBeTransferred, SenderUID, ReceiverUID, 1);   // 1 -> amount was sent
    push(Receiver->UTH, AmountToBeTransferred, SenderUID, ReceiverUID, 0); // 0 -> amount was received

    //update user walletbalance
    Sender->WalletBalance = Sender->WalletBalance - AmountToBeTransferred;
    Receiver->WalletBalance = Receiver->WalletBalance + AmountToBeTransferred;
    printf("\nTransaction histories of both sender and receiver updated successfully.\n");
    return;
}

void UpdateBlockTransactionHistory(char *SenderUID, char *ReceiverUID, long double AmountToBeTransferred)
{
    time_t t;
    time(&t);

    if (TempTransactionArray == NULL) //if the pointer to array is null allocate memory for 50 transactions
    {
        TempTransactionArray = calloc(50, sizeof(BlockTransactionHistory));
    }

    //copy transaction details to the array.
    strcpy(TempTransactionArray[NumberofTempTransactions].SenderUID, SenderUID);
    strcpy(TempTransactionArray[NumberofTempTransactions].ReceiverUID, ReceiverUID);
    strcpy(TempTransactionArray[NumberofTempTransactions].TransactionTime, ctime(&t));
    TempTransactionArray[NumberofTempTransactions].AmountToBeTransferred = AmountToBeTransferred;

    NumberofTempTransactions++;
    if (NumberofTempTransactions == 50) //50 temporary transactions reached
    {   
        //multiple of 50 successful transactions reached
        //Creating a new block
        CreateBlock();
        
        //Temporary array now loses access to the 50 transactions it had stored till now
        TempTransactionArray = NULL;
        NumberofTempTransactions = 0; //Reset the  count
    }
}

void Transact()
{
    char SenderUID[40];
    char ReceiverUID[40];
    long double AmountToBeTransferred;
    //long long SenderUID, ReceiverUID, AmountToBeTransferred;

    printf("\nEnter the Sender User ID : ");
    scanf("%s", SenderUID);
    printf("\nEnter the Receiver User ID : ");
    scanf("%s", ReceiverUID);
    printf("\nEnter the amount to be transferred : ");
    scanf("%Lg", &AmountToBeTransferred);

    //First search users (sender and receiver) with given IDs(T->SenderUID and T-> ReceiverUID)
    //and get pointers to the users.
    User *Sender = SearchUserByID(SenderUID);
    User *Receiver = SearchUserByID(ReceiverUID);

    int isValid = TransactionValidity(Sender, Receiver, AmountToBeTransferred); //check transaction validity.

    if (isValid == 1) // transaction is valid
    {
        UpdateBlockTransactionHistory(SenderUID, ReceiverUID, AmountToBeTransferred);       //updates block transaction history array
        UpdateUserHistory(Sender, Receiver, SenderUID, ReceiverUID, AmountToBeTransferred); // updates user transaction history
        printf("Transaction successful.\n");
    }
    else if (isValid == 2)
    {
        printf("Sender does not exist in the system. Transaction aborted\n");
    }
    else if (isValid == 3)
    {
        printf("Receiver does not exist in the system. Transaction aborted\n");
    }
    else if (isValid == 4)
    {
        printf("Sender does not have enough wallet balance to complete this transaction. Transaction aborted\n");
    }
    else if (isValid == 5)
    {
        printf("Cannot transfer a non-positive value. Transaction aborted.\n");
    }
    else if (isValid == 6)
    {
        printf("Cannot transfer to self. Transaction aborted.\n");
    }
    return;
}

void RepTransact(char *SenderUID, char *ReceiverUID, long double AmountToBeTransferred)
{

    //First search users (sender and receiver) with given IDs(T->SenderUID and T-> ReceiverUID)
    //and get pointers to the users.
    User *Sender = SearchUserByID(SenderUID);
    User *Receiver = SearchUserByID(ReceiverUID);

    int isValid = TransactionValidity(Sender, Receiver, AmountToBeTransferred); //check transaction validity.

    if (isValid == 1) // transaction is valid
    {
        UpdateBlockTransactionHistory(SenderUID, ReceiverUID, AmountToBeTransferred);       //updates block transaction history array
        UpdateUserHistory(Sender, Receiver, SenderUID, ReceiverUID, AmountToBeTransferred); // updates user transaction history
        printf("Transaction successful.\n\n");
    }
    else if (isValid == 2)
    {
        printf("Sender does not exist in the system. Transaction aborted\n");
    }
    else if (isValid == 3)
    {
        printf("Receiver does not exist in the system. Transaction aborted\n");
    }
    else if (isValid == 4)
    {
        printf("Sender does not have enough wallet balance to complete this transaction. Transaction aborted\n");
    }
    else if (isValid == 5)
    {
        printf("Cannot transfer a non-positive value. Transaction aborted.\n");
    }
    else if (isValid == 6)
    {
        printf("Cannot transfer to self. Transaction aborted.\n");
    }
    return;
}
