import sys

def main():
    n = int(input())
    prefixHf = [0,0]
    prefixHb = [0,0]
    _,y = map(int , input().split())
    for i in range(n-1):
        _, yy = map(int, input().split())
        if yy > y:
            prefixHf.append(prefixHf[-1] + yy - y)
            prefixHb.append(prefixHb[-1])
        else:
            prefixHf.append(prefixHf[-1])
            prefixHb.append(prefixHb[-1] + y - yy)
        y = yy
    m = int(input())
    for i in range(m):
        x, y = map(int, input().split())
        if x < y:
            print(prefixHf[y] - prefixHf[x])
        else:
            print(prefixHb[x] - prefixHb[y])

if __name__ == '__main__':
    main()
