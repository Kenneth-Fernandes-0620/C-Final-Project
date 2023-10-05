#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#ifndef HELPER_H
#define HELPER_H
#endif

typedef struct
{
    int day;
    int month;
    int year;
} Date;

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
        if (!isalpha(username[i]) && !isspace(username[i]))
            return 0;
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

// Function to check if a given year is a leap year
int is_leap_year(int year)
{
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

// Function to validate a date
int is_valid_date(Date date)
{
    if (date.year < 1 || date.month < 1 || date.month > 12 || date.day < 1)
        return 0;

    int maxDays = 31;

    if (date.month == 4 || date.month == 6 || date.month == 9 || date.month == 11)
        maxDays = 30;
    else if (date.month == 2)
        maxDays = is_leap_year(date.year) ? 29 : 28;
    return date.day <= maxDays;
}

// Function to parse a date from a string in "dd-mm-yyyy" format
Date parse_date(char *dateStr)
{
    Date date;
    sscanf(dateStr, "%d-%d-%d", &date.day, &date.month, &date.year);
    return date;
}

// Function to print a date

char *date_to_string(Date date)
{
    // Create a buffer to store the formatted date
    char *dateString = (char *)malloc(11); // 10 characters for date (dd-mm-yyyy) + 1 for null-terminator

    // Check if memory allocation was successful
    if (dateString == NULL)
    {
        fprintf(stderr, "Memory allocation failed\n");
        return NULL
    }

    // Format the date into the buffer
    snprintf(dateString, 11, "%02d-%02d-%04d", date.day, date.month, date.year);

    // Return the formatted date string
    return dateString;
}

// Function to calculate the date one week ahead
Date one_week_ahead(Date date)
{
    date.day += 7;

    if (date.day > 31)
    {
        date.day -= 31;
        date.month++;
        if (date.month > 12)
        {
            date.month = 1;
            date.year++;
        }
    }

    return date;
}

char *get_one_week_ahead(char *dateStr)
{
    Date userDate = parse_date(dateStr);

    if (is_valid_date(userDate))
    {
        Date currentDate;
        currentDate.day = 5;
        currentDate.month = 10;
        currentDate.year = 2023;

        printf("Parsed date: ");
        date_to_string(userDate);

        if (userDate.year < currentDate.year ||
            (userDate.year == currentDate.year && userDate.month < currentDate.month) ||
            (userDate.year == currentDate.year && userDate.month == currentDate.month && userDate.day < currentDate.day))

            printf("Entered date should be greater than or equal to today's date.\n");
        else
        {
            Date nextWeek = one_week_ahead(userDate);
            printf("Date one week ahead: ");
            date_to_string(nextWeek);
        }
    }
    else
        return NULL;

    return 0;
}

void free_up_validator()
{
    free(date_to_string);
}