#!/usr/bin/env python3
import sys
VER = tuple(sys.version_info)[:2]
if VER < (3, 6): print('- Python 3.6+ required'); exit()

import string

upper = string.ascii_uppercase
lower = string.ascii_lowercase
alphabet = upper + lower

KEY = 'KMT'

class AutokeyCipher(object):
    def __init__(self, key):
        self.key = key.lower()

    def encrypt(self, data):
        assert data.lower() == data # lower-case only
        key = self.key + data
        return ''.join(chr((ord(i) - ord('a') + ord(j) - ord('a')) % 26 + ord('a')) for i, j in zip(data, key))

    def decrypt(self, data):
        out = ''
        bs = len(self.key) # block size
        last_key = self.key
        for i in range(len(data)):
            if i and i % bs == 0:
                last_key = out[i-bs:i]
            v = ord(data[i]) - ord('a')
            k = ord(last_key[i % bs]) - ord('a')
            out += chr((v - k + 26) % 26 + ord('a'))
        return out

if __name__ == '__main__':
    cipher = AutokeyCipher(KEY)
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
