from data.db import conn


def add_widget(id, name, author):
    c = conn.cursor()
    c.execute('''
    INSERT INTO installed_widgets (id, name, author) VALUES (?, ?, ?)
    ''', (id, name, author))
    conn.commit()


def get_installed_widgets():
    c = conn.cursor()
    c.execute('''
    SELECT * FROM installed_widgets
    ''')
    return c.fetchall()


def get_widget_by_id(id):
    c = conn.cursor()
    c.execute('''
    SELECT * FROM installed_widgets WHERE id = ?
    ''', (id,))
    return c.fetchone()
