from data.db import conn


def add_widget_configuration(widget_id: int, name: str) -> None:
    c = conn.cursor()
    c.execute('''
    INSERT INTO widget_configurations (widget_id, name) VALUES (?, ?)
    ''', (widget_id, name))
    conn.commit()


def get_widget_configuration(id: int) -> dict:
    c = conn.cursor()
    c.execute('''
    SELECT * FROM widget_configurations WHERE id = ?
    ''', (id,))
    return c.fetchone()


def delete_widget_configuration(id: int) -> None:
    c = conn.cursor()
    c.execute('''
    DELETE FROM widget_configurations WHERE id = ?
    ''', (id,))
    conn.commit()