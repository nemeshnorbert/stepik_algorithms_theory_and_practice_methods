def mat_mul(a, b):
    return [
        [a[0][0] * b[0][0] + a[0][1] * b[1][0], a[0][0] * b[1][0] + a[0][1] * b[1][1]],
        [a[1][0] * b[0][0] + a[1][1] * b[1][0], a[1][0] * b[1][0] + a[1][1] * b[1][1]]
    ]

def mat_mod(a, m):
    return [
        [a[0][0] % m, a[0][1] % m],
        [a[1][0] % m, a[1][1] % m]
    ]

def fast_pow(a, n, m):
    result = [[1,0],[0,1]]
    power = a
    while n > 0:
        if n & 1:
            result = mat_mod(mat_mul(result, power), m)
        power = mat_mod(mat_mul(power, power), m)
        n >>= 1
    return result

def fib_mod(n, m):
    mat = fast_pow([[1, 1], [1, 0]], n, m)
    return mat[1][0]


def main():
    n, m = map(int, input().split())
    print(fib_mod(n, m))


if __name__ == "__main__":
    main()