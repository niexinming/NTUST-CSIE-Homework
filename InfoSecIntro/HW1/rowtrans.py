#!/usr/bin/env python3
import sys
VER = tuple(sys.version_info)[:2]
if VER < (3, 6): print('- Python 3.6+ required'); exit()

import string

upper = string.ascii_uppercase
lower = string.ascii_lowercase
alphabet = upper + lower

KEY = '52834671'

class RowTranspositionCipher(object):
    def __init__(self, key):
        k = ( (int(k) - 1, i) for i, k in enumerate(key) )
        self.key = [ i[1] for i in sorted(k) ]

    def encrypt(self, data):
        bs = len(self.key)
        assert len(data) % bs == 0
        rows = len(data) // bs

        matrix = [
            [ data[x + y * bs] for x in range(bs) ]
            for y in range(rows)
        ]

        out = ''
        for col in self.key:
            for j in range(rows):
                out += matrix[j][col]

        return out

    def decrypt(self, data):
        bs = len(self.key)
        assert len(data) % bs == 0
        rows = len(data) // bs

        matrix = [
            [ None ] * bs
            for y in range(rows)
        ]

        t = iter(data)
        for col in self.key:
            for i in range(rows):
                matrix[i][col] = next(t)

        return ''.join(''.join(row) for row in matrix)

if __name__ == '__main__':
    cipher = RowTranspositionCipher(KEY)
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
