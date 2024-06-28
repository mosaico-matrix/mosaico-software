import os

import aiocoap.resource as resource

import coap.dynamic_resource
import rest.services.widgets as rest_widgets_api
import data.repositories.widgets as local_widgets
from data.repositories.widget_configurations import get_widget_configuration
from interop import call_matrix
from coap.responses import *
import configs
import git

logger = logging.getLogger(name="coap.repositories.widgets")


class InstalledWidgets(coap.dynamic_resource.DynamicResource):

    async def render_delete(self, request, args):
        """
        Delete selected widget from local db but keep the files and configurations
        """
        logger.info("Received DELETE request to installed_widgets")

        # Get widget_id from request
        widget_id = args["widget_id"]

        logger.info(f"Deleting widget with id: {widget_id}")

        # Get widget
        widget = local_widgets.get_widget(widget_id)
        if widget is None:
            return error_response("Widget not found")

        # Get widget folder path
        widget_path = configs.get_widget_path(widget["author"], widget["name"])

        # Delete widget from db
        local_widgets.delete_widget(widget_id)

        # Delete widget data
        if os.path.exists(widget_path):
            os.system(f"rm -rf {widget_path}")
            logger.info(f"Deleted widget data at: {widget_path}")
        else:
            logger.warning(f"Widget path: {widget_path} does not exist, skipping")

        return success_response(None, "Widget deleted successfully")

    async def render_get(self, request, args):
        """
        Get all installed widgets from local db
        """
        logger.info("Received GET request to installed_widgets")
        widgets = local_widgets.get_installed_widgets()
        return success_response(widgets)

    async def render_post(self, request, args):

        """
        Install a widget from the app store to the local db
        """
        logger.info("Received POST request to installed_widgets")

        # Get widget_store_id from args
        widget_store_id = args["widget_store_id"]

        # Check if already installed
        widget = local_widgets.get_widget_by_store_id(widget_store_id)

        if widget:
            logger.warning(f"Widget with id: {widget_store_id} is already installed, skipping")

            # Just return success response even if already installed
        else:
            # Get the widget from the app store
            logger.info(f"Installing widget with id: {widget_store_id}")

            widget = None
            try:  # Always catch API exceptions to inform the user
                widget = rest_widgets_api.get_widget(widget_store_id)
            except Exception as e:
                logger.error(f"Error getting widget with id: {widget_store_id}")
                return error_response(f"Could not get widget from app store")

            # Get the widget path
            widget_path = configs.get_widget_path(widget["user"]["username"], widget["name"])

            # Git clone the widget from provided repository
            repo_url = widget["repository_url"]

            # Remove if path already exists
            if os.path.exists(widget_path):
                logger.warning(f"Widget path: {widget_path} already exists, removing")
                os.system(f"rm -rf {widget_path}")

            # Create widget directory
            os.makedirs(widget_path)

            # Clone the repo with depth 1
            git.Repo.clone_from(repo_url, widget_path, depth=1)
            logger.info(f"Cloned widget from: {repo_url}")

            # Save the widget to the db
            local_widgets.add_widget(widget["id"], widget["name"], widget["user"]["username"])

        return success_response(None, "Widget installed successfully")


class ActiveWidget(resource.Resource):

    async def render_get(self, request):
        pass

    async def render_post(self, request):

        """
        Set the active widget
        """
        logger.info("Received POST request to active_widget")

        # Deserialize the request
        payload = json.loads(request.payload.decode())
        widget_id = payload["widget_id"]
        config_id = payload["config_id"]

        # Get widget
        widget = local_widgets.get_widget(widget_id)
        if widget is None:
            return error_response("Widget not found")

        # Get widget configuration
        widget_config = get_widget_configuration(config_id)
        if widget_config is None:
            return error_response("Provided widget configuration not found")

        # Set widget on matrix
        call_matrix("LOAD_WIDGET",
                    {
                        "widget_path": configs.get_widget_path(widget["author"], widget["name"]),
                        "config_path": configs.get_widget_configuration_path(widget["author"], widget["name"], widget_config["name"])
                     })

        return success_response(None, "Widget set successfully")


class WidgetConfigurationForm(coap.dynamic_resource.DynamicResource):
    """
    The purpose of this service is to provide the widget configuration form .json from the file system
    It is composed by only the get method which returns the json file used to build the configuration form
    """

    async def render_get(self, request, args):
        """
        Get the widget configuration form from the file system
        """

        # Get widget
        widget_id = args["widget_id"]
        widget = local_widgets.get_widget(widget_id)
        if widget is None:
            return error_response("Widget not found")

        # Get the widget installation path
        widget_path = configs.get_widget_path(widget["author"], widget["name"])

        # Get the configuration form
        form_path = f"{widget_path}/config-form.json"

        # Return the configuration form
        try:
            with open(form_path, "r") as f:
                return success_response(json.load(f))
        except Exception as e:
            return error_response("Could not get configuration form for widget")


class CustomWidget(resource.Resource):
    """
    This service allows developers to create a custom widget package and upload it directly to the system
    This skips the app store and the download from the git repository
    """

    async def render_post(self, request):
        """
        Upload a custom widget package in .tar.gz format
        """
        logger.info("Received POST request to custom_widget")

        pass