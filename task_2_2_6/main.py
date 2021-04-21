def fib(n):
    a = 0
    b = 1
    for i in range(n):
        a, b = b, a + b
    return a

def main():
    n = int(input())
    print(fib(n))


if __name__ == "__main__":
    main()