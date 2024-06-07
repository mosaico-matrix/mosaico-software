import sqlite3

db_path = "data/db.sqlite"
conn = sqlite3.connect(db_path)


def add_widget(id, name):
    c = conn.cursor()
    c.execute('''
    INSERT INTO installed_widgets(name, id) VALUES(?, ?)
    ''', (name, id))
    conn.commit()


def get_installed_widgets():
    c = conn.cursor()
    c.execute('''
    SELECT * FROM installed_widgets
    ''')
    return c.fetchall()

def get_widget_by_id(id):
    c = conn.cursor()
    c.execute('''
    SELECT * FROM installed_widgets WHERE id = ?
    ''', (id,))
    return c.fetchone()


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
        name TEXT NOT NULL
    )
    ''')

    conn.commit()


def seed_db():
    c = conn.cursor()
    conn.commit()


def init():
    create_tables()
    seed_db()
