from data.db import conn


def add_widget_configuration(widget_id: int, name: str) -> dict:
    c = conn.cursor()
    c.execute('''
    INSERT INTO widget_configurations (widget_id, name) VALUES (?, ?)
    ''', (widget_id, name))
    conn.commit()
    if conn.total_changes:
        configuration_id = c.lastrowid
        return get_widget_configuration(configuration_id)
    return {}


def get_widget_configuration(id: int) -> dict:
    c = conn.cursor()
    c.execute('''
    SELECT * FROM widget_configurations WHERE id = ?
    ''', (id,))
    return c.fetchone()


def get_widget_from_configuration_id(configuration_id: int) -> dict:
    c = conn.cursor()
    c.execute('''
    SELECT widgets.* FROM widgets
    JOIN widget_configurations ON widgets.id = widget_configurations.widget_id
    WHERE widget_configurations.id = ?
    ''', (configuration_id,))
    return c.fetchone()


def delete_widget_configuration(id: int) -> None:
    c = conn.cursor()
    c.execute('''
    DELETE FROM widget_configurations WHERE id = ?
    ''', (id,))
    conn.commit()

