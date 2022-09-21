# Command line bank program with efficient database

## Introduction
This is the final project for the data structures course. In this project, I was asked to create an object-oriented C++ program with no pre-defined data structures and libraries capable of doing multiple tasks in the best theoretical time complexity. Besides, the application must be able to save and restore data from the previously saved state.

## Commands and Time complexities
All users should have an ID, which is the smallest 5-digit number available, and a first and last name, which no two different users have the same first and last name. 

Assuming that the current number of users is N,
- Finding the richest user: O(1)
```
INPUT:
SEARCH RICHEST

OUTPUT:
<first_name > <last_name > <account_no > <assets >
```
- Inserting a new user: O(log(N))
```
INPUT:
REGISTER <first_name > <last_name > <assets >

OUTPUT:
SUCCESS
```
- Changing a user's asset: O(log(N))
```
INPUT:
CHANGE <account_no > +/-<value >

OUTPUT:
<first_name > <last_name > <assets > <prev_assets > <new_assets >
```
- Deleting a user: O(log(N))
```
INPUT:
DELETE <account_no >

OUTPUT:
SUCCESS
```
- Finding a user by ID or name: O(log(N))
```
INPUT:
SEARCH <account_no >

OUTPUT:
<first_name > <last_name > <assets >
```
- Finding all users with negative assets: O(N)
```
INPUT:
SEARCH MINUS

OUTPUT:
<first_name > <last_name > <account_no > <assets >
```
- Showing all users with assets more than a threshold: O(N)
```
INPUT:
SEARCH GT <value >

OUTPUT:
<first_name > <last_name > <account_no > <assets >
```
- Save and Restore in O(N(log(N)) [N = count of all users ever created]
```
INPUT
SAVE
```
or
```
INPUT
LOAD
```

## Commands

