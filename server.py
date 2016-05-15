from bottle import route, run, template, static_file, post, request, view
from libtower import *
from subprocess import Popen, PIPE
import threading
import json
import sqlite3

class RunData: pass

proc = None
output = RunData()
conn = sqlite3.connect('stat.db')

def fun():
    global proc, out
    (output.out, _) = proc.communicate()
    proc = None

proc_thread = None

@post('/is_running/')
def is_running():
    return json.dumps(proc is not None)

@post('/runapp/')
def run_program():
    global proc, output, proc_thread
    proc_thread = threading.Thread(None, fun)
    m = int(request.forms.get('mini'))
    M = int(request.forms.get('maxi'))

    output.start, output.end, output.path = random_problem(m, M)
    proc  = Popen('./jimm', stdin=PIPE, stdout=PIPE)
    s = output_problem(HEIGHTS, output.start, output.end)
    proc.stdin.write(bytes(s, 'utf8'))
    proc_thread.start()

    return json.dumps({
        'start': output.start,
        'end': output.end,
        'path': output.path,
    })

from copy import deepcopy
@post('/get_result/')
def get_result():
    data = output.out.decode('utf8')
    data = data.strip('\n').split('\n')
    print(data)
    code, acc = data.pop().split()
    if code != '-1':
        return json.dumps({'fail': True})
    global conn, out
    c = conn.cursor()
    moves = [['Move number', 'From peg', 'To peg', 'Ball moved', 'Time needed']]
    total = 0
    acc = float(acc);
    for cnt, i in enumerate(data):
        i = i.split()
        moves.append(['Move {}'.format(cnt+1),
            int(i[0]), int(i[1]), int(i[2]), float(i[3])
        ])
        total += moves[-1][-1]
    print(moves)
    your_path = [str2state(output.start)]
    for i in moves[1:]:
        cur = deepcopy(your_path[-1])
        tmp = cur[i[1]-1].pop()
        cur[i[2]-1].append(tmp)
        your_path.append(cur)
    your_path = list(map(state2str, your_path))
#      print(your_path)

    c.execute('''INSERT into runs VALUES (?, ?, ?, ?, ?, ?, ?)''',
              (None, len(output.path), output.start, output.end,
              len(data), total, acc))
    key = c.execute('''SELECT MAX(id) FROM runs''').fetchone()
    key = key[0]
    if key is None: key = 0
    for i in moves:
        c.execute('''INSERT INTO moves VALUES (?, ?, ?, ?, ?, ?)''',
            (None, i[3], i[0], i[1], i[2], key))
    conn.commit()
    return json.dumps({'fail': False, 'moves': moves, 'your_path': your_path, 'acc': acc})

@route('/static/<filename:path>')
def send_static(filename):
    return static_file(filename, root='./static')

@route('/play/')
def play():
    return template('play', current_page='play')

@route('/')
def main():
    return template('main', current_page='home')

@route('/statistics/')
def statistics():
    global conn
    c = conn.cursor()
    key = c.execute('''SELECT MAX(id) FROM runs''').fetchone()
    key = key[0]
    if key is None: key = 0
    games_played = key
    optimal_games_played = 0
    avg_opt_sol_len = 0
    avg_usr_sol_len = 0
    moves = []
    opt_moves = []
    r = c.execute('''SELECT * FROM runs ORDER BY id DESC LIMIT 100''')
    acc = []
    cnt = 0
    for x in r:
        cnt += 1
        uid, opt, start, end, user, time, accc = x
        avg_opt_sol_len += opt
        avg_usr_sol_len += user
        optimal_games_played += opt == user
        opt_moves.append({'x': cnt, 'y': opt})
        moves.append({'x': cnt, 'y': user})
        acc.append({'x': cnt, 'y': accc})

    avg_opt_sol_len /= games_played
    avg_opt_sol_len = round(avg_opt_sol_len, 2)

    return template('statistics', current_page='statistics', games_played=games_played,
                    optimal_games_played=optimal_games_played,
                    avg_opt_sol_len=avg_opt_sol_len, avg_usr_sol_len=avg_usr_sol_len,
                    opt_moves=opt_moves, usr_moves=moves, acc=acc)

@route('/about/')
def about():
    return template('about', current_page='about')

run(host='localhost', port=8080)
