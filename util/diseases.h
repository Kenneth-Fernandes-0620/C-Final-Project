#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef DISEASES_H
#define DISEASES_H
#endif

int diseases_rows = 0, diseases_columns = 0;
char **diseases_array;

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
        *(temp++) == ',' ? diseases_rows++ : 0;

    diseases_array = (char **)malloc(diseases_rows * sizeof(char *));
    token = strtok(line, &seperator);
    int i = 0;

    while (token != NULL)
    {
        diseases_array[i] = (char *)malloc((strlen(token) + 1) * sizeof(char));
        strcpy(diseases_array[i], token);
        token = strtok(NULL, ",");
        i++;
    }
}

void free_diseases_array()
{
    for (int i = 0; i < diseases_rows; i++)
        free(diseases_array[i]);
    free(diseases_array);
}
