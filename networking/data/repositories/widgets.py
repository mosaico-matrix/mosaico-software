from data.db import conn
from configs import get_widgets_path


def add_widget(store_id: int, name: str, author: str):
    c = conn.cursor()
    c.execute('''
    INSERT INTO widgets (store_id, name, author) VALUES (?, ?, ?)
    ''', (store_id, name, author))
    conn.commit()


def get_installed_widgets():
    c = conn.cursor()
    c.execute('''
    SELECT * FROM widgets
    ''')
    return c.fetchall()


def get_widget(id):
    c = conn.cursor()
    c.execute('''
    SELECT * FROM widgets WHERE id = ?
    ''', (id,))
    return c.fetchone()

def get_widget_by_store_id(store_id):
    c = conn.cursor()
    c.execute('''
    SELECT * FROM widgets WHERE store_id = ?
    ''', (store_id,))
    return c.fetchone()

def get_widget_configurations(widget_id: int) -> list:
    c = conn.cursor()
    c.execute('''
    SELECT * FROM widget_configurations WHERE widget_id = ?
    ''', (widget_id,))
    return c.fetchall()


def delete_widget(widget_id: int):
    c = conn.cursor()
    c.execute('''
    DELETE FROM widgets WHERE id = ?
    ''', (widget_id,))
