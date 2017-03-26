#!/usr/bin/env python3
import sys
VER = tuple(sys.version_info)[:2]
if VER < (3, 6): print('- Python 3.6+ required'); exit()

import textwrap
import string

upper = string.ascii_uppercase
lower = string.ascii_lowercase
alphabet = upper + lower

KEY = 'DPP'

class PlayfairCipher(object):
    def __init__(self, key):
        arr = ''
        for i in (key.lower() + lower).replace('j', 'i'):
            if i not in arr:
                arr += i
        key = [ i + i for i in textwrap.wrap(arr, 5) ] * 2
        self.keys = [ key, self._transpose_list_of_str(key) ]

    def _transpose_list_of_str(self, x):
        return list(map(''.join, zip(*x)))

    def _find_pos(self, x):
        for i in range(5):
            for j in range(5):
                if self.keys[0][i][j] == x:
                    return i, j

    def _rect_replace(self, data):
        ra, ca = self._find_pos(data[0])
        rb, cb = self._find_pos(data[1])
        return self.keys[0][ra][cb] + self.keys[0][rb][ca]

    def _encrypt_pair(self, data):
        for key in self.keys:
            for row in key:
                if data in row:
                    idx = row.index(data)
                    return row[idx + 1 : idx + 3]
        return self._rect_replace(data)

    def _decrypt_pair(self, data):
        for key in self.keys:
            for row in key:
                if data in row:
                    idx = row.index(data)
                    if idx == 0:
                        idx = 5
                    return row[idx - 1 : idx + 1]
        return self._rect_replace(data)

    def encrypt(self, data):
        assert len(data) % 2 == 0
        data = textwrap.wrap(data.lower().replace('j', 'i'), 2)
        out = map(self._encrypt_pair, data)
        return ''.join(out)

    def decrypt(self, data):
        assert len(data) % 2 == 0
        data = textwrap.wrap(data.lower().replace('j', 'i'), 2)
        out = map(self._decrypt_pair, data)
        return ''.join(out)

if __name__ == '__main__':
    cipher = PlayfairCipher(KEY)
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
