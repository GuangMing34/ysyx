# add assert

> - 查看你定义的每个函数，一次一个文件。
> - 在每个函数的最上面，添加assert来保证参数正确。例如在Object_new中要添加assert(description != NULL)。
> - 浏览函数的每一行，找到所调用的任何函数。阅读它们的文档（或手册页），确认它们在错误下返回什么。添加另一个断言来检查错误是否发生。例如，Object_new在调用calloc之后应该进行assert(el != NULL)的检查。
> - 如果函数应该返回一个值，也确保它返回了一个错误值（比如NULL），或者添加一个断言来确保返回值是有效的。例如，Object_new中，你需要在最后的返回之前添加assert(el != NULL)，由于它不应该为NULL。
> - 对于每个你编写的if语句，确保都有对应的else语句，除非它用于错误检查并退出。
> - 对于每个你编写的switch语句，确保都有一个default分支，来处理非预期的任何情况。

## answer
code:
[add assert](./ex19_assert/ex19.c)

# question 1
> 修改Makefile文件，使之在执行make clean时能够同时清理object.o。

## answer
code:
[makefile_clean object](./ex19_ex1/Makefile)

# question 3
> 在游戏中添加更多房间和怪物

## answer

[more_monster](./ex19_ex3/ex19.c)
