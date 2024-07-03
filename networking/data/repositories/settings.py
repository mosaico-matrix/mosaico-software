from data.db import conn


def set_active_widget_id(widget_id) -> None:
    c = conn.cursor()

    # Check if key already exists
    c.execute('''
    SELECT key FROM settings WHERE key = 'active_widget'
    ''')

    # If key exists, update value
    if c.fetchone():
        c.execute('''
        UPDATE settings SET value = ? WHERE key = 'active_widget'
        ''', (widget_id,))

    # If key does not exist, insert new key-value pair
    else:
        c.execute('''
        INSERT INTO settings (key, value) VALUES ('active_widget', ?)
        ''', (widget_id,))

    conn.commit()


def get_active_widget_id():
    c = conn.cursor()
    c.execute('''
    SELECT value FROM settings WHERE key = 'active_widget'
    ''')

    result = c.fetchone()
    if result:
        return result['value']
    else:
        return None


def set_active_config_id(configuration_id) -> None:
    c = conn.cursor()

    # Check if key already exists
    c.execute('''
    SELECT key FROM settings WHERE key = 'active_configuration'
    ''')

    # If key exists, update value
    if c.fetchone():
        c.execute('''
        UPDATE settings SET value = ? WHERE key = 'active_configuration'
        ''', (configuration_id,))

    # If key does not exist, insert new key-value pair
    else:
        c.execute('''
        INSERT INTO settings (key, value) VALUES ('active_configuration', ?)
        ''', (configuration_id,))

    conn.commit()


def get_active_config_id():
    c = conn.cursor()
    c.execute('''
        SELECT value FROM settings WHERE key = 'active_configuration'
        ''')

    result = c.fetchone()
    if result:
        return result['value']
    else:
        return None
