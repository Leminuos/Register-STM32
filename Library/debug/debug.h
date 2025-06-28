#ifndef __DEBUG_H__
#define __DEBUG_H__

#include "token.h"
#include "log_color.h"

typedef enum {
    LOG_NONE    = 0,    /*!< No log output */
    LOG_ERROR   = 1,    /*!< Critical errors, software module can not recover on its own */
    LOG_WARN    = 2,    /*!< Error conditions from which recovery measures have been taken */
    LOG_INFO    = 3,    /*!< Information messages which describe normal flow of events */
    LOG_DEBUG   = 4,    /*!< Extra information which is not necessary for normal use (values, pointers, sizes, etc). */
    LOG_VERBOSE = 5,    /*!< Bigger chunks of debugging information, or frequent messages which can potentially flood the output. */
    LOG_MAX     = 6,    /*!< Number of levels supported */
} log_level_t;

#define NULL_TAG    NULL

#ifdef TIMESTAMP
#if CONFIG_LOG_COLOR == 1
#define LOG_FORMAT_TAG(letter, format)          LOG_COLOR_ ## letter #letter " %d [%s]: " format LOG_RESET_COLOR "\r\n"
#define LOG_FORMAT_DEFAULT(letter, format)      LOG_COLOR_ ## letter #letter " %d: " format LOG_RESET_COLOR "\r\n"
#else
#define LOG_FORMAT_TAG(letter, format)          #letter " %d [%s]: " format "\r\n"
#define LOG_FORMAT_DEFAULT(letter, format)      #letter " %d: " format "\r\n"
#endif /* CONFIG_LOG_COLOR */

#define LOG_TAG(level, tag, format, ...) do {                   \
        if (level==LOG_ERROR )          { debug_print(LOG_FORMAT_TAG(E, format), TIMESTAMP, tag, ##__VA_ARGS__); } \
        else if (level==LOG_WARN )      { debug_print(LOG_FORMAT_TAG(W, format), TIMESTAMP, tag, ##__VA_ARGS__); } \
        else if (level==LOG_DEBUG )     { debug_print(LOG_FORMAT_TAG(D, format), TIMESTAMP, tag, ##__VA_ARGS__); } \
        else if (level==LOG_VERBOSE )   { debug_print(LOG_FORMAT_TAG(V, format), TIMESTAMP, tag, ##__VA_ARGS__); } \
        else                            { debug_print(LOG_FORMAT_TAG(I, format), TIMESTAMP, tag, ##__VA_ARGS__); } \
    } while(0)

#define LOG_DEFAULT(level, format, ...) do {                    \
        if (level==LOG_ERROR )          { debug_print(LOG_FORMAT_DEFAULT(E, format), TIMESTAMP, ##__VA_ARGS__); } \
        else if (level==LOG_WARN )      { debug_print(LOG_FORMAT_DEFAULT(W, format), TIMESTAMP, ##__VA_ARGS__); } \
        else if (level==LOG_DEBUG )     { debug_print(LOG_FORMAT_DEFAULT(D, format), TIMESTAMP, ##__VA_ARGS__); } \
        else if (level==LOG_VERBOSE )   { debug_print(LOG_FORMAT_DEFAULT(V, format), TIMESTAMP, ##__VA_ARGS__); } \
        else                            { debug_print(LOG_FORMAT_DEFAULT(I, format), TIMESTAMP, ##__VA_ARGS__); } \
    } while(0)
#else
#if CONFIG_LOG_COLOR == 1
#define LOG_FORMAT_TAG(letter, format)          LOG_COLOR_ ## letter #letter " [%s]: " format LOG_RESET_COLOR "\r\n"
#define LOG_FORMAT_DEFAULT(letter, format)      LOG_COLOR_ ## letter #letter " " format LOG_RESET_COLOR "\r\n"
#else
#define LOG_FORMAT_TAG(letter, format)          #letter " [%s]: " format "\r\n"
#define LOG_FORMAT_DEFAULT(letter, format)      #letter " " format "\r\n"
#endif /* CONFIG_LOG_COLOR */

#define LOG_TAG(level, tag, format, ...) do {                   \
        if (level==LOG_ERROR )          { debug_print(LOG_FORMAT_TAG(E, format), tag, ##__VA_ARGS__); } \
        else if (level==LOG_WARN )      { debug_print(LOG_FORMAT_TAG(W, format), tag, ##__VA_ARGS__); } \
        else if (level==LOG_DEBUG )     { debug_print(LOG_FORMAT_TAG(D, format), tag, ##__VA_ARGS__); } \
        else if (level==LOG_VERBOSE )   { debug_print(LOG_FORMAT_TAG(V, format), tag, ##__VA_ARGS__); } \
        else                            { debug_print(LOG_FORMAT_TAG(I, format), tag, ##__VA_ARGS__); } \
    } while(0)

#define LOG_DEFAULT(level, format, ...) do {                    \
        if (level==LOG_ERROR )          { debug_print(LOG_FORMAT_DEFAULT(E, format), ##__VA_ARGS__); } \
        else if (level==LOG_WARN )      { debug_print(LOG_FORMAT_DEFAULT(W, format), ##__VA_ARGS__); } \
        else if (level==LOG_DEBUG )     { debug_print(LOG_FORMAT_DEFAULT(D, format), ##__VA_ARGS__); } \
        else if (level==LOG_VERBOSE )   { debug_print(LOG_FORMAT_DEFAULT(V, format), ##__VA_ARGS__); } \
        else                            { debug_print(LOG_FORMAT_DEFAULT(I, format), ##__VA_ARGS__); } \
    } while(0)
#endif /* TIMESTAMP */

#define DEBUG(level, tag, format, ...) do {                     \
        if (tag == NULL_TAG)            {LOG_DEFAULT(level, format, ##__VA_ARGS__);}    \
        else                            {LOG_TAG(level, tag, format, ##__VA_ARGS__);}   \
    } while (0)

extern int debug_print(char* format, ...);

#endif /* __DEBUG_H__ */

