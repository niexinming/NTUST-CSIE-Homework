def f1(a, b, c, d):  # BC' + AB + AD
    m1 = b and (not c)
    m2 = a and b
    m3 = a and d
    return m1 or m2 or m3

def f1_bar(a, b, c, d):  # B'D' + A'B' + AC
    m1 = not b and not d
    m2 = not a and not b
    m3 = not a and c
    return m1 or m2 or m3

def f1_pos(a, b, c, d):  # (B + D)(A + B)(A + C')
    m1 = b or d
    m2 = a or b
    m3 = a or not c
    return m1 and m2 and m3

def f1_bar_pos(a, b, c, d):  # (B' + C)(A' + B')(A' + D')
    m1 = not b or c
    m2 = not a or not b
    m3 = not a or not d
    return m1 and m2 and m3

def split(i):
    d = ( i >> 0 ) & 1
    c = ( i >> 1 ) & 1
    b = ( i >> 2 ) & 1
    a = ( i >> 3 ) & 1
    return a, b, c, d

def merge(a, b, c, d):
    return a << 3 | b << 2 | c << 1 | d

print(bool(all( f1_bar(*split(i)) == f1_bar_pos(*split(i)) for i in range(16) )))
print(bool(all( f1_bar(*split(i)) != f1_pos(*split(i))     for i in range(16) )))
print(bool(all( f1(*split(i))     == f1_pos(*split(i))     for i in range(16) )))
print(bool(all( f1(*split(i))     != f1_bar_pos(*split(i)) for i in range(16) )))
print(bool(all( f1(*split(i))     != f1_bar(*split(i))     for i in range(16) )))

for i in range(16):
    m = split(i)
    print( 'f(%d, %d, %d, %d) = %d' % (*m, f1(*m)) )
