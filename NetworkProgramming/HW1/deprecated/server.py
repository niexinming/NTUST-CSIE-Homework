#!/usr/bin/env python3.5

import os
import socket
import sys
from threading import Thread
from protosock import ProtoSock

SIG_KILL = 9

try:
    port = int(sys.argv[1], 10)
except ValueError:
    print('Invalid number')
    exit()
except IndexError:
    print('Usage: {prog} port-number'.format(prog=sys.argv[0]))
    print()
    print('  optinal: BIND_HOST in environment variable')
    exit()

host = os.getenv('BIND_HOST', '0.0.0.0')


class Server(object):
    def __init__(self, remote, addr):
        self.remote = remote

        if remote.recvn(8) != b'IMAGEREQ':
            remote.sendall(b'BAD PROTOCOL')

    def __del__(self):
        try:
            self.remote.close()
        except:
            pass

def main():
    server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    server.bind((host, port))
    server.listen(16)

    print('bind on %s:%d' % (host, port))

    while True:
        remote, addr = server.accept()
        print('Connection from %s:%d' % addr)
        Thread(target=Server, args=(ProtoSock(remote), addr)).start()

if __name__ == '__main__':
    try:
        main()
    except KeyboardInterrupt:
        print('Bye~')
        os.kill(os.getpid(), SIG_KILL)  # kill self to exit all threads
