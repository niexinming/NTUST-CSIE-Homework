#!/usr/bin/env python3
import sys
VER = tuple(sys.version_info)[:2]
if VER < (3, 6): print('- Python 3.6+ required'); exit()

import string

upper = string.ascii_uppercase
lower = string.ascii_lowercase
alphabet = upper + lower

KEY = [ 15, 11, 19, 18, 16, 3, 7, 14, 2, 20, 4, 12, 9, 6, 1, 5, 17, 13, 10, 8 ]


class ProductCipher(object):
    def __init__(self, key):
        self.key = key

    def encrypt(self, data):
        data = data.ljust(len(KEY))
        assert len(data) == len(self.key)
        slots = [None] * len(data)

        for i, k in enumerate(self.key):
            slots[i] = data[k - 1]

        return ''.join(slots)

    def decrypt(self, data):
        assert len(data) == len(self.key)
        slots = [None] * len(data)

        for i, k in enumerate(self.key):
            slots[k - 1] = data[i]

        return ''.join(slots)

if __name__ == '__main__':
    cipher = ProductCipher(KEY)
    r = cipher.encrypt('sentfrommyiphone')
    o = cipher.decrypt(r)
    print('Encrypted:', r)
    print('Decrypted:', o)

    while True:
        op = input('(E)ncrypt or (D)ecrypt? ')
        if op.upper()[:1] == 'E':
            print(cipher.encrypt(input('Data: ')))
        elif op.upper()[:1] == 'D':
            print(cipher.decrypt(input('Data: ')))
        else:
            print('Bad option, bye')
            break
