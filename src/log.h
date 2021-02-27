#ifndef GZZ_LOG_H
#define GZZ_LOG_H

typedef enum gzz_log_type
{
    GZZ_LOG_TRACE,
    GZZ_LOG_INFO,
    GZZ_LOG_WARNING,
    GZZ_LOG_ERROR,
    GZZ_LOG_FATAL,
} gzz_log_type;


void gzz_log(gzz_log_type type, const char *file_name, int line, const char *format, ...);

#define GZZ_LOG_TRACE(...)    gzz_log(GZZ_LOG_TRACE, __FILE__, __LINE__, __VA_ARGS__)
#define GZZ_LOG_INFO(...)     gzz_log(GZZ_LOG_INFO, __FILE__, __LINE__, __VA_ARGS__)
#define GZZ_LOG_WARNING(...)  gzz_log(GZZ_LOG_WARNING, __FILE__, __LINE__, __VA_ARGS__)
#define GZZ_LOG_ERROR(...)    gzz_log(GZZ_LOG_ERROR, __FILE__, __LINE__, __VA_ARGS__)
#define GZZ_LOG_FATAL(...)    gzz_log(GZZ_LOG_FATAL, __FILE__, __LINE__, __VA_ARGS__)

#endif //GZZ_LOG_H