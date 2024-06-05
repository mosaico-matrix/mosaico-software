import sqlite3

db_path = "db.sqlite"
conn = sqlite3.connect(db_path)

def create_tables():
    c = conn.cursor()
    conn.commit()

def seed_db():
    c = conn.cursor()
    conn.commit()

def init():
    create_tables()
    seed_db()