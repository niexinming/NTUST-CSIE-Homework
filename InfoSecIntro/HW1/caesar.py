#!/usr/bin/env python3
import sys
VER = tuple(sys.version_info)[:2]
if VER < (3, 6): print('- Python 3.6+ required'); exit()

import string

upper = string.ascii_uppercase
lower = string.ascii_lowercase
alphabet = upper + lower

class CaesarCipher(object):
    def __init__(self, key):
        key = int(key) % 26
        enc_mapping = upper[key:] + upper[:key] + \
                      lower[key:] + lower[:key]
        self.enc = str.maketrans(alphabet, enc_mapping)
        self.dec = str.maketrans(enc_mapping, alphabet)

    def encrypt(self, data):
        return data.translate(self.enc)

    def decrypt(self, data):
        return data.translate(self.dec)

if __name__ == '__main__':
    cipher = CaesarCipher(3)
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
