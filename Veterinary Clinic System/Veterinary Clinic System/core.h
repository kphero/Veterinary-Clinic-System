#ifndef CORE_H
#define CORE_H

//////////////////////////////////////
// DEFINITIONS AND MACROS
//////////////////////////////////////

#define MAX_LEN 1500
#define SCN_FMT 20
#define PHN_NUM 10

//////////////////////////////////////
// USER INTERFACE FUNCTIONS
//////////////////////////////////////

// Clear the standard input buffer
void clearInputBuffer(void);

// Wait for user to input the "enter" key to continue
void suspend(void);

// Ensures provided phone number is correct length
void displayFormattedPhone(const char* phoneNum);

//////////////////////////////////////
// USER INPUT FUNCTIONS
//////////////////////////////////////

// Ensures user input is a whole number
int inputInt(void);

// Ensures user input is a positive integer
int inputIntPositive(void);

// Ensures user input is within the provided range
int inputIntRange(int start, int end);

// Ensures an inputted character is found within the provided string
char inputCharOption(const char* string);

// Ensures inputted string is within provided range
void inputCString(char* string, int min, int max);

#endif // !CORE_H
