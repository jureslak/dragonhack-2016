import sqlite3
conn = sqlite3.connect('stat.db')
c = conn.cursor();

try: c.execute('''DROP TABLE runs''')
except sqlite3.OperationalError: pass

try: c.execute('''DROP TABLE moves''')
except sqlite3.OperationalError: pass
c.execute('''
CREATE TABLE runs (id INTEGER PRIMARY KEY AUTOINCREMENT, optimal INTEGER, start TEXT,
end TEXT, diff INTEGER, time REAL, acc REAL)''')

c.execute('''
CREATE TABLE moves (id INTEGER PRIMARY KEY AUTOINCREMENT, time REAL,
start INTEGER, end INTEGER, ball INTEGER, run INTEGER,
FOREIGN KEY(run) REFERENCES runs(id))''')
