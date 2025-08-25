#include <stdio.h>

int stackoverflow(int cnt)
{
    int result = 0;
    int new_cnt = cnt + 1;

    if (cnt == 0x1000000)
    {
        return 1;
    } else {
        result += stackoverflow(new_cnt);
    }

    return 0;
}
int main(void)
{
    int a = stackoverflow(0);

    printf("a:%d", a);

    return 0;
}