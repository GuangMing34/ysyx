/** WARNING: This code is fresh and potentially isn't correct yet. */

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include "../../ex20/dbg.h"

#define MAX_DATA 100

void my_printf(const char *format, ...) {
    va_list args;
    va_start(args, format);

    for (int i = 0; format[i] != '\0'; i++) {
        if (format[i] == '%') {
            i++;
            switch (format[i]) {
                case 'd': {
                    int val = va_arg(args, int);
                    //printf("%d", val);
                    {
                        char str[100] = {0};
                        int str_len = 0;
                        int val_tmp = val;
                        while (val_tmp) {
                            int rem_low = val_tmp % 10;
                            str[str_len] = '0' + rem_low;
                            val_tmp = val_tmp / 10;
                            str_len += 1;
                        }
                        for (int i = str_len-1; i >= 0; i--) {
                            putchar(str[i]);
                        }
                    }
                    break;
                }
                case 'c': {
                    int val = va_arg(args, int);
                    char val_c = (char)val;
                    putchar(val_c);
                    break;
                }
                case 's': {
                    char *str = va_arg(args, char*);
                    int i = 0;
                    if (str == NULL) str = "(null)";

                    while (str[i])
                    {
                        putchar(str[i]);
                        i++;
                    }
                    break;
                }
                default:
                    putchar('%');
                    putchar(format[i]);
                    break;
            }
        } else {
            putchar(format[i]);
        }
    }

    va_end(args);
}


int read_string(char **out_string, int max_buffer)
{
    *out_string = calloc(1, max_buffer + 1);
    check_mem(*out_string);

    char *result = fgets(*out_string, max_buffer, stdin);
    check(result != NULL, "Input error.");

    return 0;

error:
    if(*out_string) free(*out_string);
    *out_string = NULL;
    return -1;
}

int read_int(int *out_int)
{
    char *input = NULL;
    int rc = read_string(&input, MAX_DATA);
    check(rc == 0, "Failed to read number.");

    *out_int = atoi(input);

    free(input);
    return 0;

error:
    if(input) free(input);
    return -1;
}

int read_scan(const char *fmt, ...)
{
    int i = 0;
    int rc = 0;
    int *out_int = NULL;
    char *out_char = NULL;
    char **out_string = NULL;
    int max_buffer = 0;

    va_list argp;
    va_start(argp, fmt);

    for(i = 0; fmt[i] != '\0'; i++) {
        if(fmt[i] == '%') {
            i++;
            switch(fmt[i]) {
                case '\0':
                    sentinel("Invalid format, you ended with %%.");
                    break;

                case 'd':
                    out_int = va_arg(argp, int *);
                    rc = read_int(out_int);
                    check(rc == 0, "Failed to read int.");
                    break;

                case 'c':
                    out_char = va_arg(argp, char *);
                    *out_char = fgetc(stdin);
                    break;

                case 's':
                    max_buffer = va_arg(argp, int);
                    out_string = va_arg(argp, char **);
                    rc = read_string(out_string, max_buffer);
                    check(rc == 0, "Failed to read string.");
                    break;

                default:
                    sentinel("Invalid format.");
            }
        } else {
            fgetc(stdin);
        }

        check(!feof(stdin) && !ferror(stdin), "Input error.");
    }

    va_end(argp);
    return 0;

error:
    va_end(argp);
    return -1;
}


int main(int argc, char *argv[])
{
    char *first_name = NULL;
    char initial = ' ';
    char *last_name = NULL;
    int age = 0;

    my_printf("What's your first name? ");
    int rc = read_scan("%s", MAX_DATA, &first_name);
    check(rc == 0, "Failed first name.");

    my_printf("What's your initial? ");
    rc = read_scan("%c\n", &initial);
    check(rc == 0, "Failed initial.");

    my_printf("What's your last name? ");
    rc = read_scan("%s", MAX_DATA, &last_name);
    check(rc == 0, "Failed last name.");

    my_printf("How old are you? ");
    rc = read_scan("%d", &age);

    my_printf("---- RESULTS ----\n");
    my_printf("First Name: %s", first_name);
    my_printf("Initial: '%c'\n", initial);
    my_printf("Last Name: %s", last_name);
    my_printf("Age: %d\n", age);

    free(first_name);
    free(last_name);
    return 0;
error:
    return -1;
}