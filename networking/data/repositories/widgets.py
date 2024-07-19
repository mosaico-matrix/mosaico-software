from data.db import conn


def add_widget(store_id, name: str, author: str, metadata: str):
    c = conn.cursor()
    c.execute('''
    INSERT INTO widgets (store_id, name, author, metadata) VALUES (?, ?, ?, ?)
    ''', (store_id, name, author, metadata))
    conn.commit()


def update_widget_metadata(widget_id: int, metadata: str):
    c = conn.cursor()
    c.execute('''
    UPDATE widgets SET metadata = ? WHERE id = ?
    ''', (metadata, widget_id))
    conn.commit()


def get_installed_widgets():
    c = conn.cursor()
    c.execute('''
    SELECT * FROM widgets WHERE hidden = 0
    ''')
    return c.fetchall()

def get_widget_by_name_author(name: str, author: str):
    c = conn.cursor()
    c.execute('''
    SELECT * FROM widgets WHERE name = ? AND author = ?
    ''', (name, author))
    return c.fetchone()


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

def disable_widget(widget_id: int):
    c = conn.cursor()
    # instead of deleting widget, just mark it as hidden (prevent from losing configurations)
    c.execute('''
    UPDATE widgets SET hidden = 1 WHERE id = ?
    ''', (widget_id,))
    conn.commit()

def delete_widget(widget_id: int):
    """
    This should not be used as it will delete all configurations
    """
    c = conn.cursor()
    c.execute('''
    DELETE FROM widgets WHERE id = ?
    ''', (widget_id,))
    conn.commit()

def enable_widget(widget_id: int):
    c = conn.cursor()
    c.execute('''
    UPDATE widgets SET hidden = 0 WHERE id = ?
    ''', (widget_id,))
    conn.commit()