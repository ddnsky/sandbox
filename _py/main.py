import sys


def main():
    langs = {}

    N = int(input())
    for _ in range(N):
        M = int(input())
        for _ in range(M):
            lang = input()
            if lang in langs:
                langs[lang] += 1
            else:
                langs[lang] = 1

    print(len([k for k,v in langs.items() if v==N ]))
    for l,n in langs.items():
        if n==N:
            print(l)
    print(len(langs))
    for l in langs.keys():
        print(l)

if __name__ == '__main__':
    main()