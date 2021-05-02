def johnson(J, T):
    S1 = []
    S2 = []
    T = zip(T, [i + 1 for i in xrange(len(T)) for _ in xrange(2)], [i + 1 for _ in xrange(len(T)) for i in xrange(2)])
    while len(J) > 0:
        jmin, tminIndex = min(T)[1:]
        J.remove(jmin)
        del T[2 * jmin - 2:2 * jmin]
        if tminIndex % 2 != 0:
            S1.append(jmin)
        else:
            S2.append(jmin)
    return S1 + list(reversed(S2))

print johnson([1, 2, 3], [5, 2, 3, 3, 1, 5])
