from __future__ import division

v = [11.8, 9.3, 18.9, 40., 1.3, 4., 20., 19.2]
w = [2, 3, 7, 4, 1, 5, 5, 6]

def kesi(v, i):
    v = sorted(enumerate(v), key=lambda x: x[1], reverse=True)
    return v[i][0]

def kpfrac(v, w, W):
    bag = []
    vw = [x / y for x, y in zip(v, w)]
    rw = W
    i = 0

    while rw > 0:
        p = kesi(vw, i)
        bag.append((p + 1, min(w[p], rw) / w[p]))
        rw = max(0, rw - w[p])
        i += 1

    return bag

print kpfrac(v, w, 10)
