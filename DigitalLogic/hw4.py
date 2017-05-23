def function(a, bc, d):
    f1 = not a
    f2 = not ( f1 and d )
    f3 = not ( f1 and f2 )
    f4 = f1 or bc
    f5 = f3 and f4
    f6 = f4 and f2

    return f5, f6

for i in range(16):
    d = ( i >> 0 ) & 1
    c = ( i >> 1 ) & 1
    b = ( i >> 2 ) & 1
    a = ( i >> 3 ) & 1
    bc = b & c
    print( 'f(%d, %d, %d, %d) = %d, %d' % (a, b, c, d, *function(a, bc, d)) )
