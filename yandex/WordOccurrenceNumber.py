import sys


def main():
    """
    Во входном файле (вы можете читать данные из файла input.txt) записан текст. Словом
    считается последовательность непробельных символов идущих подряд, слова разделены
    одним или большим числом пробелов или символами конца строки. Для каждого слова из
    этого текста подсчитайте, сколько раз оно встречалось в этом тексте ранее.

    Пример 1 Ввод one two one tho three    Вывод  0 0 1 0 0
    Пример 2
        Ввод
            She sells sea shells on the sea shore;
            The shells that she sells are sea shells I'm sure.
            So if she sells sea shells on the sea shore,
            I'm sure that the shells are sea shore shells.
        Вывод
            0 0 0 0 0 0 1 0 0 1 0 0 1 0 2 2 0 0 0 0 1 2 3 3 1 1 4 0 1 0 1 2 4 1 5 0 0
    """


    words = {}
    with open('input.txt') as file:
        for line in file:
            for word in line.split():
                if word in words:
                    words[word] += 1
                    print(words[word], end=" ")
                else:
                    words[word] = 0
                    print("0", end=" ")

if __name__ == '__main__':
    main()
