def _gcd_impl(a, b):
    while a > 0 and b > 0:
        a, b = (a % b, b) if a >= b else (b % a, a)
    return a + b

def gcd(a, b):
    return _gcd_impl(abs(a), abs(b))

def main():
    a, b = map(int, input().split())
    print(gcd(a, b))


if __name__ == "__main__":
    main()