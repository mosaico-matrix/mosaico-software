import asyncio
import logging

import data.repositories.slideshows
import data.repositories.widgets as local_widgets
from core import configs
from core.interop import call_matrix
from data.repositories import settings
from data.repositories.widget_configurations import get_widget_configuration

logger = logging.getLogger('mosaico_networking')

# Global variables to hold the current slideshow task and lock
current_slideshow_task = None
slideshow_lock = asyncio.Lock()

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

    while True:  # Loop indefinitely to restart the slideshow when it ends
        # Get slideshow
        slideshow = data.repositories.slideshows.get_slideshow(slideshow_id)

        # Get slideshow next widget
        for slideshow_item in slideshow['items']:
            set_active_widget(slideshow_item['widget_id'], slideshow_item['config_id'])

            # Wait for the duration of the widget
            logger.info("Waiting for " + str(slideshow_item['seconds_duration']) + " seconds before switching to the next widget")
            await asyncio.sleep(slideshow_item['seconds_duration'])

async def set_active_slideshow(slideshow_id):

    global current_slideshow_task

    logger.info("Received request to set active slideshow")

    async with slideshow_lock:  # Acquire the lock before changing the slideshow
        # If there's a currently running slideshow, cancel it
        if current_slideshow_task:
            current_slideshow_task.cancel()
            try:
                await current_slideshow_task
            except asyncio.CancelledError:
                logger.info("Previous slideshow was cancelled")

        # Create a new background task for the slideshow
        settings.set_active_slideshow_id(slideshow_id)
        current_slideshow_task = asyncio.create_task(start_slideshow(slideshow_id))



async def restore_last_session():

    # Check for active slideshow
    active_slideshow_id = settings.get_active_slideshow_id()
    if active_slideshow_id is not None:
        logger.info("Restoring last active slideshow with id: " + str(active_slideshow_id))
        await set_active_slideshow(active_slideshow_id)
        return


    # Check if there is an active widget
    active_widget_id = settings.get_active_widget_id()
    if active_widget_id is not None:
        logger.info("Restoring last active widget with id: " + str(active_widget_id))
        set_active_widget(active_widget_id, settings.get_active_config_id())


