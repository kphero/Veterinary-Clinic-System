#ifndef CLINIC_H
#define CLINIC_H

//////////////////////////////////////
// Macros
//////////////////////////////////////

// Display formatting options
#define FMT_FORM 1
#define FMT_TABLE 2

// C Strings: array sizes
#define NAME_LEN 15
#define PHONE_DESC_LEN 4
#define PHONE_LEN 10


// Additional macro's:
#define JAN 1
#define DEC 12

#define HOUR_MAX 23
#define MINUTE_MAX 59

#define FIRST_HOUR 10
#define FIRST_MIN 00
#define LAST_HOUR 14
#define LAST_MIN 00
#define APPOINT_LENGTH 30

//////////////////////////////////////
// Structures
//////////////////////////////////////

// Data type: Phone
struct Phone
{
    char description[PHONE_DESC_LEN + 1];
    char number[PHONE_LEN + 1];
};

// Data type: Patient 
struct Patient
{
    int patientNumber;
    char name[NAME_LEN + 1];
    struct Phone phone;
};


// ------------------- MS#3 -------------------

// Data type: Time
struct Time
{
    int hour;
    int min;
};

// Data type: Date
struct Date
{
    int year;
    int month;
    int day;
};

// Data type: Appointment
struct Appointment
{
    int patientNumber;
    struct Time time;
    struct Date date;
};

// ClinicData type: Provided to student
struct ClinicData
{
    struct Patient* patients;
    int maxPatient;
    struct Appointment* appointments;
    int maxAppointments;
};



//////////////////////////////////////
// DISPLAY FUNCTIONS
//////////////////////////////////////

// Display's the patient table header (table format)
void displayPatientTableHeader(void);

// Displays a single patient record in FMT_FORM | FMT_TABLE format
void displayPatientData(const struct Patient* patient, int fmt);

// Display's appointment schedule headers (date-specific or all records)
void displayScheduleTableHeader(const struct Date* date, int isAllRecords);

// Display a single appointment record with patient info. in tabular format
void displayScheduleData(const struct Patient* patient,
                         const struct Appointment* appoint,
                         int includeDateField);


//////////////////////////////////////
// MENU & ITEM SELECTION FUNCTIONS
//////////////////////////////////////

// Menu: Main
void menuMain(struct ClinicData* data);

// Menu: Patient Management
void menuPatient(struct Patient patient[], int max);

// Menu: Patient edit
void menuPatientEdit(struct Patient* patient);

// Menu: Appointment Management
void menuAppointment(struct ClinicData* data);

// Display's all patient data in the FMT_FORM | FMT_TABLE format
void displayAllPatients(const struct Patient patient[], int max, int fmt);

// Search for a patient record based on patient number or phone number
void searchPatientData(const struct Patient patient[], int max);

// Add a new patient record to the patient array
void addPatient(struct Patient patient[], int max);

// Edit a patient record from the patient array
void editPatient(struct Patient patient[], int max);

// Remove a patient record from the patient array
void removePatient(struct Patient patient[], int max);

// View ALL scheduled appointments
void viewAllAppointments(struct ClinicData* data);

// View appointment schedule for the user input date
void viewAppointmentSchedule(struct ClinicData* data);

// Add an appointment record to the appointment array
void addAppointment(struct Appointment appoints[], int maxAppoints,
    struct Patient patients[], int maxPatients);

// Remove an appointment record from the appointment array
void removeAppointment(struct Appointment appoints[], int maxAppoints,
    struct Patient patients[], int maxPatients);


//////////////////////////////////////
// UTILITY FUNCTIONS
//////////////////////////////////////

// Search and display patient record by patient number (form)
void searchPatientByPatientNumber(const struct Patient patient[], int max);

// Search and display patient records by phone number (tabular)
void searchPatientByPhoneNumber(const struct Patient patient[], int max);

// Get the next highest patient number
int nextPatientNumber(const struct Patient patient[], int max);

// Find the patient array index by patient number (returns -1 if not found)
int findPatientIndexByPatientNum(int patientNumber,
                                 const struct Patient patient[], int max);

// Sort appointments by date lowest to highest
void sortAppointments(struct Appointment appoint[], int max);

// Compares two dates and return 0 if the same, -1 if apt1 < apt2 and 1 if apt1 > apt2
int compareDate(const struct Date* dt1, const struct Date* dt2);

// Compares two appointments and returns 0 if the same, -1 if apt1 < apt2 and 1 if apt1 > apt2
int compareDateTime(const struct Appointment* apt1, const struct Appointment* apt2);


//////////////////////////////////////
// USER INPUT FUNCTIONS
//////////////////////////////////////

// Get user input for a new patient record
void inputPatient(struct Patient* patient);

// Get user input for phone contact information
void inputPhoneData(struct Phone* phone);

// Get a user input for a year, month and day
void inputYearMonthDay(struct Date* date);

// Get a user input for an hour and minute within appointment hours
void inputHourMin(struct Time* time);

//////////////////////////////////////
// FILE FUNCTIONS
//////////////////////////////////////

// Import patient data from file into a Patient array (returns # of records read)
int importPatients(const char* datafile, struct Patient patients[], int max);

// Import appointment data from file into an Appointment array (returns # of records read)
int importAppointments(const char* datafile, struct Appointment appoints[], int max);

#endif // !CLINIC_H