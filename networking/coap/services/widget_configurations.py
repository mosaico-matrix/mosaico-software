import os

import base64

from core import utils, configs
from core.configs import get_widget_configuration_path
import data.repositories.widgets as local_widgets
from data.repositories.widget_configurations import *
from coap.responses import *
import coap.dynamic_resource
logger = logging.getLogger('mosaico_networking')

class WidgetConfigurations(coap.dynamic_resource.DynamicResource):

    async def render_post(self, request, args):
        """
        This method will save the configuration file to the disk for the widget with the given ID
        """
        logger.info("Received POST request to widget_configurations")

        # The request is of the kind config_name,file_base64
        payload = request.payload.decode()

        # Split the payload
        payload_split = payload.split(",")

        if len(payload_split) != 2:
            return error_response("Invalid payload format")

        # Get the widget_id
        widget_id = args['widget_id']

        # Get the config_name and file_base64
        config_name = payload_split[0]
        file_base64 = payload_split[1]


        # Log the values
        logger.info("Widget ID: {}".format(widget_id))
        logger.info("Config Name: {}".format(config_name))

        # Check if widget is found in the database
        widget = local_widgets.get_widget(widget_id)
        if not widget:
            return error_response("Widget not found in the database")

        # Check if config with same name already exists
        widget_configs = local_widgets.get_widget_configurations(widget_id)
        for config in widget_configs:
            if config["name"] == config_name:
                return error_response("Configuration with the same name already exists")

        # Get the path to save the configuration
        config_path = get_widget_configuration_path(widget["author"], widget["name"], config_name)

        # Create the directory if it does not exist
        try:
            if not os.path.exists(config_path):
                os.makedirs(config_path)
        except Exception as e:
            return error_response("Failed to create configuration directory")

        # Save the configuration to the disk
        try:
            utils.extract_archive_from_base64_bytes(file_base64, config_path)
        except Exception as e:
            return error_response("Failed to save configuration to the disk")

        # Add the configuration to the database
        config = add_widget_configuration(widget_id, config_name)

        return success_response(config)

    async def render_delete(self, request, args):
        """
        The configuration_id is found inside the args in the path template
        This method will delete the configuration with the given ID and all its files
        """
        logger.info("Received DELETE request to widget_configurations")
        configuration_id = args['configuration_id']

        # Get the configuration
        configuration = get_widget_configuration(configuration_id)
        logger.info("Configuration: {}".format(configuration))
        if not configuration:
            return error_response("Configuration not found in the database")

        # Get the widget of the configuration
        widget = local_widgets.get_widget(configuration["widget_id"])
        if not widget:
            return error_response("Widget not found in the database")

        # Delete the configuration
        delete_widget_configuration(configuration_id)

        # Get the path to the configuration
        config_path = get_widget_configuration_path(widget["author"], widget["name"], configuration["name"])

        # Remove the directory
        try:
            os.system("rm -rf {}".format(config_path))
        except Exception as e:
            return error_response("Failed to remove configuration directory, but configuration was deleted from the database")

        return success_response(None)

    async def render_get(self, request, args):
        """
        Get the configurations for the widget with the given ID
        """
        logger.info("Received GET request to widget_configurations")
        widget_id = args['widget_id']

        # Get the widget
        widget = local_widgets.get_widget(widget_id)
        if not widget:
            return error_response("Widget not found in the database")

        # Get the configurations
        widget_configs = local_widgets.get_widget_configurations(widget_id)
        return success_response(widget_configs)

class WidgetConfigurationPackage(coap.dynamic_resource.DynamicResource):

    async def render_get(self, request, args):
        """
        Get the configuration archive previously saved in order to edit existing configurations
        """
        logger.info("Received GET request to widget_configurations_package")
        configuration_id = args['configuration_id']

        # Get the configuration
        configuration = get_widget_configuration(configuration_id)
        if not configuration:
            return error_response("Configuration not found in the database")

        # Get the widget of the configuration
        widget = get_widget_from_configuration_id(configuration_id)
        if not widget:
            return error_response("Widget not found in the database")

        # Get the path to the archive
        archive_path = configs.get_widget_configuration_archive_path(widget["author"], widget["name"], configuration["name"])

        # Read the archive to base64
        try:
            with open(archive_path, "rb") as f:
                archive_base64 = base64.b64encode(f.read()).decode()
        except Exception as e:
            return error_response("Failed to read the archive")

        return success_response(archive_base64)