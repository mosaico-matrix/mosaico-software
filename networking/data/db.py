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
        author TEXT NOT NULL
    )
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
