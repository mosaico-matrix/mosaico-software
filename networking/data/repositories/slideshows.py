from data.db import conn


def get_slideshows() -> list:
    """
    Return a list of all slideshows with their related items.
    """
    c = conn.cursor()
    c.execute('''
    SELECT * FROM slideshows
    ''')
    slideshows = c.fetchall()

    for slideshow in slideshows:
        c.execute('''
        SELECT * FROM slideshow_items WHERE slideshow_id = ? ORDER BY position
        ''', (slideshow['id'],))
        slideshow['items'] = c.fetchall()

    return slideshows

def get_slideshow_by_name(name: str) -> dict:
    """
    Return a single slideshow by name with its related items.
    """
    c = conn.cursor()
    c.execute('''
    SELECT * FROM slideshows WHERE name = ?
    ''', (name,))
    slideshow = c.fetchone()

    if slideshow:
        c.execute('''
        SELECT * FROM slideshow_items WHERE slideshow_id = ? ORDER BY position
        ''', (slideshow['id'],))
        slideshow['items'] = c.fetchall()

    return slideshow

def get_slideshow(id: int) -> dict:
    """
    Return a single slideshow by id with its related items.
    """
    c = conn.cursor()
    c.execute('''
    SELECT * FROM slideshows WHERE id = ?
    ''', (id,))
    slideshow = c.fetchone()

    if slideshow:
        c.execute('''
        SELECT * FROM slideshow_items WHERE slideshow_id = ? ORDER BY position
        ''', (id,))
        slideshow['items'] = c.fetchall()

    return slideshow


def add_slideshow(name: str, items: list) -> dict:
    """
    Add a new slideshow with the given name and items.
    """
    c = conn.cursor()
    c.execute('''
    INSERT INTO slideshows (name) VALUES (?)
    ''', (name,))
    slideshow_id = c.lastrowid

    for item in items:
        c.execute('''
        INSERT INTO slideshow_items (slideshow_id, widget_id, config_id, seconds_duration, position)
        VALUES (?, ?, ?, ?, ?)
        ''', (slideshow_id, item.get('widgetId'), item.get('configId'), item.get('secondsDuration', 10), item.get('position')))

    conn.commit()
    return get_slideshow(slideshow_id)


def update_slideshow(id: int, name: str, items: list) -> dict:
    """
    Update an existing slideshow's name and items.
    """
    c = conn.cursor()
    c.execute('''
    UPDATE slideshows SET name = ? WHERE id = ?
    ''', (name, id))

    c.execute('''
    DELETE FROM slideshow_items WHERE slideshow_id = ?
    ''', (id,))

    for item in items:
        c.execute('''
        INSERT INTO slideshow_items (slideshow_id, widget_id, config_id, seconds_duration, position)
        VALUES (?, ?, ?, ?, ?)
        ''', (id, item.get('widgetId'), item.get('configId'), item.get('secondsDuration', 10), item.get('position')))

    conn.commit()
    return get_slideshow(id)


def delete_slideshow(id: int) -> None:
    """
    Delete a slideshow by id.
    """
    c = conn.cursor()
    c.execute('''
    DELETE FROM slideshow_items WHERE slideshow_id = ?
    ''', (id,))

    c.execute('''
    DELETE FROM slideshows WHERE id = ?
    ''', (id,))

    conn.commit()
