# bitcoin-transactions

A virtual bank for handling bitcoin transactions written in vanilla C++.  (A project about data structures in C/C++.)

This application handles 3 types of entities `users` or `walets` , `bitcoins`  and `transactions` .  Each users has a collection of bitcoins in his/her possession. The value of every bitcoin is the same and is given as an argument. The users may make money transactions with each other, as long as the value of bitcoins in the senders possession are sufficient. That may lead to a situation where a bicoin is shared among multiple users. Consider the following example:

> Consider two users Alex and Sofia. Alex pays Sofia 2\$, but a single bitcoin's value is 10\$. Let's say Alex owns the bitcoin with `bicoinId`  10. Then the bitcoin 10 becomes shared among Alex and Sofia.

The application trucks the history of each bitcoin using a tree data structure.

## Input

The application takes 3 arguments as an input:

1. A **balance file**, where the users along with their collection of bitcoins is listed. Each record of the file is given in a single line and follows the following format:
   
   > <UserId>" "<bitcoinId_1>" "<bitcoinId_2>" "..." "<bitcoinId_n>
   
   For example a valid balance file record is the following:
   
   > Mia 123 337 880 667
   
   where 123, 337, 880 and 667 are bitcoin Ids.

2. A **transaction file**, which initializes the application with some transactions. Again Each record of the file is given in a single line and follows the format:
   
   > <TransactionId>" "<UserId_1>" "<UserId_2>" "<Amount>" "<day>-<month>-<year>" "<hour>:<minute>
   
   E.g.:
   
   > 889 Maria Ronaldo 50 25-12-2018 20:08

3. The **bitcoin value**.

The arguments are passed to the application with the following command:

`./bitcoin -a balance.in –t transactions.in -v bitcoin_value`

Additional transactions may be done via the application's prompt (see below).  These transactions can either be typed by the user in the prompt, or be loaded from a file. The **inline transactions file** follows a slightly different format from the initializing transaction file. We give the correct format bellow:

> <UserId_1>" "<UserId_2>" "<Amount>" "<day>-<month>-<year>" "<hour>:<minute>;

As you can see here the `transactionsId` is omitted (the application internally produces one) and many records can be given in a single line, separated by semicolon `;`. The usage of a different format is so that it matches the inline transaction format of the prompt (see bellow).

Example input data can be found at the directory `input_files` . There also can be found two Python3 scripts to produce randomly valid input files:

1. **create_input_files.py**, which takes as arguments the number of users, the number of transactions and the number of the bitcoins and produces valid balance and transaction files.

2. **create_inline_transactions_file.py**, which takes as arguments the number of users and the number of transactions and produces a valid inline transactions file.

## Prompt

In the application's prompt the following commands are provided:

1. `/requestTransaction` This command comes in multiple flavours. The programme recognises the format of the command's arguments and behaves analogously. The supported formats are the following:
   
   1. > /requestTransaction <UserId_1>" "<UserId_2>" "<Amount>" "<day>-<month>-<year>" "<hour>:<minute>
      
      for a single transaction.
   
   2. Or multiple transactions of the above format separated by semicolon `;` .
   
   3. Alternatively,  one can group multiple transactions of the above format, separated by semicolon into a file, and load the file.
      
      > /requestTransaction  inline_transactions.in

2. `/walletStatus` This command returns the available amount in a user's *wallet*. The command's syntax is the following:
   
   > /walletStatus <UserId>

3. `/bitCoinStatus` This command returns the bitcoin's initial value, the number of transaction's in which this bitcoin was used and it's unspent amount. The command's syntax is the following:
   
   > /bitCoinStatus <bitcoinId>

4. `/traceCoin` This command returns the bitcoin's history, all the transactions in which has been used.  The command's syntax is the following:
   
   > /traceCoin <bitcoinId>

5. `/exit` With this command the users exits the programme.  /exit command takes no arguments.

Some other commands are implemented for testing purposes:

1. `/dbg_print_all` This command prints all the programme's records (namely users, bitcoins, transactions). The command takes no arguments.

2. `/dbg_print_coin` This command prints a bitcoin's tree. Takes as argument a `bitcoinId` .

3. `/dbg_print_wallet` This command prints a user's wallet record. Takes as argument an `userId`.

## Notes

* This code is written in C++11 and compiled with [GNU g++ compiler](https://en.wikipedia.org/wiki/GNU_Compiler_Collection#Languages), version 7.5.0.
* To compile this application use the `make` command.
* A more detailed description of this assignment can be found in the file `hw1-spring-2019.pdf` (in Greek).
