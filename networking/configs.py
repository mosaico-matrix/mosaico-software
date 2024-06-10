# PATHS
import os

program_path = os.path.dirname(os.path.abspath(__file__))

def get_widgets_path():
    return f"{program_path}/storage/widgets"


def get_db_path():
    return f"{program_path}/data/db.sqlite"
