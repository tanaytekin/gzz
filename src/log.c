#include "log.h"

#include <time.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>



const char *log_types[] =
        {
        "TRACE",
        "INFO",
        "WARNING",
        "ERROR",
        "FATAL"
        };


void gzz_log(gzz_log_type type, const char *file_name, int line, const char *format, ...)
{
    time_t t;
    struct tm *tm;

    time(&t);
    tm = localtime(&t);

    char time_str[16];
    strftime(time_str, 16, "%H:%M:%S", tm);

    fprintf(stderr, "[%s][%s][%s at line:%d] : ", time_str, log_types[type], file_name, line);

    va_list args;
    va_start(args, format);
    vfprintf(stderr, format, args);
    va_end(args);

    fprintf(stderr, "\n");

    if(type == GZZ_LOG_FATAL)
        exit(1);
}
