#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#ifndef DOCTORS_H
#define DOCTORS_H
#endif

enum DoctorType
{
    heartDoc,
    eyeDoc,
    skinDoc,
    teethDoc,
    nerveDoc,
    kidneyDoc,
    liverDoc,
    earDoc,
    boneDoc,
    mentalDoc,
};

enum DoctorType get_enum(const char *string_enum)
{
    if (strcmp(string_enum, "heartDoc") == 0)
        return heartDoc;
    else if (strcmp(string_enum, "eyeDoc") == 0)
        return eyeDoc;
    else if (strcmp(string_enum, "skinDoc") == 0)
        return skinDoc;
    else if (strcmp(string_enum, "teethDoc") == 0)
        return teethDoc;
    else if (strcmp(string_enum, "nerveDoc") == 0)
        return nerveDoc;
    else if (strcmp(string_enum, "kidneyDoc") == 0)
        return kidneyDoc;
    else if (strcmp(string_enum, "liverDoc") == 0)
        return liverDoc;
    else if (strcmp(string_enum, "earDoc") == 0)
        return earDoc;
    else if (strcmp(string_enum, "boneDoc") == 0)
        return boneDoc;
    else
        return mentalDoc;
}

int DOCTORS = -1;

struct Doctor
{
    char name[20];
    // TODO: Use Enum for Doctor Type;
    char type[20];
} *doctors_array;

struct Doctor parse_string(char inputString[], char seperator)
{
    char *token;
    struct Doctor p1;
    token = strtok(inputString, &seperator);

    int i = 0;
    while (token != NULL)
    {
        if (i == 0)
            strcpy(p1.name, token);
        else if (i == 1)
            strcpy(p1.type, token);
        token = strtok(NULL, ",");
        i++;
    }
    return p1;
}

int load_doctors(const char *file_name)
{
    FILE *file = fopen(file_name, "r");

    if (file == NULL)
    {
        printf("Error opening the file.\n");
        return 0;
    }

    int lines = 0;
    char line[256];
    while (fgets(line, sizeof(line), file))
        lines++;

    doctors_array = (struct Doctor *)malloc(sizeof(struct Doctor) * lines);
    fseek(file, 0, SEEK_SET);

    DOCTORS = lines;
    lines = 0;

    while (fgets(line, sizeof(line), file))
    {
        line[strcspn(line, "\n")] = '\0';
        doctors_array[lines] = parse_string(line, ',');
        lines++;
    }

    fclose(file);
    return 1;
}

void free_doctors_array()
{
    free(doctors_array);
}