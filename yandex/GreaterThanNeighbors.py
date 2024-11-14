import sys

def main():
    """
    Дан список чисел. Определите, сколько в этом списке элементов, которые больше двух своих соседей.
    Выведите количество таких элементов. Формат ввода
    Вводится список чисел. Все числа списка находятся на одной строке.
    Пример 1
        Ввод 1 2 3 4 5
        Вывод 0
    Пример 2
        Ввод 5 4 3 2 1
        Вывод 0
    Пример 3
        Ввод 1 5 1 5 1
        Вывод 2
    """
    nums = iter(map(int, input().split()))
    try:
        prevNum = next(nums)
    except StopIteration:
        print(0)
        return

    result = 0
    prevIsGreater = False
    for nextNum in nums:
        if nextNum<prevNum:
            result += prevIsGreater
            prevIsGreater = False
        elif nextNum>prevNum:
            prevIsGreater = True
        prevNum = nextNum
    print(result)

if __name__ == '__main__':
    main()