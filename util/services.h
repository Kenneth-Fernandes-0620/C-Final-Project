
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef SERVICES_H
#define SERVICES_H
#endif

int SERVICES = 0;
char **SERVICES_ARRAY;

int load_services();
void parse_services(char *, char);
void free_services_array();

int load_services()
{
    FILE *file = fopen("data/services.csv", "r");
    if (file == NULL)
    {
        printf("Error opening the file.\n");
        return 0;
    }

    char line[256];
    fgets(line, sizeof(line), file);
    parse_services(line, ',');

    fclose(file);
    return 1;
}

void parse_services(char *line, char seperator)
{

    char *token, *temp = line;

    while (*temp != '\0')
        *(temp++) == ',' ? SERVICES++ : 0;

    SERVICES_ARRAY = (char **)malloc(SERVICES * sizeof(char *));
    token = strtok(line, &seperator);
    int i = 0;

    while (token != NULL)
    {
        SERVICES_ARRAY[i] = (char *)malloc((strlen(token) + 1) * sizeof(char));
        strcpy(SERVICES_ARRAY[i], token);
        token = strtok(NULL, ",");
        i++;
    }
}

void free_services_array()
{
    for (int i = 0; i < SERVICES; i++)
        free(SERVICES_ARRAY[i]);
    free(SERVICES_ARRAY);
}
