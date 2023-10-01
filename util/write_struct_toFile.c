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
    // Predefined array of structures
    struct Patient patients[] = {
        {"Alice", 30, "Female", "1234567890"},
        {"Bob", 40, "Male", "2345678901"},
        {"Eve", 35, "Female", "3456789012"}};

    int numPatients = sizeof(patients) / sizeof(patients[0]);

    // Open the file for writing
    FILE *file = fopen("patients.txt", "w");

    // Check if the file is opened successfully
    if (file == NULL)
    {
        printf("Error opening the file.\n");
        return 1;
    }

    // Write the array of structures to the file
    fwrite(patients, sizeof(struct Patient), numPatients, file);

    // Close the file
    fclose(file);

    printf("Data written to patients.txt successfully.\n");

    return 0;
}
