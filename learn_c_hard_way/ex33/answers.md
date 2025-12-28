# 附加题1
## question
> 创建单元测试来比较这两个算法的性能。你需要man 3 time来查询基本的时间函数，并且需要运行足够的迭代次数，至少以几秒钟作为样本。

## answer
[ex1_dir](./ex1/)
关键代码
```c
start = time(NULL);
for (size_t i = 0; i < loop; i++)
{
    test_bubble_or_merge_sort();
}
end = time(NULL);
diff = difftime(end, start);
```

result on my machine(macos M3)
```text
RUNNING: ./tests/list_algos_tests
Bubble sort time for 10000000 sorts: 2.000000 seconds
Merge sort time for 10000000 sorts: 17.000000 seconds
```

# 附加题2
## question
> 改变需要排序的链表中的数据总量，看看耗时如何变化。

## answer

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

