from socket import *

class ProtoSock(object):
    @staticmethod
    def proxify(cls, method):
        print(method.__name__)
        return method

    def __init__(self, _sock):
        self._sock = _sock

    @ProtoSock.proxify
    def recv():
        pass

    @ProtoSock.proxify
    def send():
        pass

    @ProtoSock.proxify
    def sendall():
        pass

    @ProtoSock.proxify
    def shutdown():
        pass

    @ProtoSock.proxify
    def close():
        pass

    def recvn(self, n):
        """
        recieve n bytes from remote
        """

        data_buff = self.recv(n)
        while len(data_buff) < n:
            t = self.recv(n - len(data_buff))
            if not t: return None
            data_buff += t
        return data_buff
