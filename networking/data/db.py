import sqlite3
from core.configs import get_db_path


def row_to_dict(cursor: sqlite3.Cursor, row: sqlite3.Row):
    if row is None:
        return None

    data = {}
    for idx, col in enumerate(cursor.description):
        data[col[0]] = row[idx]
    return data


conn = sqlite3.connect(get_db_path())
conn.row_factory = row_to_dict


def create_tables():
    c = conn.cursor()

    # Return if installed_widgets table already exists
    c.execute('''
    SELECT name FROM sqlite_master WHERE type='table' AND name='widgets'
    ''')

    if c.fetchone():
        return

    # Create widgets table
    c.execute('''
    CREATE TABLE widgets (
        id INTEGER PRIMARY KEY AUTOINCREMENT, 
        store_id INTEGER UNIQUE, 
        name TEXT NOT NULL,
        author TEXT NOT NULL,
        metadata TEXT NOT NULL,
        hidden INTEGER DEFAULT 0
    )
    ''')
    c.execute('''
    CREATE INDEX idx_widgets_hidden ON widgets (hidden)
    ''')
    c.execute('''
    CREATE INDEX idx_widgets_name_author ON widgets (name, author)
    ''')

    # Create widget_configurations table
    c.execute('''
    CREATE TABLE widget_configurations (
        id INTEGER PRIMARY KEY AUTOINCREMENT,
        widget_id INTEGER NOT NULL,
        name TEXT NOT NULL,
        FOREIGN KEY (widget_id) REFERENCES widgets (id)
    )
    ''')

    # Create slideshows table
    c.execute('''
    CREATE TABLE slideshows (
        id INTEGER PRIMARY KEY AUTOINCREMENT,
        name TEXT NOT NULL UNIQUE
    )
    ''')

    # Create slideshow_items table
    c.execute('''
    CREATE TABLE slideshow_items (
        id INTEGER PRIMARY KEY AUTOINCREMENT,
        slideshow_id INTEGER NOT NULL,
        widget_id INTEGER NOT NULL,
        config_id INTEGER,
        position INTEGER NOT NULL,
        seconds_duration INTEGER DEFAULT 10,
        FOREIGN KEY (slideshow_id) REFERENCES slideshows (id),
        FOREIGN KEY (widget_id) REFERENCES widgets (id),
        FOREIGN KEY (config_id) REFERENCES widget_configurations (id)
    )
    ''')

    # Create settings table
    c.execute('''
    CREATE TABLE settings (
        key TEXT PRIMARY KEY,
        value TEXT
    )
    ''')

    conn.commit()


def seed_db():
    c = conn.cursor()
    conn.commit()


def init():
    create_tables()
    seed_db()
