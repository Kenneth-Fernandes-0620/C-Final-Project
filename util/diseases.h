#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef DISEASES_H
#define DISEASES_H
#endif

int DISEASES = 0;
char **DISEASES_ARRAY;

int load_diseases();
void parse_diseases(char *, char);
void free_diseases_array();

int load_diseases()
{
    FILE *file = fopen("data/diseases.csv", "r");
    if (file == NULL)
    {
        printf("Error opening the file.\n");
        return 0;
    }

    char line[256];
    fgets(line, sizeof(line), file);
    parse_diseases(line, ',');

    fclose(file);
    return 1;
}

void parse_diseases(char *line, char seperator)
{

    char *token, *temp = line;

    while (*temp != '\0')
        *(temp++) == ',' ? DISEASES++ : 0;

    DISEASES_ARRAY = (char **)malloc(DISEASES * sizeof(char *));
    token = strtok(line, &seperator);
    int i = 0;

    while (token != NULL)
    {
        DISEASES_ARRAY[i] = (char *)malloc((strlen(token) + 1) * sizeof(char));
        strcpy(DISEASES_ARRAY[i], token);
        token = strtok(NULL, ",");
        i++;
    }
}

void free_diseases_array()
{
    for (int i = 0; i < DISEASES; i++)
        free(DISEASES_ARRAY[i]);
    free(DISEASES_ARRAY);
}
