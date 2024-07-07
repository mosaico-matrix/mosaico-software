import asyncio
import logging

import data.repositories.slideshows
import data.repositories.widgets as local_widgets
from core import configs
from core.interop import call_matrix
from data.repositories import settings
from data.repositories.widget_configurations import get_widget_configuration

logger = logging.getLogger('mosaico_networking')


def set_active_widget(widget_id, config_id):
    """
    Set the active widget
    """
    logger.info("Setting active widget with id: " + str(widget_id) + " and config_id: " + str(config_id))

    # Get widget
    widget = local_widgets.get_widget(widget_id)
    if widget is None:
        return

    # Get widget configuration
    widget_config = get_widget_configuration(config_id)

    # Set widget on matrix
    call_matrix("LOAD_WIDGET",
                {
                    "widget_path": configs.get_widget_path(widget["author"], widget["name"]),
                    "config_path":
                        ""
                        if not widget_config
                        else
                        configs.get_widget_configuration_path(widget["author"], widget["name"], widget_config["name"])
                })

    # Save the active widget to the database
    settings.set_active_widget_id(widget_id)
    settings.set_active_config_id(config_id)


async def start_slideshow(slideshow_id):

    logger.info("Starting slideshow with id: " + str(slideshow_id))

    # Get slideshow
    slideshow = data.repositories.slideshows.get_slideshow(slideshow_id)

    # Get slideshow next widget
    for slideshow_item in slideshow['items']:
        set_active_widget(slideshow_item['widget_id'], slideshow_item['config_id'])

        #logger.error("Waiting for " + str(slideshow_item['seconds_duration']) + " seconds")

        # Wait for the duration of the widget
        await asyncio.sleep(slideshow_item['seconds_duration'])


async def set_active_slideshow(slideshow_id):
    logger.info("Received request to set active slideshow")

    # Create a background task for the slideshow
    asyncio.create_task(start_slideshow(slideshow_id))


def restore_last_session():
    # Check if there is an active widget
    if settings.get_active_widget_id() is not None:
        set_active_widget(settings.get_active_widget_id(), settings.get_active_config_id())

    # Check if there is an active slideshow
