import os
import base64
import logging
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
        Save or update the configuration file to the disk
        """
        logger.info("Received POST request to widget_configurations")

        payload = request.payload.decode()
        config_name, file_base64, error = parse_payload(payload)
        if error:
            return error_response(error)

        if 'widget_id' in args:
            # Handle the creation of a new configuration
            widget_id = args['widget_id']

            widget, error = get_widget(widget_id)
            if error:
                return error_response(error)

            error = check_existing_configuration(widget_id, config_name)
            if error:
                return error_response(error)

            config_path, error = get_config_path(widget, config_name)
            if error:
                return error_response(error)

            error = save_configuration(file_base64, config_path)
            if error:
                return error_response(error)

            config = add_widget_configuration(widget_id, config_name)
            return success_response(config)

        elif 'configuration_id' in args:

            # Handle the updating of an existing configuration
            configuration_id = args['configuration_id']

            configuration = get_widget_configuration(configuration_id)
            if not configuration:
                return error_response("Configuration not found in the database")

            # Update config name
            rename_widget_configuration(configuration_id, config_name)
            configuration["name"] = config_name

            widget, error = get_widget(configuration["widget_id"])
            if error:
                return error_response(error)

            config_path, error = get_config_path(widget, configuration["name"])
            if error:
                return error_response(error)

            error = save_configuration(file_base64, config_path)
            if error:
                return error_response(error)

            return success_response(configuration)

        else:
            return error_response("Neither widget_id nor configuration_id provided in the request arguments")

    async def render_delete(self, request, args):
        """
        Delete the configuration with the given ID and all its files
        """
        logger.info("Received DELETE request to widget_configurations")
        configuration_id = args['configuration_id']

        configuration = get_widget_configuration(configuration_id)
        if not configuration:
            return error_response("Configuration not found in the database")

        widget, error = get_widget(configuration["widget_id"])
        if error:
            return error_response(error)

        delete_widget_configuration(configuration_id)

        config_path = get_widget_configuration_path(widget["author"], widget["name"], configuration["name"])

        error = remove_configuration_directory(config_path)
        if error:
            return error_response(error)

        return success_response(None)

    async def render_get(self, request, args):
        """
        Get the configurations for the widget with the given ID
        """
        logger.info("Received GET request to widget_configurations")
        widget_id = args['widget_id']

        widget, error = get_widget(widget_id)
        if error:
            return error_response(error)

        widget_configs = local_widgets.get_widget_configurations(widget_id)
        return success_response(widget_configs)


class WidgetConfigurationPackage(coap.dynamic_resource.DynamicResource):

    async def render_get(self, request, args):
        """
        Get the configuration archive previously saved to edit existing configurations
        """
        logger.info("Received GET request to widget_configurations_package")
        configuration_id = args['configuration_id']

        configuration = get_widget_configuration(configuration_id)
        if not configuration:
            return error_response("Configuration not found in the database")

        widget = get_widget_from_configuration_id(configuration_id)
        if not widget:
            return error_response("Widget not found in the database")

        archive_path = configs.get_widget_configuration_archive_path(widget["author"], widget["name"], configuration["name"])

        try:
            with open(archive_path, "rb") as f:
                archive_base64 = base64.b64encode(f.read()).decode()
        except Exception as e:
            return error_response("Failed to read the archive")

        return success_response(archive_base64)


def parse_payload(payload):
    payload_split = payload.split(",")
    if len(payload_split) != 2:
        return None, None, "Invalid payload format"
    return payload_split[0], payload_split[1], None


def get_widget(widget_id):
    widget = local_widgets.get_widget(widget_id)
    if not widget:
        return None, "Widget not found in the database"
    return widget, None


def check_existing_configuration(widget_id, config_name):
    widget_configs = local_widgets.get_widget_configurations(widget_id)
    for config in widget_configs:
        if config["name"] == config_name:
            return "Configuration with the same name already exists"
    return None


def get_config_path(widget, config_name):
    config_path = get_widget_configuration_path(widget["author"], widget["name"], config_name)
    try:
        if not os.path.exists(config_path):
            os.makedirs(config_path)
    except Exception as e:
        return None, "Failed to create configuration directory"
    return config_path, None


def save_configuration(file_base64, config_path):
    try:
        utils.extract_archive_from_base64_bytes(file_base64, config_path)
    except Exception as e:
        return "Failed to save configuration to the disk"
    return None


def remove_configuration_directory(config_path):
    try:
        os.system("rm -rf {}".format(config_path))
    except Exception as e:
        return "Failed to remove configuration directory, but configuration was deleted from the database"
    return None
