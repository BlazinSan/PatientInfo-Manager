#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Functions to identify inputs and outputs
int calculateAge(char *icNumber);
char* determineGender(char *icNumber);
char* determineBirthState(char *icNumber);
void saveToFile(char *patientRecord);

int main() {
    // Variables to store patient information and their character limits
    char icNumber[13];
    char name[50];
    char address[50];
    char contactNo[15];

    // Get patient information from user input
    printf("--------------------------------------------------------------------\n");
    printf("                        PATIENT REGISTRATION\n");
    printf("--------------------------------------------------------------------\n \n");
    printf("Enter Patient IC Number : ");
    scanf("%s", icNumber);
    printf("Enter Patient Name      : ");
    getchar(); // Consume newline character
    fgets(name, sizeof(name), stdin);
    name[strcspn(name, "\n")] = '\0'; // Remove newline character
    printf("Enter Patient Address   : ");
    fgets(address, sizeof(address), stdin);
    address[strcspn(address, "\n")] = '\0'; // Remove newline character
    printf("Enter Patient Contact No: ");
    scanf("%s", contactNo);

    // Calculate gender, birth state, and age based on IC number format from http://www.malaysiacentral.com/information-directory/mykad-identity-number-mykad-the-malaysia-government-multipurpose-smart-identity-card/
    char *gender = determineGender(icNumber);
    char *birthState = determineBirthState(icNumber);
    int age = calculateAge(icNumber);

    // Get current date
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    char currentDate[20];
    strftime(currentDate, sizeof(currentDate), "%d/%m/%Y", &tm);

    // Display patient record
    printf("\n \n--------------------------------------------------------------------\n");
    printf("                           PATIENT RECORD\n");
    printf("--------------------------------------------------------------------\n \n");
    printf("Name                    : %s\n", name);
    printf("IC Number               : %s\n", icNumber);
    printf("Birth date (dd/mm/yyyy) : %c%c/%c%c/19%c%c\n", icNumber[4], icNumber[5], icNumber[2], icNumber[3], icNumber[0], icNumber[1]);
    printf("Age                     : %d\n", age);
    printf("Gender                  : %s\n", gender);
    printf("Birth State             : %s\n", birthState);
    printf("Contact Number          : %s\n", contactNo);
    printf("Address                 : %s\n", address);
    printf("Display date            : %s\n", currentDate);

    // Save patient record to file
    char patientRecord[500];
    sprintf(patientRecord, "IC Number: %s\nName: %s\nAddress: %s\nContact No: ", icNumber, name, address, contactNo);
    strcat(patientRecord, "Age: ");
    char ageStr[5];
    sprintf(ageStr, "%d", age);
    strcat(patientRecord, ageStr); // Save age
    strcat(patientRecord, "\nGender: ");
    strcat(patientRecord, gender); // Save gender
    strcat(patientRecord, "\nBirth State: ");
    strcat(patientRecord, birthState); // Save birth state
    strcat(patientRecord, "\n");

    saveToFile(patientRecord);

    return 0;
}

// Function to calculate age based on IC number format
int calculateAge(char *icNumber) {
    // Extract birth year from IC number format
    int birthYear = (icNumber[0] - '0') * 10 + (icNumber[1] - '0');
    if (icNumber[0] == '9' || icNumber[0] == '8') {
        birthYear += 1900;
    } else {
        birthYear += 2000;
    }

    // Get current year
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    int currentYear = tm.tm_year + 1900;

    // Calculate age
    return currentYear - birthYear;
}

// Function to determine gender based on IC number format
char* determineGender(char *icNumber) {
    // Extract gender number from IC number format which will specify whether the patient is Male or Female
    int genderDigit = icNumber[11] - '0';
    if (genderDigit % 2 == 0) {
        return "Female";
    } else {
        return "Male";
    }
}

// Function to determine birth state based on IC number format
char* determineBirthState(char *icNumber) {
    // Extract state code from IC number
    char stateCode[3];
    strncpy(stateCode, icNumber + 6, 2);
    stateCode[2] = '\0';

    // Define state names based on state codes, refrence: http://www.malaysiacentral.com/information-directory/mykad-identity-number-mykad-the-malaysia-government-multipurpose-smart-identity-card/
    char *states[] = {"Johor", "Kedah", "Kelantan", "Malacca", "Negeri Sembilan",
                      "Pahang", "Penang", "Perak", "Perlis", "Selangor",
                      "Terengganu", "Sabah", "Sarawak", "Federal Territory of Kuala Lumpur",
                      "Federal Territory of Labuan", "Federal Territory of Putrajaya"};

    // Convert state code to index and return state name
    int index = atoi(stateCode);
    return states[index - 1];
}

// Function to save patient record to file
void saveToFile(char *patientRecord) {
    FILE *file = fopen("patient_record.txt", "w");
    if (file == NULL) {
        printf("Error opening file.\n");
        exit(1);
    }

    fprintf(file, "%s", patientRecord);
    fclose(file);
}
