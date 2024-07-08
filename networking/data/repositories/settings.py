from data.db import conn

def set_setting(key: str, value: str) -> None:
    """
    Sets the value for a given key in the settings table. If the key exists, it updates the value.
    If the key does not exist, it inserts a new key-value pair.

    :param key: The key to set or update in the settings table.
    :param value: The value to associate with the key.
    """
    c = conn.cursor()

    # Check if key already exists
    c.execute('SELECT key FROM settings WHERE key = ?', (key,))

    # Update value if key exists, otherwise insert new key-value pair
    if c.fetchone():
        c.execute('UPDATE settings SET value = ? WHERE key = ?', (value, key))
    else:
        c.execute('INSERT INTO settings (key, value) VALUES (?, ?)', (key, value))

    conn.commit()


def get_setting(key: str) -> str:
    """
    Retrieves the value associated with a given key from the settings table.

    :param key: The key to look up in the settings table.
    :return: The value associated with the key, or None if the key does not exist.
    """
    c = conn.cursor()
    c.execute('SELECT value FROM settings WHERE key = ?', (key,))

    result = c.fetchone()
    return result['value'] if result else None


# Specific functions using the generic functions
def set_active_widget_id(widget_id) -> None:
    set_setting('active_widget', widget_id)


def get_active_widget_id():
    return get_setting('active_widget')


def set_active_config_id(configuration_id) -> None:
    set_setting('active_configuration', configuration_id)


def get_active_config_id():
    return get_setting('active_configuration')


def set_active_slideshow_id(slideshow_id) -> None:
    set_setting('active_slideshow', slideshow_id)


def get_active_slideshow_id():
    return get_setting('active_slideshow')