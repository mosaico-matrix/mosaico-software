# PATHS
import os

program_path = os.path.dirname(os.path.abspath(__file__))

def get_widgets_path():
    return f"{program_path}/storage/widgets"

def get_widget_path(author: str, name: str):
    return f"{get_widgets_path()}/{author}/{name}"

def get_widget_configurations_path():
    return f"{program_path}/storage/configs"

def get_widget_configuration_path(author: str, widget_name: str, config_name: str):
    return f"{get_widget_configurations_path()}/{author}/{widget_name}/{config_name}"

def get_db_path():
    return f"{program_path}/data/db.sqlite"
