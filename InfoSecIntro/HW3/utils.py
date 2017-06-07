__all__ = [
    'pyversion',
    'egcd', 'modinv', 'fpow',
    'Str', 'Bytes', 'IntTypes',
    'bytes2int', 'int2bytes',
    'ensure_bytes', 'ensure_str',
    'profile',
    'enhex', 'unhex'
]

import binascii
import sys
import time

try: # python 2/3 compatability
    pyversion = 2
    Str, Bytes, IntTypes = unicode, str, (int, long) # unicode only in python2
    def bytes2int(b):
        return int(ensure_bytes(b)[::-1].encode('hex'), 16)
    def int2bytes(b, sz):
        return ('%x' % b).zfill(sz * 2).decode('hex')[::-1]
    if sys.version_info.minor < 7:
        print('python3.5+ or python2.7+ required')
        exit()
except:
    pyversion = 3
    Str, Bytes, IntTypes = str, bytes, (int,)
    def bytes2int(b):
        return int.from_bytes(ensure_bytes(b), 'little')
    def int2bytes(b, sz):
        return b.to_bytes(sz, 'little')
    if sys.version_info.minor < 5:
        print('python3.5+ or python2.7+ required')
        exit()

assert sys.version_info.major == pyversion

def ensure_bytes(s):
    if type(s) is Str:
        return s.encode('utf-8')
    elif type(s) in (Bytes, bytearray):
        return Bytes(s)
    else:
        raise TypeError

def ensure_str(s):
    if type(s) is Str:
        return s
    elif type(s) in (Bytes, bytearray):
        return Bytes(s).decode('utf-8')
    else:
        raise TypeError

def egcd(a, b):
    l, r = abs(a), abs(b)
    x, lx, y, ly = 0, 1, 1, 0
    while r:
        l, (q, r) = r, divmod(l, r)
        x, lx = lx - q*x, x
        y, ly = ly - q*y, y

    return l, -lx if a < 0 else lx, -ly if b < 0 else ly

def modinv(a, m):
    g, x, y = egcd(a, m) # solving g = a * x + m * y
    assert g == a * x + m * y
    if g != 1:
        raise ValueError
    return x % m

def fpow(a, m, n):
    """
    fast pow algorithm

    a ** m % n
    """

    v = 1
    while m > 0:
        if m & 1 == 1:
            v = (v * a) % n
        m = m >> 1
        a = (a * a) % n
    return v


def profile(f, args=None):
    t = time.time()
    if not args:
        args = []
    r = f(*args)
    e = time.time()
    return e-t, r

def enhex(s):
    return binascii.hexlify(s)

def unhex(s):
    return binascii.unhexlify(s)

if __name__ == '__main__':
    print('[*] testing fpow')
    for i in range(2, 7):
        for j in range(3, 8):
            for k in range(4, 9):
                assert pow(i, j, k) == fpow(i, j, k)
    print('[+] fpow test passed')

    from timeit import timeit

    N = 30000
    t1 = timeit(lambda: fpow(9487, 5566, 7788), number=N)
    t2 = timeit(lambda: pow (9487, 5566, 7788), number=N)

    print('[*] running pow and fpow %d times...' % N)
    print('')
    print('      fpow took %g secs' % t1)
    print('       pow took %g secs' % t2)
    print('')

    print('[*] testing modinv')
    n = 32341
    for i in range(1, 12345):
        assert (i * modinv(i, n)) % n == 1
    print('[+] modinv test passed')
