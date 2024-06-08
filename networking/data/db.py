import sqlite3


def row_to_dict(cursor: sqlite3.Cursor, row: sqlite3.Row) -> dict:
    data = {}
    for idx, col in enumerate(cursor.description):
        data[col[0]] = row[idx]
    return data


db_path = "data/db.sqlite"
conn = sqlite3.connect(db_path)
conn.row_factory = row_to_dict

def create_tables():

    c = conn.cursor()

    # Return if installed_widgets table already exists
    c.execute('''
    SELECT name FROM sqlite_master WHERE type='table' AND name='installed_widgets'
    ''')

    if c.fetchone():
        return

    # Create installed_widgets table
    c.execute('''
    CREATE TABLE installed_widgets (
        id INTEGER PRIMARY KEY,
        name TEXT NOT NULL,
        author TEXT NOT NULL
    )
    ''')

    conn.commit()


def seed_db():
    c = conn.cursor()
    conn.commit()


def init():
    create_tables()
    seed_db()
