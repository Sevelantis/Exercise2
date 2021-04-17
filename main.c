#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct datetime
{
    int years;
    int months;
    int days;
    int hours;
    int minutes;
    double seconds;
};

/*
 *      @brief datetimeToStr()
 *          - creates string in format 'YYYY-MM-DD' from given datetime structure
 *      @param str
 *          - actual string - answer
 *      @param d
 *          - datetime
 *      @returnval
 */
void datetimeToStr(char *str, struct datetime *d)
{
    //year
    snprintf(str, 5, "%d", d->years);

    // '-'
    snprintf(str+4, 2, "%c", '-');
    
    //months
    if(d->months < 10)
        snprintf(str+5, 3, "0%d", d->months);
    else
        snprintf(str+5, 3, "%d", d->months);
    
    // '-'
    snprintf(str+7, 2, "%c", '-');
    
    //days
    if(d->days < 10)
        snprintf(str+8, 3, "0%d", d->days);
    else
        snprintf(str+8, 3, "%d", d->days);
}

/*
 *      @brief dmod()
 *          - gets modulo from given doubles
 *      @param x
 *          - double
 *      @param y
 *          - double
 *      @returnval
 *          - x % y
 */
double dmod(double x, double y) 
{
    return x - (int)(x/y) * y;
}

/*
 *      @brief julianToDatetime()
 *          - gets modulo from given doubles
 *      @param JD
 *          - year, month, date, hour, minute, second coded in Julian Date code
 *      @param d
 *          - datetime struct
 *      @returnval
 */
void julianToDatetime(double JD, struct datetime *d)
{
    // constants
    double DAYS_IN_YEAR  = 365.25;
    double DAYS_IN_MONTH = 30.61;
    int SECONDS_IN_DAY= 86400;

    // decimal part contains number of days
    // frac part contains hours, minutes, seconds of a day
    double W = JD + 0.5;                            // night begins at noon
    double dec = (int)W;                               // decimal part
    double frac = W - dec;                          // fractional part
    double Y = (int)((dec + 32044.5) / 36524.25);
    double Z = dec + Y - (int)  (Y / 4) - 38;
    double A = Z + 1524;
    double B = (int)  ((A - 122.1) / DAYS_IN_YEAR);
    double C = A - (int)  (DAYS_IN_YEAR * B);
    double E = (int)  (C / DAYS_IN_MONTH);
    double F = (int)  (E / 14);
    d->years = B - 4716 + F;
    d->months = E - 1 - 12 * F ;
    d->days = C + frac - (int)  (153 * E / 5);

    double secondsTotal = frac * SECONDS_IN_DAY;
    double minutesTotal = secondsTotal/60;

    d->hours   = (int)secondsTotal / 3600;
    d->minutes = (int)minutesTotal % 60;
    d->seconds = dmod(secondsTotal, 60);
}

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
        printf("Incorrect input (longitude).");
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
    double minutes = longitude*4;  // includes '+' and '-' cases

    // get local time from given arguments local = UTC + longitudeShift
    const double minutesPerDay = 1440.0;
    double julianDateLocal = julianDateUTC + minutes / minutesPerDay;

    // convert from Julian Date to calendar date
    struct datetime date;
    julianToDatetime(julianDateLocal, &date);

    //convert struct data to answer
    datetimeToStr(str, &date);

    return str;
}

int main(int argc, char *argv[])
{
    // handle optional input arguments
    // arg1 is Julian Date (double)
    // arg2 is longitude   (double)
    double julianDate = 2451552.54097;
    double longitude = 17.04535;
    if( argc > 1 )
    {
        julianDate = atof(*(argv+1));
        if( argc > 2)
        {
            longitude  = atof(*(argv+2));
        }
    }

    // example data can be used
//    2000-01-10 T 00:01 UTC is in the night 2000-01-09 - 2451553.50069
//    2000-01-09 T 23:59 UTC is in the night 2000-01-09 - 2451553.49931
//    2000-01-09 T 12:01 UTC is in the night 2000-01-09 - 2451553.00069
//    2000-01-09 T 11:59 UTC is in the night 2000-01-09 - 2451552.99931
//    2000-01-09 T 08:59 UTC is in the night 2000-01-08 - 2451552.87431
//    2000-01-09 T 00:59 UTC is in the night 2000-01-08 - 2451552.54097
    char *a = findDate(julianDate, longitude);

    //print answer
    printf("%s", a);

    //freee memory
    free(a);

    return 0;
}
