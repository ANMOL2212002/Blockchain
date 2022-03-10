# DSA-project- Blockchain

Blockchain is a program that shows a few functionalites of a block chain network.This Project aims to give you a smaller picture of Bitcoin and in no way a complete
representation of Bitcoin.

&nbsp;

## I. Pre-requisites

You need to install **openssl** library to make this work.
You can install it by using

- `sudo apt-get install libssl-dev`

&nbsp;
## II. Instructions To Run The Program

<br>

### 1. Clone the repo onto your local machine
```
git clone https://github.com/pallavkoppisetti/DSA-project.git
```

### 2. Compile the code
Type in `make` and all the files(.c and .h) will be compiled with the help of a makefile and the object files will be stored in `src/obj`.


### 3. Run the compiled code
```
./blockchain
```
To delete all .o files, run `make clean`.


### 4. Input commands
Once the code is up and running, a menu appears, asking for the input corresponding to the function wished to execute. A command can be chosen from the list of features available.
You can type in `list` for the program to list out all the commands available.

&nbsp;
## III. Features/Commands
<br>

>  Note: The program asks you if you want to continue running it , typing in `no` will exit out of the program and `yes` will continue running it.

### 1. adduser
Adds a user to our hashtable that stores all of the user info. and assigns a randomly generated ID and prints it out.
The user starts out with a default wallet balance of 1000.

<br>

### 2. transact / reptransact
Initiates a transaction. Asks for the SenderID, ReceiverID and amount to be transferred.If the transaction is a success, the program adds it to the current block.

> Note: You will receive an error message if any of the user inputs are invalid. Transaction will automatically abort.

> Note: The function reptransact is very similar to this function wherein it just repeats the same transaction a certain number of times, which can be decided by the user.

<br>

### 3. attack

The function performs an attack on the blockchain. Outputs a message which tells whether the attack was successful or not.

<br>

### 4. validateblockchain

This function checks whether any block was attacked previously. If it finds that a block was attacked, it corrects the block to the original state.

<br>

### 5. userdetails
This function asks for a UserID and prints out all the details of that particular user including wallet balance, jointime , ID and also has an option of view the user's transaction history, provided you give a valid/existing user ID.

<br>

### 6. blockdetails
This function ask for a block number and prints out the details of that particular block such as block number, transactions, previous block hash and nonce.

<br>

### 7. addmultiuser
This function helps us in adding multiple users at once, similar to adduser but here the program asks for how many users we want to add and gives all of them a randomly generated ID.

<br>

### 8. multitransact
This function is very similar to transact, the only difference being that using *multitransact* we can perform multiple transactions at once.

<br>

### 9. list
This function simply lists out all the commands that are available.

<br>

### 10.exit
Exits the program

&nbsp;
