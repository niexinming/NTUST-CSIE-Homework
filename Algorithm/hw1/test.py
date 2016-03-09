from functools import partial
from itertools import accumulate
from random import randint
from subprocess import Popen, PIPE
from time import time

Rs = [    3,    8,   10,   15,   25, 100, 1000 ]
Ls = [    5,  100,   20,   40,  100, 100,  200 ]
Ns = [ 8000, 6000, 3000, 2000, 1500, 600,  300 ]

def fail(test_data, my_result, wa):
    print('Fail!!!!!!!!!!!!!')
    print('test data: %s' % test_data)
    print('my answer: %s' % (my_result, ))
    print('yours:     %s' % (wa, ))
    exit()

def dump(name, I):
    name = '%8s: ' % name
    print(name + ' '.join(map(lambda x: '%3d' % x, I)))

def solve(L):
    if type(L) is str:
        L = L.split()[1:]
        L = list(map(int, L))
        print(L)
    S = [0] + list(accumulate(L))
    #dump('index', range(len(L)))
    #dump('data', L)
    #dump('acc', S)
    l, r, m = 0, 0, 0
    for i in range(len(L)):
        for j in range(i, len(L)):
            v = S[j+1] - S[i]
            if v > m: l, r, m = i, j, v
    return l, r, m

"""
9 0 -2 2 -3 1 2 -3 -1 -2
(4, 2, 3)
(4, 5, 3)
"""
#print(solve('9 0 -2 -1 -2 3 3 -2 3 -1')); exit()

for R, L, N in zip(Rs, Ls, Ns):
    rnd = partial(randint, -R, R)
    p = Popen('./a.out', stdin=PIPE, stdout=PIPE, stderr=PIPE, bufsize=1)
    send = lambda x: [ p.stdin.write(x.encode() + b'\n'), p.stdin.flush() ]
    recv = lambda: p.stdout.readline().decode()[:-1]

    test_datas = []

    for i in range(N):
        test_data = [ rnd() for _ in range(L) ]
        test = ('%d ' % len(test_data)) + ' '.join(map(str, test_data))
        answer = solve(test_data)
        test_datas.append((test, test_data, answer))

    t_start = time()
    for test_str, test, answer in test_datas:
        send(test_str)
        result = tuple(map(int, recv().split()))
        if answer[2] == 0:
            if result[2] != 0:
                fail(test, answer, result)
        elif result != answer:
            if sum(test[result[0] : result[1] + 1]) != answer[2]:
                fail(test, answer, result)
    t_end = time()

    p.kill()
    print("Pass: R=%d, L=%d, N=%d in %f secs" % (R, L, N, t_end - t_start))
