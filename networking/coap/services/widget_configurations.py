import logging
import os

import aiocoap
from aiocoap import resource
import base64

from configs import get_widget_configuration_path
from data.repositories.widgets import *
from data.repositories.widget_configurations import *
from coap.responses import *
logger = logging.getLogger(name="coap.services.widget_configurations")

class WidgetConfigurations(resource.Resource):

    # async def render_delete(self, request):
    #     """
    #     Delete a configuration for a widget and remove the corresponding files
    #     """
    #     logger.info("Received DELETE request to widget_configurations")
    #
    #     # The request is of the kind {"id": config_id}
    #     payload = request.payload.decode()
    #
    #     # Get the config_id
    #     config_id = json.loads(payload)["id"]
    #
    #     # Get the configuration from the database
    #     config = get_widget_configuration(config_id)
    #
    #     if not config:
    #         logger.error("Configuration not found in the database")
    #         return error_response("Configuration not found in the database")
    #
    #     # Get configuration path
    #     configuration_path = get_configuration_path(config_id)
    #
    #     # Delete the configuration from the database
    #     delete_widget_configuration(config_id)
    #
    #     # Remove the configuration directory
    #     try:
    #         os.remove(configuration_path)
    #     except Exception as e:
    #         logger.error("Failed to remove configuration directory: {}".format(e))
    #         return error_response("Failed to remove configuration directory")
    #
    #     return success_response(None, "Configuration deleted successfully")

    async def render_post(self, request):
        """
        Expected payload format: widget_id,config_name,file_base64
        This method will save the configuration file to the disk for the widget with the given ID
        """
        logger.info("Received POST request to widget_configurations")

        # The request is of the kind widget_id,config_name,file_base64
        payload = request.payload.decode()

        # Split the payload
        payload_split = payload.split(",")

        if len(payload_split) != 3:
            logger.error("Invalid payload format")
            return error_response("Invalid payload format")

        # Get the widget_id
        widget_id = payload_split[0]

        # Get the config_name
        config_name = payload_split[1]

        # Log the values
        logger.info("Widget ID: {}".format(widget_id))
        logger.info("Config Name: {}".format(config_name))

        # Check if widget is found in the database
        widget = get_widget(widget_id)
        if not widget:
            logger.error("Widget not found in the database")
            return error_response("Widget not found in the database")

        # Check if config with same name already exists
        widget_configs = get_widget_configurations(widget_id)
        for config in widget_configs:
            if config["name"] == config_name:
                logger.error("Configuration with the same name already exists")
                return error_response("Configuration with the same name already exists")

        # Get the file_base64
        file_base64 = payload_split[2]

        # Convert the base64 to bytes
        try:
            config_archive_bytes = base64.b64decode(file_base64)
        except Exception as e:
            logger.error("Failed to decode base64: {}".format(e))
            return error_response("Failed to decode base64")

        # Get the path to save the configuration
        widget = get_widget(widget_id)
        config_path = get_widget_configuration_path(widget["author"], widget["name"])

        # Create the directory if it does not exist
        if not os.path.exists(config_path):
            os.makedirs(config_path)

        # Sanitize the config_name by keeping only alphanumeric characters
        config_name = ''.join(e for e in config_name if e.isalnum())

        # Save the configuration to the disk
        config_archive_path = os.path.join(config_path, config_name + ".tar.gz")
        try:
            with open(config_archive_path, "wb") as file:
                file.write(config_archive_bytes)
        except Exception as e:
            logger.error("Failed to save file: {}".format(e))
            return error_response("Failed to save configuration file")

        # Extract the configuration
        try:
            command = "tar -xzf {} -C {}".format(config_archive_path, config_path)
            logger.info("Extracting configuration with command: {}".format(command))
            os.system(command)
        except Exception as e:
            logger.error("Failed to extract configuration: {}".format(e))
            return error_response("Failed to extract configuration")

        # Add the configuration to the database
        add_widget_configuration(widget_id, config_name)

        return success_response(None,"Configuration added successfully")
