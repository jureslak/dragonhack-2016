STICKS = 3
HEIGHTS = [2, 3, 4]
assert STICKS == len(HEIGHTS)
BALLS = 4

from random import randint, choice
from collections import deque

def state2str(state):
    return "|".join(''.join(reversed(list(map(str, i)))) for i in state)

def str2state(s):
    return [list(reversed(list(map(int, x)))) for x in s.split('|')]

def make_graph(h, b):
    s = []
    left = b
    c = 1
    for i in range(len(h)):
        if h[i] <= left:
            s.append([])
            for j in range(h[i]):
                s[-1].append(c)
                c += 1
            left -= h[i]
        else:
            s.append([])
            for j in range(left):
                s[-1].append(c)
                c += 1
            left = 0

    while len(s) < len(h): s.append([])

    q = deque([state2str(s)])
    v = set()
    while q:
        s = q.popleft()
        if s in v: continue
        v.add(s)

        st = str2state(s)
        for i in range(len(st)):
            if st[i]:
                to_move = st[i].pop()
                for j in range(len(st)):
                    if j == i: continue
                    if len(st[j]) == h[j]: continue
                    st[j].append(to_move)
                    s = state2str(st)
                    if s not in v:
                        q.append(s)
                    st[j].pop()
                st[i].append(to_move)

    G = {i: [] for i in v}
    for s in G:
        st = str2state(s)
        for i in range(len(st)):
            if st[i]:
                to_move = st[i].pop()
                for j in range(len(st)):
                    if j == i: continue
                    if len(st[j]) == h[j]: continue
                    st[j].append(to_move)
                    t = state2str(st)
                    G[t].append(s)
                    G[s].append(t)
                    st[j].pop()
                st[i].append(to_move)
    return G

GRAPH = make_graph(HEIGHTS, BALLS)
NODES = list(GRAPH)

def find_shortest_path(s, t):
    if s == t: return [s]

    q = deque([(s, -1)])
    v = set()
    prev = dict()
    while q:
        c, p = q.popleft()
        if c in v: continue
        v.add(c)
        prev[c] = p
        if c == t:
            break

        for n in GRAPH[c]:
            if n not in v:
                q.append((n, c))

    path = []
    x = t
    while x != -1:
        path.append(x)
        x = prev[x]
    return list(reversed(path))

#  s = '14|2|3'
#  t = '3|2|14'
#  print(str2state(s))
#  path = find_shortest_path(s, t)
#  print(path)

# returns problem with path length in [m, M]
# TODO change back
def random_problem(m, M):
    while True:
        a = choice(NODES)
        b = choice(NODES)
        a = '14|2|3'
        b = '3|2|14'
        path = find_shortest_path(a, b)
#          if m <= len(path) <= M:
        return a, b, path

def output_problem(h, start, end):
    s = '{} {}\n'.format(len(h), len(start) - len(h) + 1)
    for i in str2state(start):
        s += '{} '.format(len(i))
        for j in i:
            s += '{} '.format(j)
        s += '\n'
    for i in str2state(end):
        s += '{} '.format(len(i))
        for j in i:
            s += '{} '.format(j)
        s += '\n'
    return s

#  print(output_problem(HEIGHTS, s, t), end='')
