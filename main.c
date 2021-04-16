#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "libnova/julian_day.h"

/*
 *      @brief findDate()
 *          - gets date from given arguments
 *      @param julianDateUTC
 *          - date and time coded in Julian Date code
 *          - UTC means time in zone UTC+0 (longitude = 0) - Greenwhich
 *      @param longitude
 *          - geographic coordinate specifying east-west position
 *            on the earth's surface
 *          - '+' means east, '-' means west
 *      @returnval
 *          - YYYY-MM-DD (str)
 */
char* findDate(double julianDateUTC, double longitude)
{
    // handle corrupted input
    if(longitude > 180.0f || longitude < -180.0f)
    {
        printf("Corrupted input.");
        exit(0);
    }

    // allocate memory for answer - 'string' date
    char format[] = "YYYY-MM-DD";
    int N = strlen(format) + 1;
    char *str = (char*)calloc(N, sizeof(char));
    if(str == NULL)
    {
        printf("Error allocating memory.");
        exit(0);
    }

    // convert longitude to seconds
    // 360degrees -> 24hours, then 1  deg -> 24/360hour->4min->240sec
    double seconds = longitude*240;  // includes '+' and '-' cases

    // get local time from given arguments local = UTC - longitudeShift
    const double secondsPerDay = 86400.0;
    double julianDateLocal = julianDateUTC - seconds / secondsPerDay;

    // convert from Julian Date to calendar date
    // using library found on the web
    struct ln_date date;
    ln_get_date(julianDateLocal, &date);

    //convert struct data to answer
    //year
    snprintf(str, 4, "%d", date.years);
    snprintf(str+4, 1, "%c", '-');
    //months
    if(date.months < 10)
        snprintf(str+5, 2, "0%d", date.months);
    else
        snprintf(str+5, 2, "%d", date.months);
    snprintf(str+7, 1, "%c", '-');
    //days
    if(date.days < 10)
        snprintf(str+8, 2, "0%d", date.days);
    else
        snprintf(str+8, 2, "%d", date.days);

    return str;
}

int main()
{
    // zad 2

    char *a = findDate(2451552.54097, 17.03);

    printf("%s", a);

    free(a);

    return 0;
}
