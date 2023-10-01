#include <ctype.h>
#include <string.h>
#include <stdio.h>

#ifndef HELPER_H
#define HELPER_H
#endif

int validate_mobile_number(const char mobile_number[], int number_length)
{
    if (number_length == 10)
        return 1;
    // Validate if the number contains only digits and optional special characters
    for (int i = 0; i < number_length; i++)
        if (!(!isdigit(mobile_number[i]) || mobile_number[i] != ' ' || mobile_number[i] != '-' || mobile_number[i] != '+'))
            return 0;
    return 1;
}

int validate_age(const char age[], int age_length)
{
    if (age_length > 2 || age_length == 0)
        return 0;

    if (!isdigit(age[0]) || !isdigit(age[1]))
        return 0;

    // converting the string to integer representation
    int tempAge = ((age[0] - '0') * 10) + (age[1] - '0');

    if (tempAge < 1 || tempAge > 100)
        return 0;
    return 1;
}

int validate_name(const char username[], int name_length)
{
    if (name_length == 0)
        return 0;
    for (int i = 0; i < name_length - 1; i++)
    {
        if (!isalpha(username[i]) && !isspace(username[i]))
            return 0;
    }
    return 1;
}

int isValidDate(const char *date, const char *current_date)
{
    int day, month, year;
    if (sscanf(date, "%2d-%2d-%4d", &day, &month, &year) != 3)
        return 0;
    else if ((day < 1 || day > 31) && (month < 1 || month > 12) && (year < 2000 || year > 2100))
        return 0;
    return 1;
}