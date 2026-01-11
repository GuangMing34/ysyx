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
[list_algos_tests.c](./ex3/tests/list_algos_tests.c)
关键代码如下，主要是创建一个内容，但是仅仅创建一次即可。
如果每次都创建，会有数据共享的问题，在merge的流程中，这个创建的东西会共享到多个地方，如果前面free了，后边用起来就会有问题
如果不free，在循环次数多的情况下，会爆heap
所以，对于当前的结构来说，最好的方式就是模拟一个静态的str list. 运行时候只做一次创建。

```c
if (init_done == 0) {
    init_done = 1;

    str_cnt = rand() % (MAX_ENTRY_LEN - MIN_ENTRY_LEN + 1) + MIN_ENTRY_LEN;
    str_arr = malloc(sizeof(char *) * str_cnt);
    for (int i = 0; i < str_cnt; i++)
    {
        int cap_en = 0;

        rand_len = rand() % MAX_STR_LEN;
        rand_len += 5; // ensure min length of 5
        str_arr[i] = malloc(sizeof(char) * (rand_len + 1));
        for (int j = 0; j < rand_len; j++)
        {
            cap_en = rand() % 2;
            if (cap_en)
                str_arr[i][j] = 'A' + (rand() % 26);
            else
                str_arr[i][j] = 'a' + (rand() % 26);
        }
        str_arr[i][rand_len] = '\0';
        // printf("Generated entry[%d/%d]: %s\n", i, str_cnt, str_arr[i]);
    }
}
```

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
> 实现 List_insert_sorted （有序链表），它使用List_compare，接收一个值，将其插入到正确的位置，使链表有序。它与创建链表后再进行排序相比怎么样？
## answer
code: [list_algos.c](./ex5/list_algos.c)

相比较于创建后再排序，插入排序会把降低时间开销，因为它只需要遍历一次链表，而排序需要遍历多次链表。
但是坏处就是每次插入都需要做一次比较，而排序只需要做一次比较。

内存方向，依据实现的不同，插入后排序可能因为排序的中间状态的暂存导致消耗更多的内存，而插入排序则不会，只会改动前后节点的指针。

```c
List *List_insert_sorted(List *list, void *value, List_compare cmp)
{
    ListNode *node = calloc(1, sizeof(ListNode));

    node->value = value;

    if(!value) {
        return NULL;
    }

    if(List_count(list) == 0) {
        list->first = node;
        list->last = node;
        list->count++;
        return list;
    }

    LIST_FOREACH(list, first, next, cur) {
        if (cmp(cur->value, node->value) > 0) {
            //insert before cur
            ListNode *pre_node = cur->prev;
            ListNode *cur_node = cur;

            node->next = cur;
            node->prev = pre_node;
            cur->prev = node;
            if(pre_node) {
                pre_node->next = node;
            } else {
                //inserting at head
                list->first = node;
            }
            break;
        } else if (cur->next == NULL) {
            //insert after cur (at end)
            cur->next = node;
            node->prev = cur;
            node->next = NULL;
            list->last = node;
            break;
        }
    }

    return list;
}
```

# 附加题6
## question
> 尝试实现维基百科上“自底向上”的归并排序。上面的代码已经是C写的了，所以很容易重新创建，但是要试着理解它的工作原理，并与这里的低效版本对比。
## answer
wikipedia上的是数组排序, 并且是数值排序，这里是链表排序，带自定义比对函数指针的排序，还有点不太一样
至于效率层面，数组本身和链表的操作不同，感觉可比性不是很大
比如数组可以直接寻址，链表需要逐个寻址，尤其是在这个算法原本的逻辑上，是按照长度逐次增加，每种长度下，都需要对不同的部分做链表寻址，寻址压力比较大
[list_algos.c](./ex6/list_algos.c) 
```C
void BottomUpMergeSort(int A[], int B[], int n)
```
