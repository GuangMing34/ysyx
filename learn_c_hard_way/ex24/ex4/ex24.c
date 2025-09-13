#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../../ex20/dbg.h"

#define MAX_DATA 100

static FILE *fp;

int file_scanf(const char *filename, char *out_string, int unsigned len)
{
    int ch, idx, ret = 0;

    if (fp == NULL) {
        fp = fopen(filename, "r");
        check(fp, "file open failed!!!");
    }

    for (idx = 0; idx < len; idx++) {
        ch = fgetc(fp);
        if (ch == EOF) {
            printf("EOF, idx:%d\n", idx);
            fclose(fp);
            fp = NULL;
            ret = -1;
            break;
        } else {
            *(out_string + idx) = (char)ch;
        }
    }
    *(out_string + idx) = '\0';

    // if (ch == EOF) {
    //     printf("out, len:%d-%ld, content::%s\n", len, strlen(out_string), out_string);
    // }

    return ret;
error:
    return -1;
}

int file_init(const char *filename, int unsigned len)
{
    char ch;
    int idx, ret;

    if (fp == NULL) {
        fp = fopen(filename, "w");
        check(fp, "file open failed!!!");
    }

    for (idx = 0; idx < len; idx++) {
        ch = (idx%10) + '0';
        ret = fputc(ch, fp);
        if (ret == EOF) {
            printf("error, file write failed!!!");
            fclose(fp);
            fp = NULL;
            break;
        }
    }

    fclose(fp);
    fp = NULL;

    return 0;

error:
    return -1;
}

int main(int argc, char *argv[])
{
    int ret = 0;
    int total_len = 256*40;
    int read_total = 0, tmp_cnt = 0;
    static char *file_name = "test.txt";

    srand(time(NULL));
    ret = file_init(file_name, total_len);
    check(ret == 0, "file write failed!!");
    printf("file len:%d\n", total_len);

    for(int i = 0; i < 100; i++) {
        char *out_str;
        tmp_cnt = rand() % 1024;

        out_str = malloc(tmp_cnt);
        check(out_str != NULL, "malloc failed!!!");

        ret = file_scanf(file_name, out_str, tmp_cnt);
        if (ret < 0) {
            //file read done!
            printf("read done!!, total read cnt:%ld\n", read_total + strlen(out_str));
            break;
        } else {
            //do check content
            for (int str_idx = 0; str_idx < tmp_cnt; str_idx++) {
                char current_ch = out_str[str_idx];
                char exp_ch = ((read_total + str_idx)%10) + '0';
                check(exp_ch== current_ch, "str_match fialed, %c-%c", exp_ch, current_ch);
            }
            read_total += tmp_cnt;
            printf("read_total:%d, tmp_cnt:%d\n", read_total, tmp_cnt);
        }
        free(out_str);
    }

    return 0;
error:
    return -1;
}