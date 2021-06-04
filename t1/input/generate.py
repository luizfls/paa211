import random

edges = []

for _ in xrange(50):
    src = random.randint(1, 20)
    dst = random.randint(1, 20)
    if src == dst:
        continue
    elif src > dst:
        src, dst = dst, src
    edges.append((src, dst))

edges = list(set(edges))

weights = []
for _ in xrange(len(edges)):
    weights.append(random.randint(1, 15))

with open("sample2.txt", "w") as f:
    for edge, weight in zip(edges, weights):
        f.write("a %d %d %d\n" % (edge[0], edge[1], weight))
        f.write("a %d %d %d\n" % (edge[1], edge[0], weight))
