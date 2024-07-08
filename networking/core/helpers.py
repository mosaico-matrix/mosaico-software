import data.repositories.widgets as local_widgets
import data.repositories.settings as settings
from data.repositories.widget_configurations import get_widget_configuration


def get_active_widget():
    """
    Get the active widget from the local db
    """
    return local_widgets.get_widget(settings.get_active_widget_id())

def get_active_configuration():
    """
    Get the active widget configuration from the local db
    """
    return get_widget_configuration(settings.get_active_config_id())

def get_active_slideshow():
    """
    Get the active slideshow from the local db
    """
    return local_widgets.get_widget(settings.get_active_widget_id())