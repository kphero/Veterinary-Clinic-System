#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string.h>
#include "core.h"
#include "clinic.h"


//////////////////////////////////////
// DISPLAY FUNCTIONS
//////////////////////////////////////

// Display's the patient table header (table format)
void displayPatientTableHeader(void)
{
    printf("Pat.# Name            Phone#\n"
           "----- --------------- --------------------\n");
}

// Displays a single patient record in FMT_FORM | FMT_TABLE format
void displayPatientData(const struct Patient* patient, int fmt)
{
    if (fmt == FMT_FORM)
    {
        printf("Name  : %s\n"
               "Number: %05d\n"
               "Phone : ", patient->name, patient->patientNumber);
        displayFormattedPhone(patient->phone.number);
        printf(" (%s)\n", patient->phone.description);
    }
    else
    {
        printf("%05d %-15s ", patient->patientNumber,
               patient->name);
        displayFormattedPhone(patient->phone.number);
        printf(" (%s)\n", patient->phone.description);
    }
}

// Display's appointment schedule headers (date-specific or all records)
void displayScheduleTableHeader(const struct Date* date, int isAllRecords)
{
    printf("Clinic Appointments for the Date: ");

    if (isAllRecords)
    {
        printf("<ALL>\n\n");
        printf("Date       Time  Pat.# Name            Phone#\n"
               "---------- ----- ----- --------------- --------------------\n");
    }
    else
    {
        printf("%04d-%02d-%02d\n\n", date->year, date->month, date->day);
        printf("Time  Pat.# Name            Phone#\n"
               "----- ----- --------------- --------------------\n");
    }
}

// Display a single appointment record with patient info. in tabular format
void displayScheduleData(const struct Patient* patient,
                         const struct Appointment* appoint,
                         int includeDateField)
{
    if (includeDateField)
    {
        printf("%04d-%02d-%02d ", appoint->date.year, appoint->date.month,
               appoint->date.day);
    }
    printf("%02d:%02d %05d %-15s ", appoint->time.hour, appoint->time.min,
           patient->patientNumber, patient->name);

    displayFormattedPhone(patient->phone.number);

    printf(" (%s)\n", patient->phone.description);
}


//////////////////////////////////////
// MENU & ITEM SELECTION FUNCTIONS
//////////////////////////////////////

// main menu
void menuMain(struct ClinicData* data)
{
    int selection;

    do {
        printf("Veterinary Clinic System\n"
               "=========================\n"
               "1) PATIENT     Management\n"
               "2) APPOINTMENT Management\n"
               "-------------------------\n"
               "0) Exit System\n"
               "-------------------------\n"
               "Selection: ");
        selection = inputIntRange(0, 2);
        putchar('\n');
        switch (selection)
        {
        case 0:
            printf("Are you sure you want to exit? (y|n): ");
            selection = !(inputCharOption("yn") == 'y');
            putchar('\n');
            if (!selection)
            {
                printf("Exiting system... Goodbye.\n\n");
            }
            break;
        case 1:
            menuPatient(data->patients, data->maxPatient);
            break;
        case 2:
            menuAppointment(data);
            break;
        }
    } while (selection);
}

// Menu: Patient Management
void menuPatient(struct Patient patient[], int max)
{
    int selection;

    do {
        printf("Patient Management\n"
               "=========================\n"
               "1) VIEW   Patient Data\n"
               "2) SEARCH Patients\n"
               "3) ADD    Patient\n"
               "4) EDIT   Patient\n"
               "5) REMOVE Patient\n"
               "-------------------------\n"
               "0) Previous menu\n"
               "-------------------------\n"
               "Selection: ");
        selection = inputIntRange(0, 5);
        putchar('\n');
        switch (selection)
        {
        case 1:
            displayAllPatients(patient, max, FMT_TABLE);
            suspend();
            break;
        case 2:
            searchPatientData(patient, max);
            break;
        case 3:
            addPatient(patient, max);
            suspend();
            break;
        case 4:
            editPatient(patient, max);
            break;
        case 5:
            removePatient(patient, max);
            suspend();
            break;
        }
    } while (selection);
}

// Menu: Patient edit
void menuPatientEdit(struct Patient* patient)
{
    int selection;

    do {
        printf("Edit Patient (%05d)\n"
               "=========================\n"
               "1) NAME : %s\n"
               "2) PHONE: ", patient->patientNumber, patient->name);
        
        displayFormattedPhone(patient->phone.number);
        
        printf("\n"
               "-------------------------\n"
               "0) Previous menu\n"
               "-------------------------\n"
               "Selection: ");
        selection = inputIntRange(0, 2);
        putchar('\n');

        if (selection == 1)
        {
            printf("Name  : ");
            inputCString(patient->name, 1, NAME_LEN);
            putchar('\n');
            printf("Patient record updated!\n\n");
        }
        else if (selection == 2)
        {
            inputPhoneData(&patient->phone);
            printf("Patient record updated!\n\n");
        }

    } while (selection);
}

// Menu: Appointment Management
void menuAppointment(struct ClinicData* data)
{
    int selection;

    do {
        printf("Appointment Management\n"
               "==============================\n"
               "1) VIEW   ALL Appointments\n"
               "2) VIEW   Appointments by DATE\n"
               "3) ADD    Appointment\n"
               "4) REMOVE Appointment\n"
               "------------------------------\n"
               "0) Previous menu\n"
               "------------------------------\n"
               "Selection: ");
        selection = inputIntRange(0, 4);
        putchar('\n');
        switch (selection)
        {
        case 1:
            viewAllAppointments(data);
            suspend();
            break;
        case 2:
            viewAppointmentSchedule(data);
            suspend();
            break;
        case 3:
            addAppointment(data->appointments, data->maxAppointments,
                           data->patients, data->maxPatient);
            suspend();
            break;
        case 4:
            removeAppointment(data->appointments, data->maxAppointments,
                              data->patients, data->maxPatient);
            suspend();
            break;
        }
    } while (selection);
}

// Display's all patient data in the FMT_FORM | FMT_TABLE format
void displayAllPatients(const struct Patient patient[], int max, int fmt)
{
    int i, records;

    if (fmt == FMT_TABLE)
    {
        displayPatientTableHeader();
    }

    records = 0;

    for (i = 0; i < max; i++)
    {
        if (patient[i].patientNumber)
        {
            displayPatientData(&patient[i], fmt);
            records++;
        }
    }

    if (!records)
    {
        putchar('\n');
        printf("*** No records found ***\n");
    }
    putchar('\n');
}

// Search for a patient record based on patient number or phone number
void searchPatientData(const struct Patient patient[], int max)
{
    int selection;

    do
    {
        printf("Search Options\n"
            "==========================\n"
            "1) By patient number\n"
            "2) By phone number\n"
            "..........................\n"
            "0) Previous menu\n"
            "..........................\n"
            "Selection: ");

        selection = inputIntRange(0, 2);
        putchar('\n');

        switch (selection)
        {
        case 1:
            searchPatientByPatientNumber(patient, max);
            suspend();
            break;
        case 2:
            searchPatientByPhoneNumber(patient, max);
            suspend();
            break;
        }
    } while (selection);
}

// Add a new patient record to the patient array
void addPatient(struct Patient patient[], int max)
{
    int i, index, found;

    found = 0;

    for (i = 0; i < max && !found; i++)
    {
        if (!patient[i].patientNumber)
        {
            index = i;
            found = 1;
        }
    }

    if (!found)
    {
        printf("ERROR: Patient listing is FULL!\n\n");
    }
    else
    {
        patient[index].patientNumber = nextPatientNumber(patient, max);

        inputPatient(&patient[index]);

        printf("*** New patient record added ***\n\n");
    }
}

// Edit a patient record from the patient array
void editPatient(struct Patient patient[], int max)
{
    int num, index;

    printf("Enter the patient number: ");

    num = inputIntPositive();
    putchar('\n');

    index = findPatientIndexByPatientNum(num, patient, max);

    if (index >= 0 && num > 0)
    {
        menuPatientEdit(&patient[index]);
    }
    else
    {
        printf("ERROR: Patient record not found!\n\n");
    }
}

// Remove a patient record from the patient array
void removePatient(struct Patient patient[], int max)
{
    int num, index;

    char input;

    struct Patient empty = { 0 };

    printf("Enter the patient number: ");

    num = inputIntPositive();
    putchar('\n');

    index = findPatientIndexByPatientNum(num, patient, max);

    if (index >= 0 && num > 0)
    {
        displayPatientData(&patient[index], FMT_FORM);
        putchar('\n');
        printf("Are you sure you want to remove this patient record? (y/n): ");

        input = inputCharOption("yn");

        if (input == 'y')
        {
            patient[index] = empty;
            printf("Patient record has been removed!\n");
        }
        else
        {
            printf("Operation aborted.\n");
        }
    }
    else
    {
        printf("ERROR: Patient record not found!\n");
    }
    putchar('\n');
}

// View ALL scheduled appointments
void viewAllAppointments(struct ClinicData* data)
{
    int i, j;

    displayScheduleTableHeader(NULL, 1);

    for (i = 0; i < data->maxAppointments; i++)
    {
        for (j = 0; j < data->maxPatient; j++)
        {
            if (data->appointments[i].patientNumber == data->patients[j].patientNumber)
            {
                displayScheduleData(&data->patients[j], &data->appointments[i], 1);
            }
        }
    }
    putchar('\n');
}


// View appointment schedule for the user input date
void viewAppointmentSchedule(struct ClinicData* data)
{
    int i, j, tableIdx;
    
    struct Date tempDate = { 0 };

    inputYearMonthDay(&tempDate);
    putchar('\n');

    tableIdx = -1;

    for (i = 0; i < data->maxAppointments; i++)
    {
        if (compareDate(&tempDate, &data->appointments[i].date) == 0)
        {
            tableIdx = i;
        }
    }

    if (tableIdx > -1)
    {
        displayScheduleTableHeader(&data->appointments[tableIdx].date, 0);

        for (i = 0; i < data->maxAppointments; i++)
        {
            if (compareDate(&tempDate, &data->appointments[i].date) == 0)
            {
                for (j = 0; j < data->maxPatient && data->patients[j].patientNumber > 0; j++)
                {
                    if (data->appointments[i].patientNumber == data->patients[j].patientNumber)
                    {
                        displayScheduleData(&data->patients[j], &data->appointments[i], 0);
                    }
                }
            }
        }
        putchar('\n');
    }
    else
    {
        printf("No appointments found.\n\n");
    }
}


// Add an appointment record to the appointment array
void addAppointment(struct Appointment appoints[], int maxAppoints,
    struct Patient patients[], int maxPatients)
{
    int i, index, next, validTime;

    struct Appointment temp = { 0 };

    next = -1;

    for (i = 0; (i < maxAppoints) & (next == -1); i++)
    {
        if (!(appoints[i].patientNumber > 0))
        {
            next = i;
        }
    }

    if (next < 0)
    {
        printf("ERROR: Appointment slots are full!\n\n");
    }
    else
    {
        printf("Patient Number: ");
        temp.patientNumber = inputIntPositive();

        index = findPatientIndexByPatientNum(temp.patientNumber, patients, maxPatients);

        if (index == -1)
        {
            printf("ERROR: Patient record not found!\n\n");
        }
        else
        {
            do
            {
                validTime = 1;

                inputYearMonthDay(&temp.date);
                inputHourMin(&temp.time);

                for (i = 0; i < maxAppoints; i++)
                {
                    if (compareDateTime(&temp, &appoints[i]) == 0)
                    {
                        validTime = 0;
                    }
                }

                putchar('\n');

                if (!validTime)
                {
                    printf("ERROR: Appointment timeslot is not available!\n\n");
                }
                else
                {
                    appoints[next] = temp;

                    sortAppointments(appoints, maxAppoints);

                    printf("*** Appointment scheduled! ***\n\n");
                }
            } while (!validTime);
        }
    }   
}


// Remove an appointment record from the appointment array
void removeAppointment(struct Appointment appoints[], int maxAppoints,
    struct Patient patients[], int maxPatients)
{
    int i, index, valid;

    char input;

    struct Appointment temp = { 0 }, empty = { 0 };

    printf("Patient Number: ");
    temp.patientNumber = inputIntPositive();

    index = findPatientIndexByPatientNum(temp.patientNumber, patients, maxPatients);

    if (index == -1)
    {
        printf("ERROR: Patient record not found!\n\n");
    }
    else
    {
        inputYearMonthDay(&temp.date);
        putchar('\n');

        valid = 0;

        for (i = 0; i < maxAppoints; i++)
        {
            if (temp.patientNumber == appoints[i].patientNumber && compareDate(&temp.date, &appoints[i].date) == 0)
            {
                displayPatientData(&patients[index], FMT_FORM);
                printf("Are you sure you want to remove this appointment (y,n): ");

                input = inputCharOption("yn");

                valid = 1;

                if (input == 'y')
                {
                    appoints[i] = empty;

                    sortAppointments(appoints, maxAppoints);

                    putchar('\n');
                    printf("Appointment record has been removed!\n\n");
                }
                else
                {
                    printf("Operation aborted.\n\n");
                }
            }
        }

        if (!valid)
        {
            printf("ERROR: Appointment record not found!\n\n");
        }
    }
}


//////////////////////////////////////
// UTILITY FUNCTIONS
//////////////////////////////////////

// Search and display patient record by patient number (form)
void searchPatientByPatientNumber(const struct Patient patient[], int max)
{
    int num, found;

    printf("Search by patient number: ");

    num = inputIntPositive();
    putchar('\n');

    found = findPatientIndexByPatientNum(num, patient, max);

    if (found > -1 && num > 0)
    {
        displayPatientData(&patient[found], FMT_FORM);
    }
    else
    {
        printf("*** No records found ***\n");
    }
    putchar('\n');
}

// Search and display patient records by phone number (tabular)
void searchPatientByPhoneNumber(const struct Patient patient[], int max)
{
    int i, found;

    char num[PHONE_LEN + 1] = { 0 };

    printf("Search by phone number: ");

    inputCString(num, PHONE_LEN, PHONE_LEN);
    putchar('\n');

    displayPatientTableHeader();

    found = 0;

    for (i = 0; i < max; i++)
    {
        if (strcmp(patient[i].phone.number, num) == 0)
        {
            displayPatientData(&patient[i], FMT_TABLE);

            found++;
        }
    }

    if (!found)
    {
        putchar('\n');
        printf("*** No records found ***\n");
    }
    putchar('\n');
}

// Get the next highest patient number
int nextPatientNumber(const struct Patient patient[], int max)
{
    int i, highest;

    highest = 0;

    for (i = 0; i < max; i++)
    {
        if (patient[i].patientNumber > highest)
        {
            highest = patient[i].patientNumber;
        }
    }

    return highest + 1;
}

// Find the patient array index by patient number (returns -1 if not found)
int findPatientIndexByPatientNum(int patientNumber,
    const struct Patient patient[], int max)
{
    int i, index;

    index = -1;

    for (i = 0; i < max && index == -1; i++)
    {
        if (patient[i].patientNumber == patientNumber)
        {
            index = i;
        }
    }

    return index;
}

// Sort appointments by date lowest to highest
void sortAppointments(struct Appointment appoint[], int max)
{
    int i, j, minIdx;

    struct Appointment temp = { 0 };

    if (appoint != NULL)
    {
        for (i = 0; i < max; i++)
        {
            minIdx = i;

            for (j = i + 1; j < max; j++)
            {
                if (compareDateTime(&appoint[j], &appoint[minIdx]) < 0)
                {
                    minIdx = j;
                }
            }

            if (minIdx != i)
            {
                temp = appoint[i];
                appoint[i] = appoint[minIdx];
                appoint[minIdx] = temp;
            }
        }
    }
}

// Compares two dates and return 0 if the same, -1 if apt1 < apt2 and 1 if apt1 > apt2
int compareDate(const struct Date* dt1, const struct Date* dt2)
{
    int result = 0;

    int date1, date2;

    if (dt1 != NULL && dt2 != NULL)
    {
        date1 = dt1->year * 10000 + dt1->month * 100 + dt1->day;
        date2 = dt2->year * 10000 + dt2->month * 100 + dt2->day;

        if (!(date1 == date2))
        {
            if (date1 < date2)
            {
                result = -1;
            }
            else
            {
                result = 1;
            }
        }
    }

    return result;
}

// Compares two appointments and return 0 if the same, -1 if apt1 < apt2 and 1 if apt1 > apt2
int compareDateTime(const struct Appointment* apt1, const struct Appointment* apt2)
{
    int result = 0;

    int date1, date2, time1, time2;

    if (apt1 != NULL && apt2 != NULL)
    {
        date1 = apt1->date.year * 10000 + apt1->date.month * 100 + apt1->date.day;
        date2 = apt2->date.year * 10000 + apt2->date.month * 100 + apt2->date.day;
        time1 = apt1->time.hour * 60 + apt1->time.min;
        time2 = apt2->time.hour * 60 + apt2->time.min;

        if (!((date1 == date2) && (time1 == time2)))
        {
            if (date1 < date2)
            {
                result = -1;
            }
            else if (date1 > date2)
            {
                result = 1;
            }
            else if (time1 < time2)
            {
                result = -1;
            }
            else
            {
                result = 1;
            }
        }
    }

    return result;
}


//////////////////////////////////////
// USER INPUT FUNCTIONS
//////////////////////////////////////

// Get user input for a new patient record
void inputPatient(struct Patient* patient)
{
    printf("Patient Data Input\n"
        "------------------\n"
        "Number: %05d\n"
        "Name  : ", patient->patientNumber);

    inputCString(patient->name, 1, NAME_LEN);
    putchar('\n');

    inputPhoneData(&patient->phone);
}

// Get user input for phone contact information
void inputPhoneData(struct Phone* phone)
{
    int input;

    printf("Phone Information\n"
        "-----------------\n"
        "How will the patient like to be contacted?\n"
        "1. Cell\n"
        "2. Home\n"
        "3. Work\n"
        "4. TBD\n"
        "Selection: ");

    input = inputIntRange(1, 4);

    switch (input)
    {
    case 1:
        strcpy(phone->description, "CELL");
        break;
    case 2:
        strcpy(phone->description, "HOME");
        break;
    case 3:
        strcpy(phone->description, "WORK");
        break;
    case 4:
        strcpy(phone->description, "TBD");
        break;
    }
    putchar('\n');

    if (input != 4)
    {
        printf("Contact: %s\n"
            "Number : ", phone->description);

        inputCString(phone->number, PHONE_LEN, PHONE_LEN);
        putchar('\n');
    }
    else
    {
        strcpy(phone->number, "\0");
    }
}

// Get a user input for a year, month and day
void inputYearMonthDay(struct Date* date)
{
    if (date != NULL)
    {
        printf("Year        : ");
        date->year = inputIntPositive();

        printf("Month (%d-%d): ", JAN, DEC);
        date->month = inputIntRange(JAN, DEC);

        if (date->month == 2)
        {
            if ((date->year % 4 == 0 && date->year % 100 != 0) || (date->year % 400 == 0))
            {
                printf("Day (1-29)  : ");
                date->day = inputIntRange(1, 29);
            }
            else
            {
                printf("Day (1-28)  : ");
                date->day = inputIntRange(1, 28);
            }
        }
        else if (date->month == 4 || date->month == 6 || date->month == 9 || date->month == 11)
        {
            printf("Day (1-30)  : ");
            date->day = inputIntRange(1, 30);
        }
        else
        {
            printf("Day (1-31)  : ");
            date->day = inputIntRange(1, 31);
        }
    }
}

// Get a user input for an hour and minute within appointment hours
void inputHourMin(struct Time* time)
{
    int validInterval;

    if (time != NULL)
    {
        do
        {
            validInterval = 1;

            printf("Hour (0-%d)  : ", HOUR_MAX);
            time->hour = inputIntRange(0, HOUR_MAX);
            printf("Minute (0-%d): ", MINUTE_MAX);
            time->min = inputIntRange(0, MINUTE_MAX);

            if ((time->hour < FIRST_HOUR || time->hour > LAST_HOUR || time->min % APPOINT_LENGTH != 0) ||
                (time->hour == LAST_HOUR && time->min > LAST_MIN))
            {
                printf("ERROR: Time must be between %02d:%02d and %02d:%02d in %02d minute intervals.\n\n",
                    FIRST_HOUR, FIRST_MIN, LAST_HOUR, LAST_MIN, APPOINT_LENGTH);

                validInterval = 0;
            }

        } while (!validInterval);
    }
}

//////////////////////////////////////
// FILE FUNCTIONS
//////////////////////////////////////

// Import patient data from file into a Patient array (returns # of records read)
int importPatients(const char* datafile, struct Patient patients[], int max)
{
    int i, num = 0;

    FILE* fp = NULL;
    fp = fopen(datafile, "r");

    if (fp != NULL)
    {
        for (i = 0; i < max && fscanf(fp, "%d", &patients[i].patientNumber) == 1; i++)
        {
            fscanf(fp, "|%[^|]|%[^|]|%[^\n]",
                patients[i].name, patients[i].phone.description, patients[i].phone.number);

            if (patients[i].patientNumber)
            {
                num++;
            }
        }

        fclose(fp);
        fp = NULL;
    }

    return num;
}

// Import appointment data from file into an Appointment array (returns # of records read)
int importAppointments(const char* datafile, struct Appointment appoints[], int max)
{
    int i, num = 0;

    FILE* fp = NULL;
    fp = fopen(datafile, "r");

    if (fp != NULL)
    {
        for (i = 0; i < max && fscanf(fp, "%d", &appoints[i].patientNumber) == 1; i++)
        {

            fscanf(fp, ",%d,%d,%d,%d,%d",
                &appoints[i].date.year, &appoints[i].date.month, &appoints[i].date.day, 
                &appoints[i].time.hour, &appoints[i].time.min);

            if (appoints[i].patientNumber)
            {
                num++;
            }
        }

        fclose(fp);
        fp = NULL;
    }

    sortAppointments(appoints, max);

    return num;
}
