from bottle import route, run, template, static_file

@route('/static/<filename:path>')
def send_static(filename):
    return static_file(filename, root='./static')

@route('/')
def main():
    return template('main')

@route('/statistics/')
def statistics():
    games_played = 0
    optimal_games_played = 0
    avg_opt_sol_len = 0
    return template('statistics', games_played=games_played,
                    optimal_games_played=optimal_games_played,
                    avg_opt_sol_len=avg_opt_sol_len)

@route('/about/')
def about():
    return template('about')

run(host='localhost', port=8080)
