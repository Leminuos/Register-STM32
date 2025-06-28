#include <stdarg.h>
#include "debug.h"

static char digits[] = "0123456789ABCDEF";

static inline void print_char(char c)
{
    WRITE_OUT(c);
}

static void print_int(int64_t var, int width, int base, int sign)
{
    int i = 0;
    char buf[20] = {0};
    
    if (var == 0)
    {
        print_char('0');
        return;
    }

    if (sign)
    {
        if (var < 0) var = -var;
        else sign = 0;
    }

    for (; var != 0; var = var / base)
    {
        buf[i++] = digits[var % base];
    }

    for (; i < width; ++i)
    {
        buf[i] = '0';
    }

    if (sign) buf[i] = '-';

    while(--i >= 0)
    {
        print_char(buf[i]);
    }
}

static void print_str(char* str)
{
    char* null_str = "(null)";

    if (str == NULL)
    {
        for (; *null_str != 0; ++null_str)
        {
            print_char(*null_str);
        }

        return;
    }

    for ( ; *str != 0; ++str)
    {
        print_char(*str);
    }
}

int debug_print(char* format, ...)
{
#ifdef DEBUG_MODE
    register int width = 0;
    va_list args;
    va_start(args, format);

    for ( ; *format != 0; ++format)
    {
        if (*format != '%')
        {
            print_char(*format);
            continue;
        }

        ++format;

        if (*format == '\0') break;

        if (*format == '%')
        {
            print_char(*format);
            continue;
        }

        for ( ; *format >= '0' && *format <= '9'; ++format)
        {
            width = width * 10;
            width = width + *format - '0';
        }

        if (*format == 'd')
        {
            print_int(va_arg(args, int32_t), width, 10, 1);
        }
        else if (*format == 'x')
        {
            print_int(va_arg(args, int32_t), width, 16, 0);
        }
        else if (*format == 'X')
        {
            print_int(va_arg(args, int32_t), width, 16, 0);
        }
        else if (*format == 'c')
        {
            print_str((char[]) {
                (char) va_arg(args, int),
                '\0'
            });
        }
        else if (*format == 's')
        {
            print_str(va_arg(args, char*));
        }
    }
    
    return 0;
#endif /* DEBUG_MODE */
}

