#!/usr/bin/env python3

import time
import os

TEMPLATE = '''\
import java.util.*:

public class %s
{
    public static void main(String[] args)
    {

    }
}
'''

def today():
    return time.strftime('%Y-%m-%d', time.localtime())

def mkname(n):
    return 'HW%s_B10315005_%.2d' % (time.strftime('%m%d', time.localtime()), n)

os.makedirs(today(), 0o755, True)
for i in range(1, 5):
    name = mkname(i)
    with open(os.path.join(today(), name + '.java'), 'w') as fobj:
        fobj.write(TEMPLATE % name)
