#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <ctype.h>
#include <string.h>

#include "core.h"

//////////////////////////////////////
// USER INTERFACE FUNCTIONS
//////////////////////////////////////

// Clear the standard input buffer
void clearInputBuffer(void)
{
    // Discard all remaining char's from the standard input buffer:
    while (getchar() != '\n')
    {
        ; // do nothing!
    }
}

// Wait for user to input the "enter" key to continue
void suspend(void)
{
    printf("<ENTER> to continue...");
    clearInputBuffer();
    putchar('\n');
}

// Ensures provided phone number is correct length
void displayFormattedPhone(const char* phoneNum)
{
    int i, digit;

    char s1[PHN_NUM], s2[PHN_NUM], s3[PHN_NUM];

    digit = 0;

    if (phoneNum != NULL)
    {
        for (i = 0; i < strlen(phoneNum); i++)
        {
            if (isdigit(phoneNum[i]))
            {
                digit++;
            }
        }

        if (digit == PHN_NUM)
        {
            sscanf(phoneNum, "%3s%3s%4s", s1, s2, s3);
            printf("(%s)%s-%s", s1, s2, s3);
        }
        else
        {
            printf("(___)___-____");
        }
    }
}

//////////////////////////////////////
// USER INPUT FUNCTIONS
//////////////////////////////////////

// Ensures user input is a whole number
int inputInt(void)
{
    char newLine;

    int input, valid;

    do
    {
        valid = 1;

        scanf("%d%c", &input, &newLine);

        if (newLine != '\n')
        {
            clearInputBuffer();

            printf("Error! Input a whole number: ");

            valid = 0;
        }
    } while (!valid);

    return input;
}

// Ensures user input is a positive integer
int inputIntPositive(void)
{
    int input, valid;

    do
    {
        valid = 1;

        input = inputInt();

        if (input < 0)
        {
            printf("ERROR! Value must be > 0: ");

            valid = 0;
        }
    } while (!valid);

    return input;
}

// Ensures user input is within the provided range
int inputIntRange(int start, int end)
{
    int input, valid;

    do
    {
        valid = 1;

        input = inputInt();

        if (input < start || input > end)
        {
            printf("ERROR! Value must be between %d and %d inclusive: ", start, end);

            valid = 0;
        }
    } while (!valid);

    return input;
}

// Ensures an inputted character is found within the provided string
char inputCharOption(const char* string)
{
    int i, found;
    char input, newLine;

    input = '0';

    if (string != NULL)
    {
        do
        {
            found = 0;

            scanf("%c%c", &input, &newLine);

            if (newLine != '\n')
            {
                clearInputBuffer();

                printf("ERROR: Character must be one of [%s]: ", string);
            }
            else
            {
                for (i = 0; i < strlen(string); i++)
                {
                    if (string[i] == input)
                    {
                        found = 1;
                    }
                }

                if (!found)
                {
                    printf("ERROR: Character must be one of [%s]: ", string);
                }
            }
        } while (!found);
    }

    return input;
}

// Ensures inputted string is within provided range
void inputCString(char* string, int min, int max)
{
    int valid;

    size_t length;

    char input[MAX_LEN + 1] = { 0 };
    char fmt[SCN_FMT + 1] = { 0 };

    if (string != NULL)
    {
        sprintf(fmt, "%%%d[^\n]", MAX_LEN);

        do
        {
            valid = 1;
            length = 0;

            scanf(fmt, input);
            clearInputBuffer();

            length = strlen(input);

            if ((length > max || length < min) && (min == max))
            {
                printf("Invalid %d-digit number! Number: ", max);

                valid = 0;
            }
            else if (length > max)
            {
                printf("ERROR: String length must be no more than %d chars: ", max);

                valid = 0;
            }
            else if (length < min)
            {
                printf("ERROR: String length must be between %d and %d chars: ", min, max);

                valid = 0;
            }
            else
            {
                strcpy(string, input);
            }
        } while (!valid);
    }
}