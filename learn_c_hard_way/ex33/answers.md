# 附加题1
## question
> 创建单元测试来比较这两个算法的性能。你需要man 3 time来查询基本的时间函数，并且需要运行足够的迭代次数，至少以几秒钟作为样本。

## answer
[list_algos_tests.c](./ex1/tests/list_algos_tests.c)
关键代码
```c
struct timespec start, end;
double diff;
int unsigned loop = 10000000;
clock_gettime(CLOCK_MONOTONIC, &start);
for (size_t i = 0; i < loop; i++)
{
    test_bubble_or_merge_sort();
}
clock_gettime(CLOCK_MONOTONIC, &end);
diff = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
printf("Bubble sort time for %d sorts: %f seconds\n", loop, diff);
```

result on my machine(macos M3)
```text
RUNNING: ./tests/list_algos_tests
Bubble sort time for 10000000 sorts: 1.641338 seconds
Merge sort time for 10000000 sorts: 11.994807 seconds
```

# 附加题2
## question
> 改变需要排序的链表中的数据总量，看看耗时如何变化。

## answer
[list_algos_tests.c](./ex2/tests/list_algos_tests.c)
关键代码
```c
//before
char *values[] = {"XXXX", "1234", "abcd", "xjvef", "NDSS"};
#define NUM_VALUES 5
//after
char *values[] = {"XXXX", "1234", "abcd", "xjvef", "NDSS", "apple", "applepie", "applecider", "appleseed", "applause"};
#define NUM_VALUES 10
```

result on my machine(macos M3)
```text
RUNNING: ./tests/list_algos_tests
Bubble sort time for 10000000 sorts: 3.857339 seconds
Merge sort time for 10000000 sorts: 32.879900 seconds
```

bubble 随着数据量加大，耗时线性增长，基本是2倍多一点（可能与新增字符总数据量超过原有数据一倍有关）
merge 随着数据量加大，增长更快，超过2倍接近3倍

# 附加题3
## question
> 寻找方法来创建不同长度的随机链表，并且测量需要多少时间，之后将它可视化并与算法的描述对比。

## answer

# 附加题4
## question
> 尝试解释为什么对链表排序十分麻烦。

## answer
链表本身是散乱的数据结构，由指针把他们串接起来，实际上并不是在一起的数据。
那么对其强行排序，除了要比较原有的数据大小以外
还要对移出的结构的前后结构的指针进行修改，对移入的位置的前后的结构的指针进行修改。
如果是数组的话，只需要做值交换即可。

# 附加题5
## question
> 实现List_insert_sorted（有序链表），它使用List_compare，接收一个值，将其插入到正确的位置，使链表有序。它与创建链表后再进行排序相比怎么样？
## answer

# 附加题6
## question
> 尝试实现维基百科上“自底向上”的归并排序。上面的代码已经是C写的了，所以很容易重新创建，但是要试着理解它的工作原理，并与这里的低效版本对比。
## answer

