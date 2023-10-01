#include <stdio.h>
#include <string.h>
#include "strtokTest.h"

struct Patient *read_file(const char *);

int FILE_LINES = -1;

int main()
{
    struct Patient *p1 = read_file("data.csv");
    // printf("File has %d lines", FILE_LINES);
    for (int i = 0; i < FILE_LINES; i++)
    {
        printf("Name: %s\n", p1[i].name);
    }
}

struct Patient *read_file(const char *file_name)
{
    FILE *file = fopen(file_name, "r");

    if (file == NULL)
    {
        printf("Error opening the file.\n");
        return NULL;
    }

    int lines = 0;
    char line[256];
    while (fgets(line, sizeof(line), file))
        lines++;

    fseek(file, 0, SEEK_SET);

    // struct Patient patients[lines];
    struct Patient *patients = (struct Patient *)malloc(sizeof(struct Patient) * lines);

    FILE_LINES = lines;
    lines = 0;

    while (fgets(line, sizeof(line), file))
    {
        // Remove the trailing newline character
        line[strcspn(line, "\n")] = '\0';
        patients[lines] = parse_string(line, ',');
        lines++;
    }

    fclose(file);
    return patients;
}