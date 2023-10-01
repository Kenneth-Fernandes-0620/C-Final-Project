#include <stdio.h>
#include <string.h>

struct Patient
{
    char name[50];
    int age;
    char gender[6];
    char phone[10];
};

int main()
{
    // Open the file for reading
    FILE *file = fopen("patients.txt", "r");

    // Check if the file is opened successfully
    if (file == NULL)
    {
        printf("Error opening the file.\n");
        return 1;
    }

    // Read the number of patients in the file
    int numPatients;
    fread(&numPatients, sizeof(int), 1, file);

    // Read the array of structures from the file
    struct Patient patients[numPatients];
    fread(patients, sizeof(struct Patient), numPatients, file);

    // Close the file
    fclose(file);

    // Print details of each patient
    printf("Details of Patients:\n");
    for (int i = 0; i < numPatients; ++i)
    {
        printf("Patient %d:\n", i + 1);
        printf("Name: %s\n", patients[i].name);
        printf("Age: %d\n", patients[i].age);
        printf("Gender: %s\n", patients[i].gender);
        printf("Phone: %s\n", patients[i].phone);
        printf("\n");
    }

    return 0;
}
