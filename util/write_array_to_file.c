#include <stdio.h>

int main()
{
    // Data (array of numbers) to be written to the CSV file
    int numbers[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    int numRecords = sizeof(numbers) / sizeof(numbers[0]);

    // Open the CSV file for writing
    FILE *file = fopen("numbers.csv", "w");

    // Check if the file is opened successfully
    if (file == NULL)
    {
        printf("Error opening the file.\n");
        return 1;
    }

    // Write numbers to the CSV file
    for (int i = 0; i < numRecords; ++i)
    {
        // Add a comma after each number except the last one
        if (i < numRecords - 1)
        {
            fprintf(file, "%d,\n", numbers[i]);
        }
        else
        {
            // No comma after the last number
            fprintf(file, "%d\n", numbers[i]);
        }
    }

    // Close the CSV file
    fclose(file);

    printf("Data written to numbers.csv successfully.\n");

    return 0;
}
