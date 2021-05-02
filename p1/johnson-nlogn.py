def johnson(_, T):
    t = []
    for i in xrange(0, len(T), 2):
        if T[i] <= T[i + 1]:
            t.append((T[i], 1))
        else:
            t.append((T[i + 1], 2))

    t = [(i[1][0], i[1][1], i[0]) for i in enumerate(t, 1)]

    t.sort()

    S1 = [i[2] for i in t if i[1] == 1]
    S2 = [i[2] for i in reversed(t) if i[1] == 2]

    return S1 + S2

print johnson([1, 2, 3], [5, 2, 3, 3, 1, 5])
