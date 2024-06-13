from data.db import conn
from configs import get_widgets_path


def add_widget(id, name, author):
    c = conn.cursor()
    c.execute('''
    INSERT INTO widgets (id, name, author) VALUES (?, ?, ?)
    ''', (id, name, author))
    conn.commit()


def get_installed_widgets():
    c = conn.cursor()
    c.execute('''
    SELECT * FROM widgets
    ''')
    return c.fetchall()


def get_widget_by_id(id):
    c = conn.cursor()
    c.execute('''
    SELECT * FROM widgets WHERE id = ?
    ''', (id,))
    return c.fetchone()


def get_widget_configurations(widget_id: int) -> list:
    c = conn.cursor()
    c.execute('''
    SELECT * FROM widgets WHERE widget_id = ?
    ''', (widget_id,))
    return c.fetchall()


def get_widget_path(id: int) -> str:
    c = conn.cursor()

    # Only get name and author
    c.execute('''
       SELECT name, author FROM widgets WHERE id = ?
       ''', (id,))

    widget = c.fetchone()

    if widget:
        return f"{get_widgets_path()}/{widget['author']}/{widget['name']}"
    else:
        return ""
