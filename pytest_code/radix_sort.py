def counting_sort(arr, exp):
    n = len(arr)
    output = [0] * n  # 初始化输出数组
    count = [0] * 10  # 初始化计数数组

    # 统计每个数字的出现次数
    for num in arr:
        index = (num // exp) % 10
        count[index] += 1

    #print(f"Count array for exp={exp}: {count}")
    # 累加计数数组，得到每个数字的最终位置
    for i in range(1, 10):
        count[i] += count[i - 1]

    #print(f"Cumulative count array for exp={exp}: {count}")
    # 将元素放入输出数组
    i = n - 1
    while i >= 0:
        index = (arr[i] // exp) % 10
        output[count[index] - 1] = arr[i]
        count[index] -= 1
        i -= 1

    # 将输出数组复制到原始数组
    for i in range(n):
        arr[i] = output[i]

def radix_sort(arr):
    # 找到最大数字的位数
    max_num = max(arr)
    exp = 1  # 从个位数开始
    while max_num // exp > 0:
        counting_sort(arr, exp)  # 对当前位数进行计数排序
        print(f"Array after sorting with exp={exp}: {arr}")
        exp *= 10  # 移动到下一位
    return arr

# 示例
arr = [170, 45, 75, 90, 802, 24, 2, 66]
sorted_arr = radix_sort(arr)
print(sorted_arr)  # 输出: [2, 24, 45, 66, 75, 90, 170, 802]