# question 1:
> die函数需要接收conn变量作为参数，以便执行清理并关闭它。
## answer
[new_die](./ex17_ex1.c)

# question 2:
>修改代码，使其接收参数作为MAX_DATA和MAX_ROWS，将它们储存在Database结构体中，并且将它们写到文件。这样就可以创建任意大小的数据库。

[dynamitic_db](./ex17_ex2.c)

# question 3:
> 向数据库添加更多操作，比如find。

[find_db](./ex17_ex3.c)

# question 4:
> 查询C如何打包结构体，并且试着弄清楚为什么你的文件是相应的大小。看看你是否可以计算出结构体添加一些字段之后的新大小

## answer
use keyword <struct> to pack.
### example
```C
struct pack_data{
    int val1;
    char val2;
    int *val3;
};
```

# question 5
> 向Address添加一些字段，使它们可被搜索
## answer
[search field](./ex5/ex17_ex5.c)